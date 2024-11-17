@managed(by=TestJsonTagManager, field=tags)
struct TestJsonTag {
  @id
  u64 id;
  string name;
  @func(constructor)
  func Construct(u64 id, string name);
}

@dataManager
struct TestJsonTagManager {
  @container(elemName=tag)
  @index(on=name)
  vector<TestJsonTag> tags;

  func GetOrInsert(string tag) -> Ref<TestJsonTag>;
}

struct TestJsonStructWithTags {
  @jsonConvert(elem, from=string, using=testJsonConvertTag)
  vector<Ref<TestJsonTag>> tags;
}

struct TestJsonStructMapWithConverters {
  @jsonConvert(elem, from=string, using=testJsonConvertStringToU32)
  map<string, u32> testMapConvertElem;

  @jsonConvert(key, from=string, using=testJsonConvertStringToU32)
  map<u32, string> testMapConvertKey;

  @jsonConvert(key, from=string, using=testJsonConvertStringToU32)
  @jsonConvert(elem, from=string, using=testJsonConvertStringToU32)
  map<u32, u32> testMapConvertKeyElem;
}

enum TestJsonEnum {
  Entry1 = 1;
  Entry2 = 2;
  Entry5 = 5;
}

struct TestJsonStructWithSingleTag {
  TestJsonTag tag;
}

struct TestJsonStructContainer {
  vector<TestJsonTag> tags;
  vector<TestJsonEnum> enums;
  map<string, TestJsonTag> tagMap;
}