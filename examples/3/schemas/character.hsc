mixin Object {
    u32 id = -1;
}

enum Race {
    Human = 2;
    Elf;
    Dwarf;
    Hobbit;
}

// There are different weapon types
// Your characters can hold weapons which might have different properties. For example bows might have a range while
// you might want to define the sharpness of each sword. You can do all this with variants.
enum WeaponType {
    Sword;
    Bow;
}

// Here we're defining that this struct is for the WeaponType variant, specifically for the Sword entry.
@variant(enum=WeaponType, entry=Sword)
struct WeaponTypeSword {
    float sharpness;
    bool isShortSword;
}

@variant(enum=WeaponType, entry=Bow)
struct WeaponTypeBow {
    u32 range;
    string material;
}

struct WeaponInInventory {
    WeaponType type;
    // And here we define the actual variant. It can be a sword or a bow.
    @variant(typeField=type)
    variant weapon;
}

struct Character : Object {
    string name;
    Race race;
    // holgen supports vectors, deques, maps and sets (and their unordered counterparts).
    vector<WeaponInInventory> weapons;
}