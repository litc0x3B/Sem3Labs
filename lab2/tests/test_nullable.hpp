#pragma once
#include <gtest/gtest.h>
#include "TestHelper.hpp"
#include "nullable.hpp"
#include "test_nullable.hpp"

// NOLINTBEGIN(misc-definitions-in-headers)
TEST(nullableTest, nullTest) {
    Nullable<int> num = Nullable<int>::Null;
    EXPECT_TRUE(num.IsNull());
    EXPECT_ANY_THROW({num.GetValue();});
} 


TEST(nullableTest, nonNullTest) {
    Nullable<int> num = 10;
    EXPECT_FALSE(num.IsNull());
    EXPECT_EQ(num.GetValue(), 10);
}
// NOLINTEND(misc-definitions-in-headers)

