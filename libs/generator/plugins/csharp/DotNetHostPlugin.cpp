#include "DotNetHostPlugin.h"
#include "core/Annotations.h"
#include "core/St.h"

namespace holgen {
void DotNetHostPlugin::Run() {
  if (!mSettings.IsFeatureEnabled(TranslatorFeatureFlag::CSharp))
    return;
  auto &hostCls = CreateClass();
  PopulateClass(hostCls);
  for (auto &cls: mProject.mClasses) {
    if (!cls.mStruct || !cls.mStruct->GetAnnotation(Annotations::DotNetModule))
      continue;
    PopulateForModule(hostCls, cls);
  }
}

Class &DotNetHostPlugin::CreateClass() {
  auto cls = Class{St::DotNetHostName, mSettings.mNamespace};
  for (auto &header: {"hostfxr.h", "coreclr_delegates.h", "nethost.h"}) {
    cls.mHeaderIncludes.AddLocalHeader(header);
  }
  cls.mHeaderCustomSection.Add("#ifdef WINDOWS");
  cls.mHeaderCustomSection.Add("#include <Windows.h>");
  cls.mHeaderCustomSection.Add("#else");
  cls.mHeaderCustomSection.Add("#include <dlfcn.h>");
  cls.mHeaderCustomSection.Add("#endif");
  Validate().NewClass(cls);
  mProject.mClasses.push_back(std::move(cls));
  return mProject.mClasses.back();
}

void DotNetHostPlugin::PopulateClass(Class &cls) {
  cls.mFields.emplace_back(LibraryHandleField(), Type{"void", PassByType::Pointer},
                           Visibility::Private, Staticness::NotStatic, "nullptr");
  for (auto &hostfxrMethod: HostfxrMethods) {
    cls.mFields.emplace_back(HostfxrMethodFieldName(hostfxrMethod),
                             Type{HostfxrMethodTypedefName(hostfxrMethod)}, Visibility::Private,
                             Staticness::NotStatic, "nullptr");
  }

  cls.mFields.emplace_back(Naming().FieldNameInCpp("hostfxrHandle"), Type{"hostfxr_handle"},
                           Visibility::Private, Staticness::NotStatic, "nullptr");

  for (auto &name: HostfxrDelegates) {
    cls.mFields.emplace_back(Naming().FieldNameInCpp(std::format("hostfxrDelegate_{}", name)),
                             Type{std::format("{}_fn", name)}, Visibility::Private,
                             Staticness::NotStatic, "nullptr");
  }

  {
    auto &method = cls.mMethods.emplace_back(Naming().FieldNameInCpp("loadModule"), Type{"void"},
                                             Visibility::Private, Constness::NotConst);
    method.mArguments.emplace_back("absolutePath",
                                   Type{"char", PassByType::Pointer, Constness::Const});
    method.mFunctionPointer = true;
  }

  CreateLoadFunctionMethod(cls);
  CreateDestructor(cls);
  CreateLoadLibraryMethod(cls);
  CreateFreeLibraryMethod(cls);
  CreateLoadDelegateMethod(cls);
  CreateInitializeMethod(cls);
  CreateLoadBaseModuleMethod(cls);
  CreateInitializeHolgenMethod(cls);
}

void DotNetHostPlugin::CreateDestructor(Class &cls) {
  cls.mDestructor.mBody.Add("if ({}) {{", Naming().FieldNameInCpp("hostfxrHandle"));
  cls.mDestructor.mBody.Indent(1);
  cls.mDestructor.mBody.Add("{}({});", Naming().FieldNameInCpp("hostfxr_close"),
                            Naming().FieldNameInCpp("hostfxrHandle"));
  cls.mDestructor.mBody.Indent(-1);
  cls.mDestructor.mBody.Add("}}");

  cls.mDestructor.mBody.Add("if ({}) {{", LibraryHandleField());
  cls.mDestructor.mBody.Indent(1);
  cls.mDestructor.mBody.Add("FreeLibrary({});", LibraryHandleField());
  cls.mDestructor.mBody.Indent(-1);
  cls.mDestructor.mBody.Add("}}");
}

void DotNetHostPlugin::CreateInitializeMethod(Class &cls) {
  auto method =
      ClassMethod{St::DotNetHost_Initialize, Type{"void"}, Visibility::Public, Constness::NotConst};
  method.mArguments.emplace_back(
      "pathToBaseModule", Type{"std::filesystem::path", PassByType::Reference, Constness::Const});

  method.mBody.Add("auto pathToBaseModuleStr = pathToBaseModule.string();");
  method.mBody.Add("get_hostfxr_parameters params{{sizeof(get_hostfxr_parameters), "
                   "pathToBaseModuleStr.c_str(), nullptr}};");
  method.mBody.Add("std::vector<char_t> hostfxrPath(4096, '\\0');");
  method.mBody.Add("size_t hostfxrPathSize = hostfxrPath.size();");
  method.mBody.Add("int res = get_hostfxr_path(hostfxrPath.data(), &hostfxrPathSize, &params);");
  method.mBody.Add("HOLGEN_FAIL_IF(res != 0, \"get_hostfxr_path failed!\");");
  method.mBody.Add("{} = LoadLibrary(hostfxrPath.data());", LibraryHandleField());

  for (auto &hostfxrMethod: HostfxrMethods) {
    method.mBody.Add("{} = {}(LoadFunction({}, \"hostfxr_{}\"));",
                     HostfxrMethodFieldName(hostfxrMethod), HostfxrMethodTypedefName(hostfxrMethod),
                     LibraryHandleField(), hostfxrMethod);
  }
  method.mBody.Add("LoadBaseModule(pathToBaseModuleStr);", LibraryHandleField());

  cls.mMethods.push_back(std::move(method));
}

void DotNetHostPlugin::CreateLoadLibraryMethod(Class &cls) {
  auto method = ClassMethod{"LoadLibrary", Type{"void", PassByType::Pointer}, Visibility::Private};
  method.mArguments.emplace_back("path", Type{"char_t", PassByType::Pointer, Constness::Const});

  method.mBody.Add("#ifdef WINDOWS");
  method.mBody.Add("HMODULE h = ::LoadLibraryW(path);");
  method.mBody.Add("#else");
  method.mBody.Add("void* h = dlopen(path, RTLD_LAZY | RTLD_LOCAL);");
  method.mBody.Add("#endif");
  method.mBody.Add("HOLGEN_FAIL_IF(!h, \"Could not load hostfxr library!\");");
  method.mBody.Add("return h;");

  cls.mMethods.push_back(std::move(method));
}

void DotNetHostPlugin::CreateFreeLibraryMethod(Class &cls) {
  auto method = ClassMethod{"FreeLibrary", Type{"void"}, Visibility::Private};
  method.mArguments.emplace_back("h", Type{"void", PassByType::Pointer});

  method.mBody.Add("#ifdef WINDOWS");
  method.mBody.Add("::FreeLibrary((HMODULE)h);");
  method.mBody.Add("#else");
  method.mBody.Add("dlclose(h);");
  method.mBody.Add("#endif");

  cls.mMethods.push_back(std::move(method));
}

void DotNetHostPlugin::CreateLoadFunctionMethod(Class &cls) {
  auto method = ClassMethod{"LoadFunction", Type{"void", PassByType::Pointer}, Visibility::Private};
  method.mArguments.emplace_back("module", Type{"void", PassByType::Pointer});
  method.mArguments.emplace_back("name", Type{"char", PassByType::Pointer, Constness::Const});

  method.mBody.Add("#ifdef WINDOWS");
  method.mBody.Add("void *f = ::GetProcAddress((HMODULE)module, name);");
  method.mBody.Add("#else");
  method.mBody.Add("void *f = dlsym(module, name);");
  method.mBody.Add("#endif");
  method.mBody.Add("HOLGEN_FAIL_IF(!f, \"Could not load {{}} from hostfxr library!\", name);");
  method.mBody.Add("return f;");

  cls.mMethods.push_back(std::move(method));
}

void DotNetHostPlugin::CreateLoadDelegateMethod(Class &cls) {
  auto method = ClassMethod{"LoadDelegate", Type{"void", PassByType::Pointer}, Visibility::Private,
                            Constness::Const};
  method.mArguments.emplace_back("delegateType", Type{"hostfxr_delegate_type"});

  method.mBody.Add("void *f = nullptr;");
  method.mBody.Add("auto res = {}({}, delegateType, &f);",
                   HostfxrMethodFieldName("get_runtime_delegate"),
                   Naming().FieldNameInCpp("hostfxrHandle"));
  method.mBody.Add("HOLGEN_FAIL_IF(res < 0 || !f, \"Could not load function delegate {{}}\", "
                   "(int)delegateType);");
  method.mBody.Add("return f;");

  cls.mMethods.push_back(std::move(method));
}

void DotNetHostPlugin::GenerateLoadDelegatesMethod(CodeBlock &codeBlock) {
  codeBlock.Add("auto configPath = (modulePath / moduleName).string() + \".runtimeconfig.json\";",
                Naming().FieldNameInCpp("name"));
  codeBlock.Add("auto res = {}(configPath.c_str(), nullptr, &{});",
                HostfxrMethodFieldName("initialize_for_runtime_config"),
                Naming().FieldNameInCpp("hostfxrHandle"));
  codeBlock.Add(
      "HOLGEN_FAIL_IF(res < 0 || !{}, \"Could not initialize {{}}\", modulePath.string());",
      Naming().FieldNameInCpp("hostfxrHandle"));

  for (auto &name: HostfxrDelegates) {
    codeBlock.Add("{} = ({})LoadDelegate({});",
                  Naming().FieldNameInCpp(std::format("hostfxrDelegate_{}", name)),
                  std::format("{}_fn", name), std::format("hdt_{}", name));
  }
}

void DotNetHostPlugin::CreateLoadBaseModuleMethod(Class &cls) {
  auto method =
      ClassMethod{"LoadBaseModule", Type{"void"}, Visibility::Private, Constness::NotConst};
  method.mArguments.emplace_back(
      "modulePath", Type{"std::filesystem::path", PassByType::Reference, Constness::Const});

  method.mBody.Add("auto moduleName = modulePath.filename().string();");
  GenerateLoadDelegatesMethod(method.mBody);

  method.mBody.Add("auto dllPath = (modulePath / moduleName).string() + \".dll\";");
  method.mBody.Add("res = {}(dllPath.c_str(), nullptr, nullptr);",
                   Naming().FieldNameInCpp("hostfxrDelegate_load_assembly"));
  method.mBody.Add(
      "HOLGEN_FAIL_IF(res < 0, \"Could not load base assembly {{}}\", modulePath.string());");

  method.mBody.Add("InitializeHolgen();");
  cls.mMethods.push_back(std::move(method));
}

void DotNetHostPlugin::PopulateForModule(Class &cls, const Class &moduleCls) {
  auto field = ClassField{Naming().FieldNameInCpp(moduleCls.mName + "s"), Type{"std::deque"}};
  field.mType.mTemplateParameters.emplace_back(moduleCls.mName);
  Validate().NewField(cls, field);
  cls.mFields.emplace_back(std::move(field));
  CreateLoadModuleMethod(cls, moduleCls);
}

void DotNetHostPlugin::CreateInitializeHolgenMethod(Class &cls) {
  cls.mSourceIncludes.AddStandardHeader("vector");
  auto method =
      ClassMethod{"InitializeHolgen", Type{"void"}, Visibility::Private, Constness::NotConst};
  GenerateInitializeDotNetInterface(method.mBody);
  GenerateInitializeMetaClasses(method.mBody);

  for (auto &csCls: mProject.mCSharpClasses) {
    if (!csCls.mClass || !ShouldInitializeClass(*csCls.mClass))
      continue;
    GenerateInitializeClass(cls, method.mBody, *csCls.mClass);
  }

  method.mBody.Add("void (*baseModuleInitialize)();");
  method.mBody.Add("auto res = {0}(\"{1}.HolgenMain, {1}\", \"Initialize\", "
                   "\"{1}.HolgenMain+InitializeDelegate, {1}\", nullptr, "
                   "nullptr, (void**)(&baseModuleInitialize));",
                   Naming().FieldNameInCpp("hostfxrDelegate_get_function_pointer"),
                   St::CSharpProjectName);
  method.mBody.Add("HOLGEN_FAIL_IF(res < 0 || !baseModuleInitialize, \"Initialize method not found "
                   "in main module!\");");
  method.mBody.Add("baseModuleInitialize();");

  method.mBody.Add("res = {0}(\"{1}.HolgenMain, {1}\", \"LoadModule\", "
                   "\"{1}.HolgenMain+LoadModuleDelegate, {1}\", nullptr, "
                   "nullptr, (void**)(&{2}));",
                   Naming().FieldNameInCpp("hostfxrDelegate_get_function_pointer"),
                   St::CSharpProjectName, Naming().FieldNameInCpp("loadModule"));
  method.mBody.Add(
      "HOLGEN_FAIL_IF(res < 0 || !{}, \"LoadModule method not found in main module!\");",
      Naming().FieldNameInCpp("loadModule"));
  cls.mMethods.push_back(std::move(method));
}

void DotNetHostPlugin::GenerateInitializeMetaClasses(CodeBlock &codeBlock) {
  GenerateInitializeMetaClassMethod(codeBlock, St::DeferredDeleter,
                                    St::DeferredDeleterPerformManaged);
  GenerateInitializeMetaClassMethod(codeBlock, St::DeferredDeleter,
                                    St::DeferredDeleterPerformManagedArray);
}

void DotNetHostPlugin::GenerateInitializeMetaClassMethod(CodeBlock &codeBlock,
                                                         const std::string &className,
                                                         const std::string &methodName) {
  auto delegateName = std::format("{0}.{1}+{2}, {0}", St::CSharpProjectName, className,
                                  Naming().CSharpMethodDelegateName(className, methodName));
  auto classFullName = std::format("{0}.{1}, {0}", St::CSharpProjectName, className);

  codeBlock.Add("{{");
  codeBlock.Indent(1);
  codeBlock.Add("auto res = {0}(\"{1}\", \"{3}\", "
                "\"{4}\", nullptr, "
                "nullptr, (void**)(&{2}::{3}));",
                Naming().FieldNameInCpp("hostfxrDelegate_get_function_pointer"), classFullName,
                className, methodName, delegateName);
  codeBlock.Add(
      "HOLGEN_FAIL_IF(res < 0 || !{0}::{1}, \"Required meta method {1} not found in {0}\");",
      className, methodName);
  codeBlock.Indent(-1);
  codeBlock.Add("}}");
}

void DotNetHostPlugin::CreateLoadModuleMethod(Class &cls, const Class &moduleCls) {
  auto method =
      ClassMethod{"LoadCustom" + moduleCls.mName, Type{moduleCls.mName, PassByType::Reference},
                  Visibility::Public, Constness::NotConst};
  method.mArguments.emplace_back(
      "modulePath", Type{"std::filesystem::path", PassByType::Reference, Constness::Const});

  auto moduleNameAccessor = Naming().FieldNameInCpp("name");
  method.mBody.Add("auto& module = {}.emplace_back();",
                   Naming().FieldNameInCpp(moduleCls.mName + "s"));
  method.mBody.Add("module.{} = modulePath.filename().string();", moduleNameAccessor);


  method.mBody.Add(
      "auto dllPath = std::filesystem::absolute(modulePath / (module.{} + \".dll\")).string();",
      moduleNameAccessor);
  method.mBody.Add("{}(dllPath.c_str());", Naming().FieldNameInCpp("loadModule"));


  method.mBody.Add("auto className = std::format(\"{{0}}.{}, {{0}}\", module.{});", moduleCls.mName,
                   moduleNameAccessor);
  for (auto &moduleClsMethod: moduleCls.mMethods) {
    if (!moduleClsMethod.mFunctionPointer)
      continue;
    method.mBody.Add("{{");
    method.mBody.Indent(1);
    auto delegateName = std::format("{0}.{1}+{2}, {0}", St::CSharpProjectName,
                                    Naming().ModuleInterfaceNameInCSharp(moduleCls.mName),
                                    Naming().CSharpMethodDelegateName(moduleCls, moduleClsMethod));
    method.mBody.Add("auto res = {0}(className.c_str(), \"{1}\", "
                     "\"{2}\", nullptr, "
                     "nullptr, (void**)(&module.{1}));",
                     Naming().FieldNameInCpp("hostfxrDelegate_get_function_pointer"),
                     moduleClsMethod.mName, delegateName);
    method.mBody.Add(
        "HOLGEN_FAIL_IF(res < 0 || !module.{0}, \"Required module method {0} not found "
        "in {{}}\", module.{1});",
        moduleClsMethod.mName, moduleNameAccessor);
    method.mBody.Indent(-1);
    method.mBody.Add("}}");
  }

  method.mBody.Add("return module;");
  Validate().NewMethod(cls, method);
  cls.mMethods.push_back(std::move(method));
}

void DotNetHostPlugin::GenerateInitializeDotNetInterface(CodeBlock &codeBlock) {
  codeBlock.Add("{{");
  codeBlock.Indent(1);
  codeBlock.Add("auto res = {}(", Naming().FieldNameInCpp("hostfxrDelegate_get_function_pointer"));
  codeBlock.Indent(1);
  codeBlock.Add(R"("{0}.{1}, {0}", "{2}",)", St::CSharpProjectName, St::CSharpInterfaceName,
                St::CSharpInterfaceFree);
  codeBlock.Add(
      "\"{0}.{1}+{2}, {0}\",", St::CSharpProjectName, St::CSharpInterfaceName,
      Naming().CSharpMethodDelegateName(St::CSharpInterfaceName, St::CSharpInterfaceFree));
  codeBlock.Add("nullptr, nullptr, (void**)(&{}::{}));", St::CSharpInterfaceName,
                St::CSharpInterfaceFreePtr);
  codeBlock.Indent(-1);
  codeBlock.Add("HOLGEN_FAIL_IF(res < 0 || !{0}::{1}, \"Could not get {1} of {0}, was the project "
                "built after the cs files were generated?\");",
                St::CSharpInterfaceName, St::CSharpInterfaceFreePtr);
  codeBlock.Indent(-1);
  codeBlock.Add("}}");
}

void DotNetHostPlugin::GenerateInitializeClass(Class &cls, CodeBlock &codeBlock,
                                               const Class &projectCls) {
  cls.mSourceIncludes.AddLocalHeader(projectCls.mName + ".h");
  codeBlock.Add("{{");
  codeBlock.Indent(1);

  if (projectCls.mStruct && projectCls.mStruct->GetAnnotation(Annotations::DotNetInterface))
    GenerateInitializeInterface(codeBlock, projectCls);
  else
    GenerateInitializeNonInterface(codeBlock, projectCls);
  codeBlock.Indent(-1);
  codeBlock.Add("}}");
}

void DotNetHostPlugin::GenerateInitializeNonInterface(CodeBlock &codeBlock,
                                                      const Class &projectCls) {

  std::stringstream initFuncDefinition;
  std::stringstream initFuncCallArgs;
  initFuncDefinition << "void (*initFunc)(";
  bool isFirstMethod = true;
  for (auto &func: projectCls.mCFunctions) {
    if (!ShouldInitializeCFunction(func))
      continue;
    if (isFirstMethod) {
      isFirstMethod = false;
    } else {
      initFuncDefinition << ", ";
      initFuncCallArgs << ", ";
    }
    initFuncDefinition << func.mReturnType.ToString(false, true) << "(*)(";
    initFuncCallArgs << func.mName;

    bool isFirstArg = true;
    for (auto &arg: func.mArguments) {
      if (isFirstArg)
        isFirstArg = false;
      else
        initFuncDefinition << ", ";
      initFuncDefinition << arg.mType.ToString(true, true);
    }
    initFuncDefinition << ")";
  }
  initFuncDefinition << ");";
  codeBlock.Add("{}", initFuncDefinition.str());

  codeBlock.Add("auto res = {}(", Naming().FieldNameInCpp("hostfxrDelegate_get_function_pointer"));
  codeBlock.Indent(1);
  codeBlock.Add(R"("{0}.{1}, {0}", "{2}",)", St::CSharpProjectName, projectCls.mName,
                St::CSharpInitializerMethodName);
  codeBlock.Add(
      "\"{0}.{1}+{2}, {0}\",", St::CSharpProjectName, projectCls.mName,
      Naming().CSharpMethodDelegateName(projectCls.mName, St::CSharpInitializerMethodName));
  codeBlock.Add("nullptr, nullptr, (void**)(&initFunc));");
  codeBlock.Indent(-1);
  codeBlock.Add("HOLGEN_FAIL_IF(res < 0 || !initFunc, \"Could not initialize {}, was the project "
                "built after the cs files were generated?\");",
                projectCls.mName);
  codeBlock.Add("initFunc({});", initFuncCallArgs.str());
}

void DotNetHostPlugin::GenerateInitializeInterface(CodeBlock &codeBlock, const Class &projectCls) {
  std::string resPrefix = "auto ";
  for (auto &method: projectCls.mMethods) {
    if (!method.mExposeToCSharp)
      continue;

    codeBlock.Add("{}res = {}(", resPrefix,
                  Naming().FieldNameInCpp("hostfxrDelegate_get_function_pointer"));
    resPrefix.clear();
    codeBlock.Indent(1);
    codeBlock.Add(R"("{0}.{1}, {0}", "{2}{3}",)", St::CSharpProjectName, projectCls.mName,
                  method.mName, St::CSharpInterfaceFunctionCallerSuffix);
    codeBlock.Add("\"{0}.{1}+{2}, {0}\",", St::CSharpProjectName, projectCls.mName,
                  Naming().CSharpMethodDelegateName(projectCls.mName, method.mName));
    codeBlock.Add("nullptr, nullptr, (void**)(&{}::{}));", projectCls.mName,
                  method.mName + St::CSharpInterfaceFunctionSuffix);
    codeBlock.Indent(-1);
    codeBlock.Add(
        "HOLGEN_FAIL_IF(res < 0 || !{0}::{1}{2}, \"Could not get {1} of {0}, was the project "
        "built after the cs files were generated?\");",
        projectCls.mName, method.mName, St::CSharpInterfaceFunctionSuffix);
  }
}

bool DotNetHostPlugin::ShouldInitializeClass(const Class &cls) {
  if (cls.mStruct &&
      (cls.mStruct->GetMatchingAnnotation(Annotations::No, Annotations::No_Script) ||
       cls.mStruct->GetMatchingAnnotation(Annotations::No, Annotations::No_CSharp) ||
       cls.mStruct->GetAnnotation(Annotations::DotNetModule)))
    return false;
  return true;
}

bool DotNetHostPlugin::ShouldInitializeCFunction(const CFunction &function) {
  // TODO: this is the same as CSharpWrapperGenerator::ShouldProcess. Put it in CSharpHelper
  if (!function.mMethod)
    return false;
  if (!function.mMethod->mExposeToCSharp)
    return false;
  if (function.mMethod->mFunction &&
      (function.mMethod->mFunction->GetMatchingAttribute(Annotations::No, Annotations::No_Script) ||
       function.mMethod->mFunction->GetMatchingAttribute(Annotations::No, Annotations::No_CSharp)))
    return false;
  return true;
}

std::string DotNetHostPlugin::LibraryHandleField() const {
  return Naming().FieldNameInCpp("hostfxrLibHandle");
}

std::string DotNetHostPlugin::HostfxrMethodFieldName(const std::string &method) const {
  return Naming().FieldNameInCpp("hostfxr_") + method;
}

std::string DotNetHostPlugin::HostfxrMethodTypedefName(const std::string &method) const {
  return std::format("hostfxr_{}_fn", method);
}
} // namespace holgen
