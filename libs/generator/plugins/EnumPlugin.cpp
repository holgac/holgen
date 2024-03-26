#include "EnumPlugin.h"
#include "core/St.h"

namespace holgen {
  void EnumPlugin::Run() {

    for (auto &generatedClass: mProject.mClasses) {
      if (generatedClass.mEnum == nullptr)
        continue;
      auto &enumDefinition = *generatedClass.mEnum;
      GenerateCommon(generatedClass, enumDefinition);
    }
  }

  void EnumPlugin::GenerateCommon(Class &generatedClass, const EnumDefinition &enumDefinition) {
    generatedClass.mTypedefs.emplace_back(Type{"int64_t"}, "UnderlyingType");
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

    std::vector<std::pair<std::string, std::pair<bool, bool>>> operations = {
        {"=",  {false, false}},
        {"==", {true,  true}},
        {"!=", {true,  true}}
    };
    for (int i = 0; i < 2; ++i) {
      bool isForIntegral = i == 0;
      for (auto&[op, settings]: operations) {
        auto&[isConst, returnResult] = settings;
        ClassMethod &opMethod = generatedClass.mMethods.emplace_back(
            "operator " + op, Type{"bool"}
        );
        opMethod.mName = "operator " + op;
        if (!returnResult) {
          opMethod.mReturnType.mName = generatedClass.mName;
          opMethod.mReturnType.mType = PassByType::Reference;
        }
        auto &rhs = opMethod.mArguments.emplace_back("rhs", Type{"UnderlyingType"});
        rhs.mName = "rhs";
        if (!isForIntegral) {
          rhs.mType.mConstness = Constness::Const;
          rhs.mType.mName = generatedClass.mName;
          rhs.mType.mType = PassByType::Reference;
        }
        opMethod.mConstness = isConst ? Constness::Const : Constness::NotConst;
        {
          auto line = opMethod.mBody.Line();
          if (returnResult)
            line << "return ";
          line << "mValue";
          line << " " << op << " rhs";
          if (!isForIntegral)
            line << ".mValue";
          line << ";";
        }
        if (!returnResult)
          opMethod.mBody.Add("return *this;");
      }
    }
  }
}
