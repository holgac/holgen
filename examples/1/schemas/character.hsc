mixin Object {
    // We want all objects to have the same id field and type, so we're defining it as part of this mixin.
    // The comment above is a comment for the schema file reader. The comment below will appear in the codebase.
    @comment('This is the ID of the object. Data managers can assign these, check out the relevant tutorial')
    // uXX types are unsigned integrals. This one is 32 bits.
    // It's possible to specify default values for fields. Here we're using -1 so that we know it's uninitialized.
    u32 id = -1;
}

// Enum entries get assigned a unique value automatically, but they can also have default values
enum Race {
    Human = 2;
    Elf;
    Dwarf;
    Hobbit;
}

struct Character : Object {
    // This will be a std::string
    string name;
    /*
        Enums can be parsed as their entry name or the value in json. They're exported using their value to lua, but
        the name can be accessed through the exposed enum table: If "char.race" is 2,  Race[char.race] will be 'Human'
    */
    Race race;
    // A class can be used before it is defined. Here we define an instance member. In DataManager example we'll embed an object reference instead.
    Armor armor;
}

struct Armor : Object {
    // sXX types are signed. This one is 8 bits.
    s8 armorClass;
}