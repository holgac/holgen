#include "EnumPlugin.h"
#include "core/St.h"
#include <format>

namespace holgen {
  void EnumPlugin::Run() {

    for (auto &cls: mProject.mClasses) {
      if (cls.mEnum == nullptr)
        continue;
      GenerateCommon(cls);
      Validate().Enum(cls);
    }
  }

  namespace {
    enum class EnumOperatorReturnType {
      This,
      Result
    };
    struct EnumOperator {
      std::string mOperator;
      Constness mConstness;
      EnumOperatorReturnType mReturn;
      bool mIntegralArgument;
    };
  }

  void EnumPlugin::GenerateCommon(Class &cls) {
    cls.mUsings.emplace_back(Type{"int64_t"}, St::Enum_UnderlyingType);

    {
      auto field = ClassField{"mValue", Type{St::Enum_UnderlyingType}};
      Validate().NewField(cls, field);
      cls.mFields.push_back(std::move(field));
      // TODO: validate ctors
      ClassConstructor &ctor = cls.mConstructors.emplace_back();
      ctor.mExplicitness = Explicitness::Explicit;
      ctor.mArguments.emplace_back("value", Type{St::Enum_UnderlyingType}, "Invalid");
      ctor.mInitializerList.emplace_back("mValue", "value");
    }

    GenerateGetValue(cls);
    GenerateEntries(cls);
    GenerateFromString(cls);
    GenerateToString(cls);
    GenerateOperators(cls);
    GenerateGetEntries(cls, true);
    GenerateGetEntries(cls, false);
  }

  void EnumPlugin::GenerateGetValue(Class &cls) {
    // TODO: rename to GetValue
    auto method = ClassMethod{"Get", Type{St::Enum_UnderlyingType}};
    method.mBody.Add("return mValue;");
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }

  void EnumPlugin::GenerateOperators(Class &cls) {
    // TODO: test these properly. Currently overloads aren't well supported.
    const std::vector<EnumOperator> operators = {
        {"=",  Constness::NotConst, EnumOperatorReturnType::This,   true},
        {"==", Constness::Const,    EnumOperatorReturnType::Result, true},
        {"==", Constness::Const,    EnumOperatorReturnType::Result, false},
        {"!=", Constness::Const,    EnumOperatorReturnType::Result, true},
        {"!=", Constness::Const,    EnumOperatorReturnType::Result, false}
    };

    for (auto &op: operators) {
      auto method = ClassMethod{
          "operator " + op.mOperator, Type{"bool"}, Visibility::Public, op.mConstness
      };
      if (op.mReturn == EnumOperatorReturnType::This) {
        method.mReturnType.mName = cls.mName;
        method.mReturnType.mType = PassByType::Reference;
      }
      if (op.mIntegralArgument)
        method.mArguments.emplace_back("rhs", Type{St::Enum_UnderlyingType});
      else
        method.mArguments.emplace_back("rhs", Type{cls.mName, PassByType::Reference, Constness::Const});
      {
        auto line = method.mBody.Line();
        if (op.mReturn == EnumOperatorReturnType::Result)
          line << "return ";
        line << "mValue";
        line << " " << op.mOperator << " rhs";
        if (!op.mIntegralArgument)
          line << ".mValue";
        line << ";";
      }
      if (op.mReturn == EnumOperatorReturnType::This)
        method.mBody.Add("return *this;");
      Validate().NewMethod(cls, method);
      cls.mMethods.push_back(std::move(method));
    }
  }

