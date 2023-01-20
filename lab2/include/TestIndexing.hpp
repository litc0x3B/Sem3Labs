#pragma once
#include <string>

#include "dictionary/indexing.hpp"

namespace TestIndexing
{
struct Person
{
  std::string name;
  int age;
  int height;

  bool operator==(const Person &person) const
  {
    return this->name == person.name;
  }

   friend std::ostream &operator<<(std::ostream &os, const Person &person)
   {
    os << "{\"" << person.name << "\", " << person.age << ", "
              << person.height << "}";
    return os;
   }
};

struct `~PersonKey
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

int compPeopleByName(const Person &a, const Person &b)
{
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