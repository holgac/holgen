@managed(by=TestJsonTagManager, field=tags)
struct TestJsonTag {
  @id
  u64 id;
  string name;
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
