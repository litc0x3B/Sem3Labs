#include <array>
#include <memory>

#include "TestHelper.hpp"
#include "dictionary/indexing.hpp"
#include "TestIndexing.hpp"

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
  auto dict = generateTestDict();

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

  // newDict->ForEach([](const Person &person){std::cout <<
  // person.testName << " " << person.age << std::endl;});
  doesDictHave(newDict.get(), getKeys(expect), expect);
}
