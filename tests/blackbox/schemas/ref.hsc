@managed(by=HumanManager, field=humans)
struct Human {
    @id
    u32 id;
    string name;
}

@dataManager
struct HumanManager {
    @container(elemName=human)
    @index(on=name)
    unordered_map<Ref<Human>, Human> humans;
}
