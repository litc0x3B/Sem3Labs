#pragma once
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

TEST(Dictionary, RemoveAndCopy)
{
  using namespace TestDictionary;
  auto dict = generateDict();

  std::array<std::string, 4> expectKeys = {"one", "two", "three", "four"};
  std::array<int, 4> expectValues = {1, 2, 3, 4};
  EXPECT_EQ(dict->Remove("five").GetValue(), 5);
  EXPECT_TRUE(dict->Remove("six").IsNull());

  doesDictHave(dict.get(), expectKeys, expectValues);

  auto dict2 = dict->Copy();
  doesDictHave(dict.get(), expectKeys, expectValues);
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
