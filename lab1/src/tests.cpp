#include <gtest/gtest.h>

#include "sequence/linkedListSequenceTest.hpp"
#include "sequence/dynamicArraySequenceTest.hpp"
#include "sortTest.hpp"

int main()
{
    testing::InitGoogleTest();
    RUN_ALL_TESTS();
    return 0;

}