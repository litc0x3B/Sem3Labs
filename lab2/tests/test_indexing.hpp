#include "TestHelper.hpp"
#include "indexing.hpp"

struct Person
{
  std::string name;
  int age;
  int height;
};

struct PersonKey
{
  int age;
  std::string name;
};

PersonKey getPersonKey(const Person &person)
{
  PersonKey personKey;
  personKey.age = person.age;
  personKey.name = person.name;
  return personKey;
}

int compNames(const PersonKey &key1, const PersonKey &key2)
{
  std::string str1 = key1.name;
  std::string str2 = key2.name;

  if (str1 > str2) return 1;
  if (str1 < str2) return -1;
  return 0;
}

int compAges(const PersonKey &key1, const PersonKey &key2)
{
  int a = key1.age;
  int b = key2.age;

  if (a > b) return 1;
  if (a < b) return -1;
  return 0;
}

TEST(Indexing, Add)
{
  AttributeInfo<PersonKey> age("age", compAges);
  AttributeInfo<PersonKey> name("name", compNames);
  auto attriutes = AttributesInfo<PersonKey, Person>({age, name}, getPersonKey);
  // IndexedDict<PersonKey, Person> dict(
  //     AttributesInfo<PersonKey, Person>({age, name}, getPersonKey));

  // Person sasha = {"Sasha", 20, 170};
  // dict.Add(sasha);
  // dict.Add({"Dmitry", 19, 165});
  // EXPECT_EQ(dict.Get(getPersonKey(sasha)), sasha);
}
