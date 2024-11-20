#include <rapidjson/document.h>
#include "2/output/gen/Character.h"
#include "2/output/gen/Armor.h"
#include "2/output/gen/Converter.h"
#include "2/output/gen/LuaHelper.h"
#include "LuaContext.h"

using namespace ex2_schemas;
using namespace holgen::usages;

void example2() {
  const char *data = R"R(
{
  "name": "Frodo",
  "race": "Hobbit",
  "armor": {
    "armorClass": -2
  },
  "weapon": {
    "damageMin":10,
    "damageMax":20,
    "GetAverageDamage": "AverageDamageGetter"
  }
}
)R";
  // Currently it's not possible to populate data from Lua, but it'll eventually be implemented
  rapidjson::Document doc;
  doc.Parse(data);
  Character c1;
  //
  LuaContext ctx(LuaContext::Create);
  LuaHelper::CreateMetatables(ctx);
  // Let's create the damage getter function that we referenced in json
  ctx.Execute(
      "AverageDamageGetter = function(weapon) return (weapon.damageMin + weapon.damageMax) / 2 end");
  c1.ParseJson(doc, {}, ctx);
  c1.Initialize();
  c1.PushGlobalToLua(ctx, "frodo");

  std::cout << c1.GetName() << " does " << int(c1.GetWeapon().GetDamage()) << " damage!" <<
      std::endl;
  std::cout << c1.GetName() << " does " << int(c1.GetWeapon().GetAverageDamage(ctx)) << " damage!"
      << std::endl;
  std::cout << ctx.ExecuteReturn("frodo.name .. ' does ' .. frodo.weapon:GetDamage() .. ' damage!'")
      << std::endl;
}
