#include <rapidjson/document.h>
#include "1/output/gen/Character.h"
#include "1/output/gen/Armor.h"
#include "1/output/gen/Converter.h"
#include "1/output/gen/LuaHelper.h"
#include "LuaContext.h"

using namespace ex1_schemas;
using namespace holgen::usages;

namespace {
Character GenerateCharacter() {
  Character c;
  c.SetName("Urist");
  c.SetRace(Race::Dwarf);
  c.SetId(0);
  c.GetArmor().SetId(0);
  c.GetArmor().SetArmorClass(-3);
  return c;
}
}

void example1() {
  // You can fill up the data in C++ of course
  auto c1 = GenerateCharacter();
  // or use a json string
  const char *data = R"R(
{
  "name": "Frodo",
  "race": "Hobbit",
  "armor": {
    "armorClass": -2
  }
}
)R";
  // Currently it's not possible to populate data from Lua, but it'll eventually be implemented
  rapidjson::Document doc;
  doc.Parse(data);
  Character c2;
  // The second parameter is a converter. It's not useful for this example, but it allows programmatically converting values from json
  // into more c++ friendly values before storing in the class. The second example will expand on this.
  // The third parameter is the lua context. We don't need lua when parsing json in this example, so we pass null.
  c2.ParseJson(doc, {}, nullptr);

  // And let's test things from lua! I'll use a basic context class I created
  LuaContext ctx(LuaContext::Create);
  // Before using Lua bindings, you need to create the required data like so:
  LuaHelper::CreateMetatables(ctx);
  c1.PushGlobalToLua(ctx, "urist");
  c2.PushGlobalToLua(ctx, "frodo");
  std::cout << ctx.ExecuteReturn("urist.name") << " and " << ctx.ExecuteReturn("frodo.name") <<
      " were exported to lua." << std::endl;


  std::cout << ctx.ExecuteReturn("urist.name .. ' is a ' .. Race[urist.race]") << std::endl;
  // You can set the values in lua too:
  std::cout << ctx.Execute("urist.armor.armorClass = 15");
  std::cout << "Urist's armor class is " << int(c1.GetArmor().GetArmorClass()) << std::endl;
}
