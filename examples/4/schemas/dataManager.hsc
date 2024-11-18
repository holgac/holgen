mixin NamedObject {
    @no(json)
    // Annotate this field as id. DataManager will use it to uniquely identify instances.
    @id
    u32 id = -1;
    // All our objects have unique names
    string name;
}

// The characters are managed by data manager - it's responsible for assigning IDs, storing them and handling references.
@managed(by=DataManager, field=characters)
struct Character : NamedObject {
    // A character might have a spouse/partner. Let's store a reference to it! This will store a pointer or an id depending on
    // the struct. Since ours has an ID and is managed, this will result in an id field.
    Ref<Character> partner;

    // When reading character weapons from json, we don't want to read ids of weapons. It's easier to use object names instead.
    // The annotation below tells that the data in json is a string, and it should be converted to an id using weaponNameToId converter.
    // This converter can be manually passed to the ParseJson method of the class.
    @jsonConvert(from=string, using=weaponNameToId)
    Ref<Weapon> weapon;
    // We want to track how much dirt appears in each character's armor, so let's embed a struct that stores it
    CharacterArmor armor;
}

struct CharacterArmor {
    float dirtAmount = 0;
    @jsonConvert(from=string, using=armorNameToId)
    Ref<Armor> armor;
}

@managed(by=DataManager, field=armors)
struct Armor : NamedObject {
    s8 armorClass;
}

@managed(by=DataManager, field=weapons)
struct Weapon : NamedObject {
    u8 damageMin;
    u8 damageMax;
}

// This deletes the default constructor and assignment operators that copy the class. It's optional, but I like
// specifying it so that I don't accidentally copy my data structures where I was intending to store a reference.
@struct(nonCopyable)
// Apart from managing resources, data managers also have a ParseFiles function that parses multiple json files under a directory.
@dataManager
struct DataManager {
    // Annotate the container field as a container so that we get extra functions like GetCharacter, AddCharacter, DeleteCharacter.
    @container(elemName=character)
    // We want an extra index by name so that we can reference it easily. The annotation below creates a GetCharacterFromName method
    // under our DataManager.
    @index(on=name)
    deque<Character> characters;

    @container(elemName=armor)
    // Apart from indexing on the name field, this tells the dataManager that armorNameToId converter uses this index. So
    // we don't need to explicitly define the converters for json!
    @index(on=name, forConverter=armorNameToId)
    deque<Armor> armors;

    @container(elemName=weapon)
    @index(on=name, forConverter=weaponNameToId)
    deque<Weapon> weapons;
}