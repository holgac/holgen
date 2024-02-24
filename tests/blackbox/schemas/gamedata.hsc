@containedBy(GameData)
struct Boot {
    // assigned by container
    @id()
    u32 id;
    string name;
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

    // uses u32 (Boot.id) instead
    // what's the benefit? less duplicated code
    @reference()
    // @jsonConvert(from=string, using=bootNameToBootId)
    Boot boot;
    Armor armor;
}

// generate a single loader that scans the filesystem for jsons.
// it should populate converters
// Needs to be aware of dependency graph (Character has Boot/Armor converters, so it should be loaded after)
// needs to be a singleton for lua to work
@dataManager()
struct GameData {
    @index(on=name, using=unordered_map)
    @container(elemName=boot)
    vector<Boot> boots;

    @container(elemName=armor)
    @index(on=name)
    @index(on=alternativeName)
    vector<Armor> armors;

    @index(on=name)
    @container(elemName=character)
    vector<Character> characters;
}