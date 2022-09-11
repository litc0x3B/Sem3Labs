#pragma once

#include "TestHelper.hpp"
#include "sequence/dynamicArraySequence.hpp"
#include "sequence/sequence.hpp"
#include <gtest/gtest.h>

TEST(dynamicArraySequenceTest, defaultConstructor)
{
    Sequence<int> *array = new DynamicArraySequence<int>;
    ASSERT_EQ(array->GetSize(), 0);
    delete array;
}

TEST(dynamicArraySequenceTest, constructorFromArray)
{
    Sequence<int> *array = new DynamicArraySequence<int>(TEST_ARR, TEST_ARR_SIZE);
    containersEqual(*array, TEST_ARR, TEST_ARR_SIZE);
    ASSERT_EQ(array->GetSize(), TEST_ARR_SIZE);
    delete array;
}

TEST(dynamicArraySequenceTest, constructorCopy)
{
    Sequence<int> *array1 = new DynamicArraySequence<int>(TEST_ARR, TEST_ARR_SIZE);
    Sequence<int> *array2 = new DynamicArraySequence<int>(*(DynamicArraySequence<int>*)array1);

    containersEqual(*array1, *array2, TEST_ARR_SIZE);
    ASSERT_EQ(array1->GetSize(), array2->GetSize());

    delete array1;
    delete array2;
}

TEST(dynamicArraySequenceTest, constructorCopyZeroSize)
{
    Sequence<int> *array1 = new DynamicArraySequence<int>();
    Sequence<int> *array2 = new DynamicArraySequence<int>(*(DynamicArraySequence<int>*)array1);

    ASSERT_EQ(array1->GetSize(), array2->GetSize());

    delete array1;
    delete array2;
}

TEST(dynamicArraySequenceTest, GetLastZeroSize)
{
    Sequence<int> *array = new DynamicArraySequence<int>();
    EXPECT_ANY_THROW(array->GetLast());
    delete array;
}

TEST(dynamicArraySequenceTest, GetLast)
{
    Sequence<int> *array = new DynamicArraySequence<int>(TEST_ARR, TEST_ARR_SIZE);
    EXPECT_EQ(array->GetLast(), 7);
    delete array;
}

TEST(dynamicArraySequenceTest, GetFirstZeroSize)
{
    Sequence<int> *array = new DynamicArraySequence<int>();
    EXPECT_ANY_THROW(array->GetFirst());
    delete array;
}

TEST(dynamicArraySequenceTest, GetFirst)
{
    Sequence<int> *array = new DynamicArraySequence<int>(TEST_ARR, TEST_ARR_SIZE);
    EXPECT_EQ(array->GetFirst(), 0);
    delete array;
}

TEST(dynamicArraySequenceTest, bracketsOperator)
{
    Sequence<int> *array = new DynamicArraySequence<int>(TEST_ARR, TEST_ARR_SIZE);
    EXPECT_EQ(array->operator[](3), 3);
    delete array;
}

TEST(dynamicArraySequenceTest, bracketsOperatorException1)
{
    Sequence<int> *array = new DynamicArraySequence<int>(TEST_ARR, TEST_ARR_SIZE);
    EXPECT_ANY_THROW(array->operator[](8));
    delete array;
}

TEST(dynamicArraySequenceTest, bracketsOperatorException2)
{
    Sequence<int> *array = new DynamicArraySequence<int>(TEST_ARR, TEST_ARR_SIZE);
    EXPECT_ANY_THROW(array->operator[](-1));
    delete array;
}

