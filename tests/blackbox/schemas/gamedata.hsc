@managed(by=GameData, field=boots)
struct Boot {
    // assigned by container
    @id
    u32 id;
    string name;
    // TODO: enums and auto conversion from string to integral
    string color;
}

@managed(by=GameData, field=armors)
struct Armor {
    @id
    u32 id;
    string name;
    string alternativeName;
    s8 armorClass;
}

@managed(by=GameData, field=characters)
struct Character {
    // id should be an integral field
    @id
    u32 id;

    string name;

    // adds GetBoot() that uses GameData singleton
    @jsonConvert(from=string, using=bootNameToId)
    Ref<Boot> boot;

    @jsonConvert(from=string, using=armorNameToId)
    Ref<Armor> armor;
}

@dataManager
struct GameData {
    @index(on=name, using=unordered_map, forConverter=bootNameToId)
    @container(elemName=boot, const)
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