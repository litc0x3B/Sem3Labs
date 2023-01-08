#pragma once
#include <array>
#include <memory>

#include "TestHelper.hpp"
#include "dictionary/indexing.hpp"

// NOLINTBEGIN(misc-definitions-in-headers)
namespace TestIndexing
{
struct Person
{
  std::string name;
  int age;
  int height;

  bool operator==(const Person &person) const { return this->name == person.name; }
};

struct PersonKey
{
  std::string name;
  int age;
};

PersonKey getKey(const Person &person)
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

int compPeopleByName(const Person &a, const Person &b) {
  return compNames(getKey(a), getKey(b));
}

Person dmitry = {"Dmitry", 19, 173};
Person sasha = {"Sasha", 20, 170};
Person sasha2 = {"Sasha", 30, 170};
Person cirno = {"Cirno", 80, 9};  // baka

std::array<Person, 4> people = {sasha, cirno, sasha2, dmitry};

AttributeInfo<PersonKey> age("age", compAges);
AttributeInfo<PersonKey> name("name", compNames);
AttributesInfo<PersonKey, Person> attrs({age, name}, getKey);

IndexedDict<PersonKey, Person> generateTestDict()
{
  IndexedDict<PersonKey, Person> dict(attrs, people);
  return dict;
}

template <unsigned long size>
std::array<PersonKey, size> getKeys(std::array<Person, size> people)
{
  std::array<PersonKey, size> keys;
  for (int i = 0; i < size; i++)
  {
    keys.at(i) = getKey(people.at(i));
  }

  return keys;
}
}  // namespace TestIndexing

TEST(Indexing, Add)
{
  using namespace TestIndexing;
  
  auto dict = generateTestDict();
  doesDictHave(&dict, getKeys(people), people);
}

TEST(Indexing, Get)
{
  using namespace TestIndexing;

  auto dict = generateTestDict();

  ASSERT_TRUE(dict.Get({"Cirno", 77}).IsNull());
  EXPECT_EQ(dict.Get(getKey(sasha)).GetValue(), sasha);
}

TEST(Indexing, Remove)
{
  using namespace TestIndexing;

  auto dict = generateTestDict();
  auto removedValue1 = dict.Remove(getKey(sasha));
  auto removedValue2 = dict.Remove({"amogus", 10});


  ASSERT_EQ(removedValue1.GetValue(), sasha);
  ASSERT_TRUE(removedValue2.IsNull());

  std::array<Person, 3> expect = {cirno, dmitry, sasha2};
  doesDictHave(&dict, getKeys(expect), expect);
}

TEST(Indexing, FromSet)
{
  using namespace TestIndexing;

  auto set = std::make_shared<Set<Person>>(compPeopleByName);
  set->Add(sasha);
  set->Add(cirno);
  set->Add(dmitry);
  IndexedDict<PersonKey, Person> dict(attrs, set);

  std::array<Person, 3> expect = {sasha, cirno, dmitry};
  doesDictHave(&dict, getKeys(expect), expect);
}

TEST(Indexing, Constructor)
{
  using namespace TestIndexing;
  auto dict =  generateTestDict();

  auto dict2(dict);
}

TEST(Indexing, GetInRange) 
{
  using namespace TestIndexing;

  auto dict = generateTestDict();
  Person oleg = dmitry;
  Person sonya = dmitry;

  sonya.name = "Sonya";
  oleg.name = "Oleg";


  dict.Add(sonya);
  dict.Add(oleg);

  std::array<Person, 3> expect = {oleg, sonya, sasha};

  auto newDict = dict.GetInRange({"O", 19}, {"Sz", 20});

  // newDict->ForEach([](const Person &person){std::cout << person.name << " " << person.age << std::endl;});
  doesDictHave(newDict.get(), getKeys(expect), expect);
}
//NOLINTEND(misc-definitions-in-headers)

