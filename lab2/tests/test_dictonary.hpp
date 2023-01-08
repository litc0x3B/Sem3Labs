#pragma once
#include "TestHelper.hpp"
#include "dictionary/dictionary.hpp"

namespace TestDictionary
{

using Dict = Dictionary<std::string, int>;

int comparer(const std::string &str1, const std::string &str2) { return str1.compare(str2); }

std::shared_ptr<Dictionary<std::string, int>> generateDict()
{
  auto dict = std::shared_ptr<Dictionary<std::string, int>>(new Dict(comparer,
                                                       {{"one", 1},
                                                           {"two", 2},
                                                           {"three", 3},
                                                           {"four", 4},
                                                           {"five", 5}}));
  return dict;
}
}  // namespace TestDictionary

TEST(Dictionary, Copy)
{
  using namespace TestDictionary;
  generateDict();
}
