struct DamageMultiplier {
    string when;
    float value;
}

struct Weapon {
    u32 damageMin;
    u32 damageMax;
    vector<DamageMultiplier> damageMultipliers;
    vector<string> modifiers;
    func GetAverageDamage() -> u32;
}