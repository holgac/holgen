struct Race {
    u32 id;
    string name;
    vector<string> hairColors;
    map<string, vector<string>> names;
}

struct Person {
    @jsonConvert(from=string, using=raceNameToId)
    u32 race;
    @jsonConvert(from=string, using=countryNameToId)
    u32 currentCountry;
    @jsonConvert(from=string, using=cityNameToId)
    u32 currentCity;
    @jsonConvert(from=string, using=countryNameToId)
    u32 homeCountry;
    @jsonConvert(from=string, using=cityNameToId)
    u32 placeOfBirth;
}

struct RaceId {
    @jsonConvert(from=string, using=raceNameToId)
    u32 id;
}

struct Country {
    Person leader;
    vector<Person> citizens;
    @jsonConvert(from=map<string, u32>, using=raceU32Map)
    map<u32, u32> population;
}