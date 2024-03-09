#include "EnumPlugin.h"
#include "core/St.h"

namespace holgen {
  void EnumPlugin::Run() {

    for (auto &generatedClass: mProject.mClasses) {
      if (generatedClass.mEnum == nullptr)
        continue;
      auto &enumDefinition = *generatedClass.mEnum;

      {
        auto &typdef = generatedClass.mTypedefs.emplace_back();
        typdef.mTargetType = "UnderlyingType";
        typdef.mSourceType.mName = "int64_t";
      }
      {
        ClassField &valueField = generatedClass.mFields.emplace_back();
        valueField.mName = "mValue";
        valueField.mType.mName = "UnderlyingType";
      }
      {
        ClassConstructor &ctor = generatedClass.mConstructors.emplace_back();
        ctor.mExplicitness = Explicitness::Explicit;
        ClassMethodArgument &value = ctor.mArguments.emplace_back();
        value.mName = "value";
        value.mType.mName = "UnderlyingType";
        value.mDefaultValue = "Invalid";
        ClassConstructorInitializer &initializer = ctor.mInitializerList.emplace_back();
        initializer.mDestination = "mValue";
        initializer.mValue = "value";
      }

      for (auto &entry: enumDefinition.mEntries) {
        ClassField &valueField = generatedClass.mFields.emplace_back();
        valueField.mType.mConstness = Constness::Const;
        valueField.mType.mConstexprness = Constexprness::Constexpr;
        valueField.mStaticness = Staticness::Static;
        valueField.mType.mName = "UnderlyingType";
        valueField.mName = entry.mName + "Value";
        valueField.mDefaultValue = entry.mValue;
        valueField.mVisibility = Visibility::Public;

        ClassField &entryField = generatedClass.mFields.emplace_back();
        entryField.mType.mConstness = Constness::Const;
        entryField.mStaticness = Staticness::Static;
        entryField.mType.mName = generatedClass.mName;
        entryField.mName = entry.mName;
        entryField.mDefaultConstructorArguments.push_back(entry.mValue);
        entryField.mVisibility = Visibility::Public;
      }

      {
        ClassField &invalidEntry = generatedClass.mFields.emplace_back();
        invalidEntry.mType.mConstness = Constness::Const;
        invalidEntry.mType.mConstexprness = Constexprness::Constexpr;
        invalidEntry.mStaticness = Staticness::Static;
        invalidEntry.mType.mName = "UnderlyingType";
        invalidEntry.mName = "Invalid";
        invalidEntry.mDefaultValue = enumDefinition.mInvalidValue;
        invalidEntry.mVisibility = Visibility::Public;
      }

      {
        ClassMethod &fromString = generatedClass.mMethods.emplace_back();
        fromString.mName = "FromString";
        fromString.mStaticness = Staticness::Static;
        fromString.mConstness = Constness::NotConst;
        fromString.mReturnType.mName = generatedClass.mName;
        ClassMethodArgument &strArg = fromString.mArguments.emplace_back();
        strArg.mType.mName = "std::string_view";
        strArg.mName = "str";
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
        ClassMethod &toString = generatedClass.mMethods.emplace_back();
        toString.mName = "ToString";
        toString.mConstness = Constness::Const;
        toString.mReturnType.mName = "std::string";
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
          ClassMethod &opMethod = generatedClass.mMethods.emplace_back();
          opMethod.mName = "operator " + op;
          if (returnResult)
            opMethod.mReturnType.mName = "bool";
          else {
            opMethod.mReturnType.mName = generatedClass.mName;
            opMethod.mReturnType.mType = PassByType::Reference;
          }
          auto &rhs = opMethod.mArguments.emplace_back();
          rhs.mName = "rhs";
          if (isForIntegral) {
            rhs.mType.mName = "UnderlyingType";
          } else {
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
}
