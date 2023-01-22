#include <array>
#include <string>

#include "TestHelper.hpp"
#include "dictionary/dictionary.hpp"

namespace TestDictionary
{

using Dict = Dictionary<std::string, int>;

int comparer(const std::string &str1, const std::string &str2)
{
  return str1.compare(str2);
}

std::shared_ptr<Dictionary<std::string, int>> generateDict()
{
  auto dict = std::shared_ptr<Dictionary<std::string, int>>(new Dict(
      comparer, {{"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}}));
  return dict;
}
}  // namespace TestDictionary


TEST(Dictionary, Constuctor)
{
  using namespace TestDictionary;
  auto dict = generateDict();

  std::array<std::string, 5> expectKeys = {"one", "two", "three", "four", "five"};
  std::array<int, 5> expectValues = {1, 2, 3, 4, 5};

  doesDictHave(dict.get(), expectKeys, expectValues);
}

TEST(Dictionary, Add)
{
  using namespace TestDictionary;
  auto dict = generateDict();

  ASSERT_TRUE(dict->Add("six", 6));
  ASSERT_FALSE(dict->Add("six", 7));


  std::array<std::string, 6> expectKeys = {"one", "two", "three", "four", "five", "six"};
  std::array<int, 6> expectValues = {1, 2, 3, 4, 5, 6};

  doesDictHave(dict.get(), expectKeys, expectValues);
}

TEST(Dictionary, CopyAndRemove)
{
  using namespace TestDictionary;
  auto dict1 = generateDict();
  auto dict2 = dict1->Copy();

  EXPECT_EQ(dict1->Remove("five").GetValue(), 5);
  EXPECT_TRUE(dict1->Remove("six").IsNull());

  std::array<std::string, 4> expectKeys1 = {"one", "two", "three", "four"};
  std::array<int, 4> expectValues1 = {1, 2, 3, 4};

  std::array<std::string, 5> expectKeys2 = {"one", "two", "three", "four", "five"};
  std::array<int, 5> expectValues2 = {1, 2, 3, 4, 5};

  doesDictHave(dict1.get(), expectKeys1, expectValues1);
  doesDictHave(dict2.get(), expectKeys2, expectValues2);
}

TEST(Dictionary, ForEach)
{
  using namespace TestDictionary;
  auto dict = generateDict();

  std::array<std::string, 5> expectKeys = {"five", "four", "one", "three", "two"};
  std::array<int, 5> expectValues = {5, 4, 1, 3, 2};

  dict->ForEach(
      [expectKeys, expectValues](const std::string &key, const int &value)
      {
        static int i = 0;
        ASSERT_EQ(key, expectKeys[i]);
        ASSERT_EQ(value, expectValues[i]);
        i++;
      });
}
