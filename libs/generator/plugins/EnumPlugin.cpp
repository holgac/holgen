#include "EnumPlugin.h"
#include "core/St.h"
#include <format>

namespace holgen {
  void EnumPlugin::Run() {

    for (auto &generatedClass: mProject.mClasses) {
      if (generatedClass.mEnum == nullptr)
        continue;
      auto &enumDefinition = *generatedClass.mEnum;
      GenerateCommon(generatedClass, enumDefinition);
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

  void EnumPlugin::GenerateCommon(Class &generatedClass, const EnumDefinition &enumDefinition) {
    generatedClass.mUsings.emplace_back(Type{"int64_t"}, "UnderlyingType");
    generatedClass.mFields.emplace_back("mValue", Type{"UnderlyingType"});
    {
      ClassConstructor &ctor = generatedClass.mConstructors.emplace_back();
      ctor.mExplicitness = Explicitness::Explicit;
      ctor.mArguments.emplace_back("value", Type{"UnderlyingType"}, "Invalid");
      ctor.mInitializerList.emplace_back("mValue", "value");
    }

    {
      ClassMethod &valueGetter = generatedClass.mMethods.emplace_back("Get", Type{"UnderlyingType"});
      valueGetter.mBody.Add("return mValue;");
    }

    for (auto &entry: enumDefinition.mEntries) {
      ClassField &valueField = generatedClass.mFields.emplace_back(
          entry.mName + "Value",
          Type{"UnderlyingType", PassByType::Value, Constness::Const},
          Visibility::Public,
          Staticness::Static,
          entry.mValue
      );
      valueField.mType.mConstexprness = Constexprness::Constexpr;

      ClassField &entryField = generatedClass.mFields.emplace_back(
          entry.mName,
          Type{generatedClass.mName, PassByType::Value, Constness::Const},
          Visibility::Public,
          Staticness::Static
      );
      entryField.mDefaultConstructorArguments.push_back(entry.mValue);
    }

    {
      ClassField &invalidEntry = generatedClass.mFields.emplace_back(
          "Invalid",
          Type{"UnderlyingType", PassByType::Value, Constness::Const},
          Visibility::Public,
          Staticness::Static,
          enumDefinition.mInvalidValue
      );
      invalidEntry.mType.mConstexprness = Constexprness::Constexpr;
      invalidEntry.mDefaultValue = enumDefinition.mInvalidValue;
    }

    {
      ClassMethod &fromString = generatedClass.mMethods.emplace_back(
          "FromString", Type{generatedClass.mName},
          Visibility::Public, Constness::NotConst, Staticness::Static
      );
      fromString.mArguments.emplace_back("str", Type{"std::string_view"});
      bool isFirst = true;
      for (auto &entry: enumDefinition.mEntries) {
        if (isFirst) {
          isFirst = false;
          fromString.mBody.Add("if (str == \"{}\") {{", entry.mName);
        } else {
          fromString.mBody.Add("}} else if (str == \"{}\") {{", entry.mName);
        }
        fromString.mBody.Indent(1);
        fromString.mBody.Add("return {}({});", generatedClass.mName, entry.mValue);
        fromString.mBody.Indent(-1);
      }
      if (!enumDefinition.mEntries.empty()) {
        fromString.mBody.Add("}} else {{");
        fromString.mBody.Indent(1);
      }
      fromString.mBody.Add("return {}({}::Invalid);", generatedClass.mName, generatedClass.mName);
      if (!enumDefinition.mEntries.empty()) {
        fromString.mBody.Indent(-1);
        fromString.mBody.Add("}}");
      }
    }

    {
      // TODO: it'd be better to return a char* here since everything is const already.
      // TODO: implement formatter and << operators
      ClassMethod &toString = generatedClass.mMethods.emplace_back(
          "ToString",
          Type{"std::string"},
          Visibility::Public,
          Constness::Const
      );
      toString.mBody.Add("switch (mValue) {{");
      toString.mBody.Indent(1);
      for (auto &entry: enumDefinition.mEntries) {
        toString.mBody.Add("case {}: return \"{}\";", entry.mValue, entry.mName);
      }
      toString.mBody.Add("default: return \"INVALID\";");
      toString.mBody.Indent(-1);
      toString.mBody.Add("}}"); // switch
    }

    GenerateOperators(generatedClass);
    GenerateGetEntries(generatedClass, true);
    GenerateGetEntries(generatedClass, false);
  }

  void EnumPlugin::GenerateOperators(Class &generatedClass) {
    // TODO: test these properly. Currently overloads aren't well supported.
    const std::vector<EnumOperator> operators = {
        {"=",  Constness::NotConst, EnumOperatorReturnType::This,   true},
        {"==", Constness::Const,    EnumOperatorReturnType::Result, true},
        {"==", Constness::Const,    EnumOperatorReturnType::Result, false},
        {"!=", Constness::Const,    EnumOperatorReturnType::Result, true},
        {"!=", Constness::Const,    EnumOperatorReturnType::Result, false}
    };

    for (auto &op: operators) {
      ClassMethod &opMethod = generatedClass.mMethods.emplace_back(
          "operator " + op.mOperator, Type{"bool"}, Visibility::Public, op.mConstness
      );
      if (op.mReturn == EnumOperatorReturnType::This) {
        opMethod.mReturnType.mName = generatedClass.mName;
        opMethod.mReturnType.mType = PassByType::Reference;
      }
      if (op.mIntegralArgument)
        opMethod.mArguments.emplace_back("rhs", Type{"UnderlyingType"});
      else
        opMethod.mArguments.emplace_back("rhs", Type{generatedClass.mName, PassByType::Reference, Constness::Const});
      {
        auto line = opMethod.mBody.Line();
        if (op.mReturn == EnumOperatorReturnType::Result)
          line << "return ";
        line << "mValue";
        line << " " << op.mOperator << " rhs";
        if (!op.mIntegralArgument)
          line << ".mValue";
        line << ";";
      }
      if (op.mReturn == EnumOperatorReturnType::This)
        opMethod.mBody.Add("return *this;");
    }
  }

  void EnumPlugin::GenerateGetEntries(Class &generatedClass, bool forValues) {
    auto &method = generatedClass.mMethods.emplace_back(
        forValues ? "GetEntryValues" : "GetEntries",
        Type{"std::array"},
        Visibility::Public,
        Constness::NotConst,
        Staticness::Static
    );
    if (forValues) {
      method.mConstexprness = Constexprness::Constexpr;
      method.mReturnType.mTemplateParameters.emplace_back(generatedClass.mName + "::UnderlyingType");
    } else {
      method.mReturnType.mTemplateParameters.emplace_back(generatedClass.mName);
    }
    method.mReturnType.mTemplateParameters.emplace_back(
        std::format("{}", generatedClass.mEnum->mEntries.size()));
    auto line = method.mBody.Line();
    line << "return " << method.mReturnType.ToString() << "{";
    for (size_t i = 0; i < generatedClass.mEnum->mEntries.size(); ++i) {
      if (i > 0)
        line << ", ";
      if (forValues)
        line << generatedClass.mEnum->mEntries[i].mName + "Value";
      else
        line << generatedClass.mEnum->mEntries[i].mName;
    }
    line << "};";
  }
}
