#pragma once

namespace holgen {
// This kinda serves as documentation
class Annotations {
public:
  /**
   * These are possible values for method options
   */
  inline static const std::string MethodOption_Custom = "custom";
  inline static const std::string MethodOption_None = "none";
  inline static const std::string MethodOption_Private = "private";
  inline static const std::string MethodOption_Protected = "protected";

  /*
   * When reading from json, convert the field data using a converter.
   * Only for fields.
   *
   * Example:
   *  @jsonConvert(from=string, using=countryConverter) // func(string) -> u8 function added to
   *                                                    // Converter class u8 country;
   *  @jsonConvert(from=string, using=cityConverter) // another func added
   *  u8 city;
   *
   *  @jsonConvert(key, from=string, using=cityConverter)
   *  @jsonConvert(elem, from=string, using=countryConverter)
   *  map<u8, u8> cityToCountryMap;
   */
  inline static const std::string JsonConvert = "jsonConvert";
  inline static const std::string JsonConvert_From = "from";
  inline static const std::string JsonConvert_Using = "using";
  inline static const std::string JsonConvert_Key = "key";
  inline static const std::string JsonConvert_Elem = "elem";

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
   * Be careful using this with a vector if the underlying data type is not managed; the
   * object addresses will change when the vector resizes (affects lua handles)
   *
   * TODO: For data managers, implement locking const containers
   * If it's OK for Lua to modify the container at startup (i.e. when loading a mod where iterator
   * invalidation is no issue) but we want to pass pointers around at runtime, locking is required.
   *
   * Container methods can be customized, use the operation name as key (add, get, count, delete,
   * has) and a value of "custom" to define it as a custom function, "none" to remove it altogether,
   * or "private" to set its visibility.
   *
   * Example:
   * @container(elemName=country)
   * vector<Country> countries;
   * @container(elemName=country, add=custom, delete=none)
   * vector<Country> countries;
   */
  inline static const std::string Container = "container";
  inline static const std::string Container_ElemName = "elemName";
  inline static const std::string Container_Add = "add";
  inline static const std::string Container_Get = "get";
  inline static const std::string Container_Count = "count";
  inline static const std::string Container_Delete = "delete";
  inline static const std::string Container_Has = "has";

  /**
   * Indicates that the struct is managed by the given DataManager.
   * It adds a static Class::Get function that uses GlobalPointer<DataManager> to fetch the object.
   * Lua instances and Refs use an index (determined by @id field) instead of pointer.
   *
   * Requires an id field.
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
   * TODO: strings should also work
   *
   * Example:
   * @id()
   * u32 id;
   */
  inline static const std::string Id = "id";

  /**
   * Defines an extra unique index for a container field by creating a map<indexedFieldType,
   * idFieldType> field.
   * TODO: how to prevent index field modifications? Can remove setter method for lua only. trust
   * cpp? Maybe the setter of the indexed field can notify data manager? If so, use friend and make
   * it private.
   *
   * Parameters:
   *  On: Field to index on (determines map's key)
   *  Using: Map type to use (defaults to map, can use unordered_map etc.)
   *  ForConverter: Use the index map for the given converter. Works only with data managers.
   *
   * Example:
   * struct Person {
   * @id // not required for index
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
   * Marks the struct as a data manager that has a special method for loading the types from the FS
   * it contains.
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
 * Interfaces are classes fully implemented in C# that can be called from C++.
  */
 inline static const std::string Interface = "interface";

  /**
   * Marks the struct below as a function table where all functions are defined in lua and json
   * contains the name of the table. The optional table attribute defines where the function table
   * is stored.
   *
   * @luaFuncTable(sourceTable=AIScripts)
   * struct AIScript {
   *  func Think(Actor actor);
   * }
   * The struct above expects something like AIScripts.<table>.Think = function(AIScript, Actor)
   *
   * @luaFuncTable(static)
   * struct AIScript {
   *  func Think(Actor actor);
   * }
   * The struct above expects something like <table>.Think = function(Actor)
   */
  inline static const std::string LuaFuncTable = "luaFuncTable";
  inline static const std::string LuaFuncTable_SourceTable = "sourceTable";
  inline static const std::string LuaFuncTable_Static = "static";

  /**
   * Marks the function below as a custom function implemented in lua
   * and callable from cpp and lua. table attribute determines the global lua table
   * used to store and reference the function
   */
  inline static const std::string LuaFunc = "luaFunc";
  inline static const std::string LuaFunc_SourceTable = "sourceTable";
  inline static const std::string LuaFunc_Static = "static";

  /**
   * luadata related annotations.
   *
   * By default, any luadata field will be initialized in InitializeLua and deleted in
   * UninitializeLua.
   * @luaData(noInit) can be used to prevent this behaviour.
   *
   * If the luadata is shared across many instances, it's OK for the class to be destroyed without
   * setting it back to LUA_NOREF. @luaData(shared) can be used to remove the warning logging in
   * destructor.
   */
  inline static const std::string LuaData = "luadata";
  inline static const std::string LuaData_NoInit = "noInit";
  inline static const std::string LuaData_Shared = "shared";


