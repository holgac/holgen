mixin Object {
    // We don't want to read ids from json, so let's annotate as such.
    @noJson
    u32 id = -1;
    // You can define custom functions for your data structures. By default they're implemented in c++.
    // If you have c++ functions, a separate directory will be generated that will have partially generated code.
    // You can fill up the spaces between the markers. Check out the output/src directory.
    // The generator will keep your custom code if you regenerate.
    func Initialize();
}

@comment('enums can have comments too')
enum Race {
    Human = 2;
    @comment('enum entries can have comments too')
    Elf;
    Dwarf;
    Hobbit;
}

struct Character : Object {
    string name;
    Race race;
    Armor armor;
    Weapon weapon;
}

struct Armor : Object {
    // We can use custom setters that do some validation, and getters that compute extra stuff
    @field(set=custom)
    s8 armorClass;
}

struct Weapon : Object {
    @field(set=none)
    u8 damageMin;
    @field(set=none)
    u8 damageMax;
    // If we want to set multiple values together to ensure they're consistent, we can disable their setters and write
    // a custom function.
    // If we don't want this called from lua,
    @noLua
    func SetDamage(u8 min, u8 max);

    // here's a function returning a uint8_t:
    func GetDamage() -> u8;
    // You can also define functions in lua:
    @luaFunc
    func GetAverageDamage() -> u8;
}