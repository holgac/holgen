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
    // using=map by default
    // creates an extra map<fieldType, idType>, adds GetBootFromName(string) function
    // only applies to @container
    @index(on=name, using=unordered_map)
    // containers can only be indexed containers. After pushing, assigns id.
    // adds AddBoot, GetBoot(u32) functions
    // There should be one container per type
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