  void EnumPlugin::GenerateGetEntries(Class &cls, bool forValues) {
    auto method = ClassMethod{
        forValues ? "GetEntryValues" : "GetEntries",
        Type{"std::array"},
        Visibility::Public,
        Constness::NotConst,
        Staticness::Static
    };
    if (forValues) {
      method.mConstexprness = Constexprness::Constexpr;
      method.mReturnType.mTemplateParameters.emplace_back(
          std::format("{}::{}", cls.mName, St::Enum_UnderlyingType));
    } else {
      method.mReturnType.mTemplateParameters.emplace_back(cls.mName);
    }
    method.mReturnType.mTemplateParameters.emplace_back(
        std::format("{}", cls.mEnum->mEntries.size()));
    {
      auto line = method.mBody.Line();
      line << "return " << method.mReturnType.ToString() << "{";
      for (size_t i = 0; i < cls.mEnum->mEntries.size(); ++i) {
        if (i > 0)
          line << ", ";
        if (forValues)
          line << cls.mEnum->mEntries[i].mName + "Value";
        else
          line << cls.mEnum->mEntries[i].mName;
      }
      line << "};";
    }
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }

  void EnumPlugin::GenerateEntries(Class &cls) {
    // TODO: iterate twice so that all entries are added before all values and in the same order.
    for (auto &entry: cls.mEnum->mEntries) {
      auto valueField = ClassField{
          entry.mName + "Value",
          Type{St::Enum_UnderlyingType, PassByType::Value, Constness::Const},
          Visibility::Public,
          Staticness::Static,
          entry.mValue
      };
      valueField.mType.mConstexprness = Constexprness::Constexpr;
      valueField.mEntry = &entry;
      auto entryField = ClassField{
          entry.mName,
          Type{cls.mName, PassByType::Value, Constness::Const},
          Visibility::Public,
          Staticness::Static
      };
      entryField.mDefaultConstructorArguments.push_back(entry.mValue);
      entryField.mEntry = &entry;

      Validate().NewField(cls, valueField);
      cls.mFields.push_back(std::move(valueField));
      Validate().NewField(cls, entryField);
      cls.mFields.push_back(std::move(entryField));

    }

    {
      auto invalidEntry = ClassField{
          "Invalid",
          Type{St::Enum_UnderlyingType, PassByType::Value, Constness::Const},
          Visibility::Public,
          Staticness::Static,
          cls.mEnum->mInvalidValue
      };
      invalidEntry.mType.mConstexprness = Constexprness::Constexpr;
      invalidEntry.mDefaultValue = cls.mEnum->mInvalidValue;
      Validate().NewField(cls, invalidEntry);
      cls.mFields.push_back(std::move(invalidEntry));
    }
  }

  void EnumPlugin::GenerateFromString(Class &cls) {
    auto method = ClassMethod{
        "FromString", Type{cls.mName},
        Visibility::Public, Constness::NotConst, Staticness::Static
    };
    method.mArguments.emplace_back("str", Type{"std::string_view"});
    bool isFirst = true;
    for (auto &entry: cls.mEnum->mEntries) {
      if (isFirst) {
        isFirst = false;
        method.mBody.Add("if (str == \"{}\") {{", entry.mName);
      } else {
        method.mBody.Add("}} else if (str == \"{}\") {{", entry.mName);
      }
      method.mBody.Indent(1);
      method.mBody.Add("return {}({});", cls.mName, entry.mValue);
      method.mBody.Indent(-1);
    }
    if (!cls.mEnum->mEntries.empty()) {
      method.mBody.Add("}} else {{");
      method.mBody.Indent(1);
    }
    method.mBody.Add("return {}({}::Invalid);", cls.mName, cls.mName);
    if (!cls.mEnum->mEntries.empty()) {
      method.mBody.Indent(-1);
      method.mBody.Add("}}");
    }
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }

  void EnumPlugin::GenerateToString(Class &cls) {
    // TODO: implement formatter and << operators
    auto method = ClassMethod{
        "ToString",
        Type{"char", PassByType::Pointer, Constness::Const},
        Visibility::Public,
        Constness::Const
    };
    method.mBody.Add("switch (mValue) {{");
    method.mBody.Indent(1);
    for (auto &entry: cls.mEnum->mEntries) {
      method.mBody.Add("case {}: return \"{}\";", entry.mValue, entry.mName);
    }
    method.mBody.Add("default: return \"INVALID\";");
    method.mBody.Indent(-1);
    method.mBody.Add("}}"); // switch
    Validate().NewMethod(cls, method);
    cls.mMethods.push_back(std::move(method));
  }
}
