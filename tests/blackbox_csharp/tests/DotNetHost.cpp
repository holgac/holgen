#include "DotNetHost.h"

void DotNetHost::Start() {
  char pathStr[MAX_PATH];
  getcwd(pathStr, MAX_PATH - 1);
  std::filesystem::path path = pathStr;
  path = path / "bin";
  Initialize(path / "TestModule", {path / "TestModule2"});
  // Initialize(path / "TestModule", {});
}

void DotNetHost::Initialize(const std::filesystem::path &baseModule,
                            const std::vector<std::filesystem::path> &modules) {
  auto rootPathStr = baseModule.string();
  get_hostfxr_parameters params{sizeof(get_hostfxr_parameters), rootPathStr.c_str(), nullptr};
  // // Pre-allocate a large buffer for the path to hostfxr
  char_t hostFXRPath[MAX_PATH];
  size_t buffer_size = sizeof(hostFXRPath) / sizeof(char_t);
  int res = get_hostfxr_path(hostFXRPath, &buffer_size, &params);
  HOLGEN_FAIL_IF(res != 0, "get_hostfxr_path failed!");
  mHostFXRModule = LoadLibrary(hostFXRPath);

  // auto hostFXRPath = rootPath / GetHostFXRPath();
  // mHostFXRModule = LoadLibrary(hostFXRPath.c_str());
  HOLGEN_FAIL_IF(mHostFXRModule == nullptr, "hostFXR module not found!");
  LoadFunction(mHostFXRModule, "hostfxr_initialize_for_dotnet_command_line",
               mHostFXR_initialize_for_dotnet_command_line);
  LoadFunction(mHostFXRModule, "hostfxr_initialize_for_runtime_config",
               mHostFXR_initialize_for_runtime_config);
  LoadFunction(mHostFXRModule, "hostfxr_get_runtime_delegate", mHostFXR_get_runtime_delegate);
  LoadFunction(mHostFXRModule, "hostfxr_close", mHostFXR_close);
  HOLGEN_FAIL_IF(!mHostFXR_initialize_for_dotnet_command_line ||
                     !mHostFXR_initialize_for_runtime_config || !mHostFXR_get_runtime_delegate ||
                     !mHostFXR_close,
                 "HostFXR methods not found!");

  LoadModule(baseModule);
  for (auto &modulePath: modules) {
    LoadModule(modulePath);
  }
  for (auto &[moduleName, module]: mModules) {
    InitializeModule(moduleName, module);
  }
}

void *DotNetHost::LoadLibrary(const char_t *path) {
#ifdef WINDOWS
  HMODULE h = ::LoadLibraryW(path);
  assert(h != nullptr);
  return (void *)h;
#else
  void *h = dlopen(path, RTLD_LAZY | RTLD_LOCAL);
  assert(h != nullptr);
  return h;
#endif
}

void *DotNetHost::LoadFunction(void *dl, const char *name) {
#ifdef WINDOWS
  void *f = ::GetProcAddress((HMODULE)dl, name);
#else
  void *f = dlsym(dl, name);
#endif
  assert(f != nullptr);
  return f;
}

void *DotNetHost::LoadDelegate(hostfxr_handle hostFXRHandle, hostfxr_delegate_type delegateType) {
  void *f = nullptr;
  auto res = mHostFXR_get_runtime_delegate(hostFXRHandle, delegateType, &f);
  HOLGEN_FAIL_IF(res != 0 || f == nullptr, "Could not load function delegate {}",
                 (int)delegateType);
  return f;
}

void DotNetHost::LoadModule(const std::filesystem::path &modulePath) {
  auto moduleName = modulePath.filename().string();
  auto [it, moduleRes] = mModules.emplace(moduleName, DotNetModule{});
  HOLGEN_FAIL_IF(!moduleRes, "Module {} was already loaded!", moduleName);
  auto &module = it->second;

  auto configPath = (modulePath / moduleName).string() + ".runtimeconfig.json";
  auto res =
      mHostFXR_initialize_for_runtime_config(configPath.c_str(), nullptr, &module.mHostFXRHandle);
  HOLGEN_FAIL_IF(res < 0 || module.mHostFXRHandle == nullptr, "Could not initialize {}",
                 modulePath.string());
  LoadDelegate(module.mHostFXRHandle, hdt_load_assembly, module.mHostFXRDelegate_load_assembly);
  LoadDelegate(module.mHostFXRHandle, hdt_get_function_pointer,
               module.mHostFXRDelegate_get_function_pointer);

  auto dllPath = (modulePath / moduleName).string() + ".dll";
  module.mHostFXRDelegate_load_assembly(dllPath.c_str(), nullptr, nullptr);
}

void DotNetHost::InitializeModule(const std::string &moduleName, DotNetModule &module) {
  // TODO: actual signature will depend on the settings defined in the schema
  int(CORECLR_DELEGATE_CALLTYPE * initializeMethod)();
  auto dotnetTypeName = std::format("{0}.Module, {0}", moduleName);
  auto res = module.mHostFXRDelegate_get_function_pointer(
      dotnetTypeName.c_str(), "Initialize",
      UNMANAGEDCALLERSONLY_METHOD, nullptr, nullptr,
      // delegateName.c_str(), nullptr, nullptr,
      (void **)&initializeMethod);
  HOLGEN_FAIL_IF(res < 0, "Could not get Initialize method of {}: 0x{:x}", dotnetTypeName, uint32_t(res));
  HOLGEN_FAIL_IF(initializeMethod() != 0, "Initialization failed!");
}
