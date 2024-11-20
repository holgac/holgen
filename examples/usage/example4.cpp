#include <string>
#include "4/output/gen/DataManager.h"
#include "4/output/gen/Character.h"
#include "4/output/gen/Converter.h"
#include "4/output/gen/GlobalPointer.h"
#include "4/output/gen/LuaHelper.h"
#include "LuaContext.h"

using namespace ex4_schemas;
using namespace holgen::usages;

void example4() {
  if (!std::filesystem::is_directory("data/4")) {
    std::cerr << "Run this example from the project directory!";
    return;
  }
  DataManager manager;
  LuaContext ctx(LuaContext::Create);
  LuaHelper::CreateMetatables(ctx);
  // load everything recursively
  manager.ParseFiles("data/4", "", {}, ctx);
  // set the singleton
  GlobalPointer<DataManager>::SetInstance(&manager);
  std::cout << std::format("character #{}: {}. They're wearing {} and wielding {}\n",
                           manager.GetCharacterFromName("knight")->GetId(),
                           manager.GetCharacterFromName("knight")->GetName(),
                           manager.GetCharacterFromName("knight")->GetArmor().GetArmor()->GetName(),
                           manager.GetCharacterFromName("knight")->GetWeapon()->GetName());

  // let's add an armor programmatically
  {
    Armor armor;
    armor.SetName("leather jacket");
    armor.SetArmorClass(5);
    manager.AddArmor(std::move(armor));
  }
  // Since the data structures are managed, you can use ::Get and ::GetFromName functions
  std::cout << std::format("New armor {} has id {}\n",
                           Armor::GetFromName("leather jacket")->GetName(),
                           Armor::GetFromName("leather jacket")->GetId());
  manager.PushToLua(ctx);
  manager.PushGlobalToLua(ctx, "manager");
  // Do the same from lua:
  std::cout << std::format("Leather jacket ac is {}\n",
                           ctx.ExecuteReturn(
                               "manager:GetArmorFromName('leather jacket').armorClass"));
}