TEST(dynamicArraySequenceTest, append1)
{
    Sequence<int> *array = new DynamicArraySequence<int>(TEST_ARR, TEST_ARR_SIZE);
    int expect[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    array->Append(8);

    containersEqual(*array, expect, TEST_ARR_SIZE + 1);
    EXPECT_EQ(array->GetSize(), TEST_ARR_SIZE + 1);
    EXPECT_EQ(array->GetActualSize(), TEST_ARR_SIZE * 2);

    delete array;
}

TEST(dynamicArraySequenceTest, append2)
{
    Sequence<int> *array = new DynamicArraySequence<int>(TEST_ARR, TEST_ARR_SIZE);
    int expect[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    array->Append(8);
    array->Append(9);

    containersEqual(*array, expect, TEST_ARR_SIZE + 2);
    EXPECT_EQ(array->GetSize(), TEST_ARR_SIZE + 2);
    EXPECT_EQ(array->GetActualSize(), TEST_ARR_SIZE * 2);

    delete array;
}

TEST(dynamicArraySequenceTest, appendZeroSize)
{
    Sequence<int> *array = new DynamicArraySequence<int>;
    int expect[] = {10};

    array->Append(10);

    containersEqual(*array, expect, 1);
    EXPECT_EQ(array->GetSize(), 1);
    delete array;
}

TEST(dynamicArraySequenceTest, prepend1)
{
    Sequence<int> *array = new DynamicArraySequence<int>(TEST_ARR, TEST_ARR_SIZE);
    int expect[] = {-1, 0, 1, 2, 3, 4, 5, 6, 7};

    array->Prepend(-1);

    containersEqual(*array, expect, TEST_ARR_SIZE + 1);
    EXPECT_EQ(array->GetSize(), TEST_ARR_SIZE + 1);
    EXPECT_EQ(array->GetActualSize(), TEST_ARR_SIZE * 2);

    delete array;
}

TEST(dynamicArraySequenceTest, prepend2)
{
    Sequence<int> *array = new DynamicArraySequence<int>(TEST_ARR, TEST_ARR_SIZE);
    int expect[] = {-2, -1, 0, 1, 2, 3, 4, 5, 6, 7};

    array->Prepend(-1);
    array->Prepend(-2);

    containersEqual(*array, expect, TEST_ARR_SIZE + 2);
    EXPECT_EQ(array->GetSize(), TEST_ARR_SIZE + 2);
    EXPECT_EQ(array->GetActualSize(), TEST_ARR_SIZE * 2);

    delete array;
}

TEST(dynamicArraySequenceTest, prependZeroSize)
{
    Sequence<int> *array = new DynamicArraySequence<int>;
    int expect[] = {10};

    array->Prepend(10);

    containersEqual(*array, expect, 1);
    EXPECT_EQ(array->GetSize(), 1);
    delete array;
}

TEST(dynamicArraySequenceTest, insertAt1)
{
    Sequence<int> *array = new DynamicArraySequence<int>(TEST_ARR, TEST_ARR_SIZE);
    int expect[] = {0, 1, 2, 10, 3,4, 5, 6, 7};

    array->InsertAt(10, 3);

    containersEqual(*array, expect, TEST_ARR_SIZE + 1);
    EXPECT_EQ(array->GetSize(), TEST_ARR_SIZE + 1);
    EXPECT_EQ(array->GetActualSize(), TEST_ARR_SIZE * 2);

    delete array;
}

TEST(dynamicArraySequenceTest, insertAt2)
{
    Sequence<int> *array = new DynamicArraySequence<int>(TEST_ARR, TEST_ARR_SIZE);
    int expect[] = {0, 1, 2, 10, 20, 3,4, 5, 6, 7};

    array->InsertAt(10, 3);
    array->InsertAt(20, 4);

    containersEqual(*array, expect, TEST_ARR_SIZE + 2);
    EXPECT_EQ(array->GetSize(), TEST_ARR_SIZE + 2);
    EXPECT_EQ(array->GetActualSize(), TEST_ARR_SIZE * 2);

    delete array;
}

TEST(dynamicArraySequenceTest, popBack)
{
    Sequence<int> *array = new DynamicArraySequence<int>(TEST_ARR, TEST_ARR_SIZE);
    int expect[] = {0, 1, 2, 3,4, 5, 6};

    array->PopBack();

    containersEqual(*array, expect, TEST_ARR_SIZE - 1);
    EXPECT_EQ(array->GetSize(), TEST_ARR_SIZE - 1);
    EXPECT_EQ(array->GetActualSize(), TEST_ARR_SIZE);

    delete array;
}

TEST(dynamicArraySequenceTest, popBack2)
{
    Sequence<int> *array = new DynamicArraySequence<int>(TEST_ARR, TEST_ARR_SIZE);
    int expect[] = {0, 1, 2, 3};
    int iterationsCount = 4;

    for (int i = 0; i < iterationsCount; i++)
    {
        array->PopBack();
    }

    containersEqual(*array, expect, TEST_ARR_SIZE - iterationsCount);
    EXPECT_EQ(array->GetSize(), TEST_ARR_SIZE - iterationsCount);
    EXPECT_EQ(array->GetActualSize(), TEST_ARR_SIZE / 2);

    delete array;
}

TEST(dynamicArraySequenceTest, popBackZeroSize)
{
    Sequence<int> *array = new DynamicArraySequence<int>();
    EXPECT_ANY_THROW(array->PopBack());
    delete array;
}

TEST(dynamicArraySequenceTest, popFront1)
{
    Sequence<int> *array = new DynamicArraySequence<int>(TEST_ARR, TEST_ARR_SIZE);
    int expect[] = {1, 2,3,4, 5, 6, 7};

    array->PopFront();

    containersEqual(*array, expect, TEST_ARR_SIZE - 1);
    EXPECT_EQ(array->GetSize(), TEST_ARR_SIZE - 1);
    EXPECT_EQ(array->GetActualSize(), TEST_ARR_SIZE);

    delete array;
}

TEST(dynamicArraySequenceTest, popFront2)
{
    Sequence<int> *array = new DynamicArraySequence<int>(TEST_ARR, TEST_ARR_SIZE);
    int expect[] = {4, 5, 6, 7};
    int iterationsCount = 4;

    for (int i = 0; i < iterationsCount; i++)
    {
        array->PopFront();
    }

    containersEqual(*array, expect, TEST_ARR_SIZE - iterationsCount);
    EXPECT_EQ(array->GetSize(), TEST_ARR_SIZE - iterationsCount);
    EXPECT_EQ(array->GetActualSize(), TEST_ARR_SIZE / 2);

    delete array;
}

TEST(dynamicArraySequenceTest, popFrontZeroSize)
{
    Sequence<int> *array = new DynamicArraySequence<int>();
    EXPECT_ANY_THROW(array->PopFront());
    delete array;
}

TEST(dynamicArraySequenceTest, deleteTest1)
{
    Sequence<int> *array = new DynamicArraySequence<int>(TEST_ARR, TEST_ARR_SIZE);
    int expect[] = {0, 1, 3,4, 5, 6, 7};

    array->Delete(2);

    containersEqual(*array, expect, TEST_ARR_SIZE - 1);
    EXPECT_EQ(array->GetSize(), TEST_ARR_SIZE - 1);
    EXPECT_EQ(array->GetActualSize(), TEST_ARR_SIZE);

    delete array;
}

TEST(dynamicArraySequenceTest, deleteTest2)
{
    Sequence<int> *array = new DynamicArraySequence<int>(TEST_ARR, TEST_ARR_SIZE);
    int expect[] = {0, 1, 6, 7};

    int iterationsCount = 4;

    for (int i = 0; i < iterationsCount; i++)
    {
        array->Delete(2);
    }

    containersEqual(*array, expect, TEST_ARR_SIZE - iterationsCount);
    EXPECT_EQ(array->GetSize(), TEST_ARR_SIZE - iterationsCount);
    EXPECT_EQ(array->GetActualSize(), TEST_ARR_SIZE / 2);

    delete array;
}

TEST(dynamicArraySequenceTest, deleteException1)
{
    Sequence<int> *array = new DynamicArraySequence<int>(TEST_ARR, TEST_ARR_SIZE);
    EXPECT_ANY_THROW(array->Delete(-1));
    delete array;
}

TEST(dynamicArraySequenceTest, deleteException2)
{
    Sequence<int> *array = new DynamicArraySequence<int>(TEST_ARR, TEST_ARR_SIZE);
    EXPECT_ANY_THROW(array->Delete(8));
    delete array;
}

TEST(dynamicArraySequenceTest, concat)
{
    int arr1[] = {0, 1, 2, 3};
    int arr2[] = {4, 5, 6, 7};

    Sequence<int> *dynamicArray1 = new DynamicArraySequence(arr1, ARRAY_SIZE(arr1, int));
    Sequence<int> *dynamicArray2 = new DynamicArraySequence(arr2, ARRAY_SIZE(arr2, int));
    Sequence<int> *dynamicArray3 = dynamicArray1->Concat(dynamicArray2);

    containersEqual(*dynamicArray3, TEST_ARR, TEST_ARR_SIZE);
    EXPECT_EQ(dynamicArray3->GetSize(), TEST_ARR_SIZE);

    delete dynamicArray1;
    delete dynamicArray2;
    delete dynamicArray3;
}

TEST(dynamicArraySequenceTest, concatZeroSize1)
{
    Sequence<int> *dynamicArray1 = new DynamicArraySequence<int>;
    Sequence<int> *dynamicArray2 = new DynamicArraySequence(TEST_ARR, TEST_ARR_SIZE);
    Sequence<int> *dynamicArray3 = dynamicArray1->Concat(dynamicArray2);

    containersEqual(*dynamicArray3, TEST_ARR, TEST_ARR_SIZE);
    EXPECT_EQ(dynamicArray3->GetSize(), TEST_ARR_SIZE);

    delete dynamicArray1;
    delete dynamicArray2;
    delete dynamicArray3;
}

TEST(dynamicArraySequenceTest, concatZeroSize2)
{
    Sequence<int> *dynamicArray1 = new DynamicArraySequence(TEST_ARR, TEST_ARR_SIZE);
    Sequence<int> *dynamicArray2 = new DynamicArraySequence<int>;
    Sequence<int> *dynamicArray3 = dynamicArray1->Concat(dynamicArray2);

    containersEqual(*dynamicArray3, TEST_ARR, TEST_ARR_SIZE);
    EXPECT_EQ(dynamicArray3->GetSize(), TEST_ARR_SIZE);

    delete dynamicArray1;
    delete dynamicArray2;
    delete dynamicArray3;
}

TEST(dynamicArraySequenceTest, subsequence)
{
    int expected[] = {2, 3, 4, 5};

    Sequence<int> *array1 = new DynamicArraySequence(TEST_ARR, TEST_ARR_SIZE);
    Sequence<int> *array2 = array1->GetSubsequence(2, 5);

    containersEqual(*array2, expected, ARRAY_SIZE(expected, int));
    EXPECT_EQ(array2->GetSize(), ARRAY_SIZE(expected, int));

    delete array1;
    delete array2;
}
