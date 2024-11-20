#include <string>
#include <rapidjson/document.h>
#include "3/output/gen/Character.h"
#include "3/output/gen/Converter.h"
#include "3/output/gen/LuaHelper.h"
#include "LuaContext.h"

using namespace ex3_schemas;
using namespace holgen::usages;

namespace std {
template <>
struct formatter<ex3_schemas::WeaponTypeBow> : public formatter<string> {
public:
  template <typename FormatContext>
  auto format(const ex3_schemas::WeaponTypeBow &obj, FormatContext &ctx) const {
    return format_to(ctx.out(), "A {} bow with {} range", obj.GetMaterial(), obj.GetRange());
  }
};

template <>
struct formatter<ex3_schemas::WeaponTypeSword> : public formatter<string> {
public:
  template <typename FormatContext>
  auto format(const ex3_schemas::WeaponTypeSword &obj, FormatContext &ctx) const {
    return format_to(ctx.out(), "A {} sword with {} sharpness",
                     obj.GetIsShortSword() ? "short" : "long", obj.GetSharpness());
  }
};

template <>
struct formatter<ex3_schemas::WeaponInInventory> : public formatter<string> {
public:
  template <typename FormatContext>
  auto format(const ex3_schemas::WeaponInInventory &obj, FormatContext &ctx) const {
    switch (obj.GetType().GetValue()) {
    case WeaponType::Bow:
      return format_to(ctx.out(), "{}", *obj.GetWeaponAsWeaponTypeBow());
    case WeaponType::Sword:
      return format_to(ctx.out(), "{}", *obj.GetWeaponAsWeaponTypeSword());
    }
    return format_to(ctx.out(), "Invalid weapon");
  }
};
}

void example3() {
  const char *data = R"R(
{
  "name": "Frodo",
  "race": "Hobbit",
  "weapons": [
    {
      "type": "Bow",
      "weapon": {
        "range": 15,
        "material": "cedar"
      }
    },
    {
      "type": "Sword",
      "weapon": {
        "sharpness": 4.2,
        "isShortSword": false
      }
    }
  ]
}
)R";
  // Currently it's not possible to populate data from Lua, but it'll eventually be implemented
  rapidjson::Document doc;
  doc.Parse(data);
  Character c1;

  LuaContext ctx(LuaContext::Create);
  LuaHelper::CreateMetatables(ctx);
  // Let's create the damage getter function that we referenced in json
  ctx.Execute(
      "AverageDamageGetter = function(weapon) return (weapon.damageMin + weapon.damageMax) / 2 end");
  c1.ParseJson(doc, {}, ctx);
  c1.PushGlobalToLua(ctx, "frodo");

  std::cout << c1.GetName() << " has " << c1.GetWeapons().size() << " weapons:" << std::endl;
  for (auto &weapon: c1.GetWeapons()) {
    std::cout << std::format("{}", weapon) << std::endl;
  }

  std::cout << "You can access the variant fields in lua too! The first weapon (bow) is made of " <<
      ctx.ExecuteReturn("frodo.weapons[0].weapon.material") << std::endl;
}
