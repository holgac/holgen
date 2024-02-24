@containedBy(GameData)
struct Boot {
    // assigned by container
    @id()
    u32 id;
    string name;
    // TODO: enums and auto conversion from string to integral
    string color;
}

@containedBy(GameData)
struct Armor {
    @id()
    u32 id;
    string name;
    string alternativeName;
    s8 armorClass;
}

@containedBy(GameData)
struct Character {
    // id should be an integral field
    @id()
    u32 id;

    string name;

    // adds GetBoot() that uses GameData singleton
    @reference(Boot)
    @jsonConvert(from=string, using=bootNameToId)
    u32 boot;

    @jsonConvert(from=string, using=armorNameToId)
    u32 armor;
}

// needs to be a singleton for lua to work
@dataManager()
struct GameData {
    @index(on=name, using=unordered_map, forConverter=bootNameToId)
    @container(elemName=boot)
    vector<Boot> boots;

    @container(elemName=armor)
    @index(on=name, forConverter=armorNameToId)
    @index(on=alternativeName)
    vector<Armor> armors;

    // @index(on=name, forConverter=characterNameToId)
    @index(on=name)
    @container(elemName=character)
    vector<Character> characters;
}