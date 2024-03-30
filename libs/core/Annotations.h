#pragma once


namespace holgen {
  // TODO: properly define what managed, datamanager, id and container annotations do.
  // currently things are mixed up a bit.
  // This kinda serves as documentation
  class Annotations {
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
     * Methods like AddElem, GetElem, GetElemCount, DeleteElem are added and exposed to lua.
     * Be careful using this with a vector if the underlying data type is not managed, the
     * object addresses will change when the vector resizes.
     *
     * TODO: For data managers, implement locking const containers
     * If it's OK for Lua to modify the container at startup (i.e. when loading a mod where iterator
     * invalidation is no issue) but we want to pass pointers around at runtime, locking is required.
     *
     * Example:
     * @container(elemName=country)
     * vector<Country> countries;
     */
    inline static const std::string Container = "container";
    inline static const std::string Container_ElemName = "elemName";
    inline static const std::string Container_Const = "const";

    /**
     * Indicates that the struct is managed by the given DataManager.
     * It adds a static Class::Get function * that uses GlobalPointer<DataManager> to fetch the object.
     * Lua instances and Refs use an index (determined by @id field) instead of pointer.
     *
     * Example:
     * @managed(by=Country, field=people)
     * struct Person {
     *  @id
     *  u32 id;
     * }
     *
     * struct Country {
     *  @container(elemName=person)
     *  vector<Person> people;
     * }
     */
    inline static const std::string Managed = "managed";
    inline static const std::string Managed_By = "by";
    inline static const std::string Managed_Field = "field";

    /**
     * Defines the field as an id. Determines whether Refs are raw pointers or indices.
     *
     * Can only exist once per struct. The field needs to be an integral type.
     * TODO: default = -1
     * TODO: strings should also work
     *
     * Example:
     * @id()
     * u32 id;
     */
    inline static const std::string Id = "id";

    /**
     * Defines an extra unique index for a container field by creating a map<indexedFieldType, idFieldType> field.
     * TODO: how to prevent index field modifications? Can remove setter method for lua only. trust cpp?
     * Maybe the setter of the indexed field can notify data manager? If so, use friend and make it private.
     * TODO: should just use size_t if no id field is specified
     *
     * Parameters:
     *  On: Field to index on (determines map's key)
     *  Using: Map type to use (defaults to map, can use unordered_map etc.)
     *  ForConverter: Use the index map for the given converter. Works only with data managers.
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
    /**
     * Marks the function below as a custom function implemented in lua
     * and callable from cpp and lua. table attribute determines the global lua table
     * used to store and reference the function
     */
    inline static const std::string LuaFunc = "luaFunc";
    inline static const std::string LuaFuncTable = "table";

    /**
     * Marks the function below as a custom function implemented in cpp
     * and callable from cpp and lua.
     */
    inline static const std::string CppFunc = "cppFunc";
  };
}
