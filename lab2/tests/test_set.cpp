#include <gtest/gtest.h>
#include <memory>
#include <string>
#include "set.hpp"


int ConvertToInt(const std::string& str)
{
    return stoi(str);
}

TEST(SetTest, AddFromStr)
{
    auto set = std::make_shared<Set<int>>();
    set->AddFromStr(ConvertToInt, "1 2 3");
    EXPECT_EQ(set->ToStr(), "1 2 3");
}

TEST(SetTest, Union)
{
    auto set1 = std::make_shared<Set<int>>();
    set1->AddFromStr(ConvertToInt, "1 2 3 4");

    auto set2 = std::make_shared<Set<int>>();
    set2->AddFromStr(ConvertToInt, "3 4 5 6");

    EXPECT_EQ(set1->Union(set2)->ToStr(), "1 2 3 4 5 6");
}

TEST(SetTest, Intersection)
{
    auto set1 = std::make_shared<Set<int>>();
    set1->AddFromStr(ConvertToInt, "1 2 3 4");

    auto set2 = std::make_shared<Set<int>>();
    set2->AddFromStr(ConvertToInt, "3 4 5 6");

    EXPECT_EQ(set1->Intersection(set2)->ToStr(), "3 4");
}

TEST(SetTest, Difference)
{
    auto set1 = std::make_shared<Set<int>>();
    set1->AddFromStr(ConvertToInt, "1 2 3 4");

    auto set2 = std::make_shared<Set<int>>();
    set2->AddFromStr(ConvertToInt, "3 4 5 6");

    EXPECT_EQ(set1->Difference(set2)->ToStr(), "1 2");
}

TEST(SetTest, Has)
{
    Set<int> set1(ConvertToInt, "1 2 3 4");
    Set<int> set2(ConvertToInt, "2 3");
    Set<int> set3(ConvertToInt, "2 5");

    EXPECT_TRUE(set1.Has(set2));
    EXPECT_FALSE(set1.Has(set3));
}

TEST(SetTest, IsEqual)
{
    auto set1 = std::make_shared<Set<int>>();
    set1->AddFromStr(ConvertToInt, "1 2 3 4");

    auto set2 = std::make_shared<Set<int>>();
    set2->AddFromStr(ConvertToInt, "1 2 3 4");

    auto set3 = std::make_shared<Set<int>>();
    set3->AddFromStr(ConvertToInt, "1 2 3 5");

    auto set4 = std::make_shared<Set<int>>();
    set4->AddFromStr(ConvertToInt, "2 5");

    EXPECT_TRUE(set1->IsEqualTo(set2));
    EXPECT_FALSE(set1->IsEqualTo(set3));
    EXPECT_FALSE(set1->IsEqualTo(set4));
}