  /**
   * Controls the function below. By default the functions are defined in cpp, but
   * @luaFunc can be used to change this behaviour.
   *
   * onDestroy: Called when the object is destroyed, use it to free unmanaged resources
   *            By default, onDestroy functions are protected, use "public" attribute if needed.
   *
   * const: Specifies whether the function is const
   *
   * static: Creates a static method that can be called without an instance.
   *
   * hash: Marks the function as the hash function used in std::hash
   *
   * toString: Uses the function in the std::formatter specialization
   *
   * constructor: Adds a custom constructor with the same arguments that calls the function. Implies
   *              staticness.
   *
   * noexcept: User defined functions without this attribute are not marked as noexcept.
   *
   * virtual: A user defined function that's marked as virtual.
   *
   * pureVirtual: A function with no definition that's marked as pure virtual.
   */
  inline static const std::string Func = "func";
  inline static const std::string Func_OnDestroy = "onDestroy";
  inline static const std::string Func_Const = "const";
  inline static const std::string Func_Private = "private";
  inline static const std::string Func_Protected = "protected";
  inline static const std::string Func_Public = "public";
  inline static const std::string Func_Static = "static";
  inline static const std::string Func_Hash = "hash";
  inline static const std::string Func_ToString = "toString";
  inline static const std::string Func_Constructor = "constructor";
  inline static const std::string Func_Noexcept = "noexcept";
  inline static const std::string Func_Virtual = "virtual";
  inline static const std::string Func_PureVirtual = "pureVirtual";

  /**
   * Adds a comment to a struct, field or function.
   *
   * @comment("This is a comment")
   * struct A {
   *   @comment("another comment", "second line")
   *   s32 a;
   *   @comment("func comment")
   *   @comment("another func comment")
   *   func f(s32 a);
   * }
   */
  inline static const std::string Comment = "comment";

  /**
   * For floating point fields, defines the epsilon value to use when comparing them in operator==.
   * Can also be defined for a struct to apply to all floating point fields, but the field
   * definition takes precedence.
   *
   * @epsilon('0.001')
   * struct A {
   *   @epsilon('0.000001')
   *   double f;
   * }
   */
  inline static const std::string Epsilon = "epsilon";

  /**
   * Variants are similar to std::variant. They're used as fields and their type is backed by an
   * enum. A struct with a variant field will have two fields - a type field (backed by the
   * specified enum) and a uint8_t array with size that can hold the largest variant struct.
   *
   * enum DataType {
   *  Type1;
   *  Type2;
   * }
   * struct DataHolder {
   *   @variant(typeField=dataType, enum=DataType)
   *   variant data;
   * }
   *
   * @variant(enum=DataType, entry=Type1)
   * struct DataType1 {
   *  u32 f1;
   * }
   * @variant(enum=DataType, entry=Type2)
   * struct DataType2 {
   *  float f1;
   * }
   */
  inline static const std::string Variant = "variant";
  inline static const std::string Variant_TypeField = "typeField";
  inline static const std::string Variant_Enum = "enum";
  inline static const std::string Variant_Entry = "entry";

  /**
   * Enum related annotations
   *
   * Initial enum value is EnumName::Invalid by default. This can be changed with default attribute:
   * enum SomeEnum {
   *  @enum(default)
   *  Entry1;
   * }
   *
   * By default, enums use the smallest unsigned integer type that can store the entries, but type
   * attribute can be used to override this behaviour.
   * @enum(type=u32)
   * enum SomeEnum {
   *  Entry1;
   * }
   */
  inline static const std::string Enum = "enum";
  inline static const std::string Enum_Default = "default";
  inline static const std::string Enum_Type = "type";

  /**
   * You can store primitive data and strings in enums.
   * @enumProperty(name=canTalk, type=bool)
   * @enumProperty(name=alternativeName, type=string, default=NOT_DEFINED)
   * enum Animal {
   *   @enumProperty(name=canTalk, value=true)
   *   @enumProperty(name=alternativeName, value=hooman)
   *   Human;
   *   @enumProperty(name=canTalk, value=false)
   *   @enumProperty(name=alternativeName, value=kitty)
   *   Cat;
   * }
   */
  inline static const std::string EnumProperty = "enumProperty";
  inline static const std::string EnumProperty_Name = "name";
  inline static const std::string EnumProperty_Default = "default";
  inline static const std::string EnumProperty_Type = "type";
  inline static const std::string EnumProperty_Value = "value";

  /**
   * Struct related annotations
   *
   * A struct can be defined nonCopyable which removes operator= and copy constructor
   * @struct(nonCopyable)
   * struct YouCanOnlyMoveThis {
   *    BigData field;
   * }
   */
  inline static const std::string Struct = "struct";
  inline static const std::string Struct_NonCopyable = "nonCopyable";


  /**
   * Field related annotations.
   *
   * A field can be const (not an actual const field, but it disables setter in cpp and lua).
   * @field(const)
   * u32 fieldWithNoSetter;
   *
   * In case of pointer backed refs, the pointer can be set as const with constPtr
   *
   * Field setters can be disabled, made private/protected, or custom
   * @field(set=private, set=custom)
   * u32 customPrivateSetter;
   * @field(set=none)
   * u32 noSetter;
   */
  inline static const std::string Field = "field";
  inline static const std::string Field_Const = "const";
  inline static const std::string Field_ConstPtr = "constPtr";
  inline static const std::string Field_Set = "set";
  inline static const std::string Field_Get = "get";
  inline static const std::string Field_OnCopy = "onCopy";
  inline static const std::string Field_OnCopy_Copy = "copy";
  inline static const std::string Field_OnCopy_Swap = "swap";
};
} // namespace holgen
