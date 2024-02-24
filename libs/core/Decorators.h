#pragma once


namespace holgen {
  // This kinda serves as documentation
  class Decorators {
  public:
    /*
     * When reading from json, convert the field data using a converter.
     * Only for fields.
     *
     * Example:
     *  @jsonConvert(from=string, using=countryConverter) // func(string) -> u8 function added to Converter class
     *  u8 country;
     *  @jsonConvert(from=string, using=cityConverter) // another func added
     *  u8 city;
     *  @jsonConvert(sameAs=country) // first func reused
     *  u8 previousCountry;
     */
    inline static const std::string JsonConvert = "jsonConvert";
    inline static const std::string JsonConvert_From = "from";
    inline static const std::string JsonConvert_Using = "using";

    /*
     * Disables lua integration for the specified data type.
     *
     * Example:
     * @noLua
     * struct Something { ... }
     *
     * struct Something {
     *  @noLua()
     *  i8 field;
     * }
     */
    inline static const std::string NoLua = "noLua";

    /*
     * Disables json integration for the specified data type.
     *
     * Example:
     * @noJson
     * struct Something { ... }
     *
     * struct Something {
     *  @noJson()
     *  i8 field;
     * }
     */
    inline static const std::string NoJson = "noJson";

    /*
     * Indicates that the field is a container that should be exposed.
     *
     * Example:
     * @container(elemName=country)
     * vector<Country> countries;
     */
    inline static const std::string Container = "container";
    inline static const std::string Container_ElemName = "elemName";

    /**
     * Defines the field as an id. Can only exist once per struct.
     * The field needs to be an integral type.
     *
     * Example:
     * @id()
     * u32 id;
     */
    inline static const std::string Id = "id";

    /**
     * Defines an extra unique index for a container field by creating a map<indexedFieldType, idFieldType> field.
     * TODO: how to prevent index field modifications? Can remove setter method for lua only. trust cpp?
     *
     * Parameters:
     *  On: Field to index on (determines map's key)
     *  Using: Map type to use (defaults to map, can use unordered_map etc.)
     *
     * Example:
     * struct Person {
     * @id()
     * u32 id;
     *
     * u32 ssn;
     * }
     * struct House {
     *   @container(elemName=person)
     *   @index(on=ssn,using=unordered_map)
     *   vector<Person> people;
     * }
     *
     * Data managers can use an index as a converter for the data they load
     * struct House {
     *   @container(elemName=person)
     *   @index(on=ssn,forConverter=ssnToPerson)
     *   vector<Person> people;
     * }
     */
    inline static const std::string Index = "index";
    inline static const std::string Index_On = "on";
    inline static const std::string Index_Using = "using";
    inline static const std::string Index_ForConverter = "forConverter";

    /**
     * Marks the struct as a data manager that has a special method for loading the types from the FS it contains.
     *
     * All fields marked with @container should be indexed containers with a field marked with @id decorator.
     *
     * Example:
     * struct Plant {
     *  @id()
     *  u32 id;
     *  string name;
     * }
     * struct Animal {
     *  @jsonConvert(from=string, using=plantNameToPlant)
     *  Plant favPlant;
     * }
     * @dataManager()
     * struct DataManager {
     *  @index(on=name, forConverter=plantNameToPlant)
     *  @container(elementName=plant)
     *  vector<Plant> plants;
     *  @container(elementName=animal)
     *  vector<Animal> animals;
     * }
     */
    inline static const std::string DataManager = "dataManager";
  };
}
