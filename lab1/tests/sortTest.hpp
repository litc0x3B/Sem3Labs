#include "TestHelper.hpp"
#include "sequence/linkedListSequence.hpp"
#include "sequence/dynamicArraySequenceTest.hpp"
#include "sequence/sequence.hpp"
#include "sort.hpp"


template<template<class> class SequenceType>
void SortTestFunc(SortFunc<int> func)
{
    int initArr[] = {9, 8, 6, 6, 6, 7, 8, 1, 5, 10, 4, 0, -5, -10};
    int expect[] =  {-10, -5, 0, 1, 4, 5, 6, 6, 6, 7, 8, 8, 9, 10};

    Sequence<int> *seq = new SequenceType<int>(initArr, ARRAY_SIZE(initArr, int));
    seq = func(seq, DefaultComparer<int>);

    containersEqual(*seq, expect, ARRAY_SIZE(expect, int));

    delete seq;
}

TEST(sortTest, BubbleSort)
{
    SortTestFunc<DynamicArraySequence>(BubbleSort<int>);
    SortTestFunc<LinkedListSequence>(BubbleSort<int>);
}

TEST(sortTest, QuickSort)
{
    SortTestFunc<DynamicArraySequence>(QuickSort<int>);
    SortTestFunc<LinkedListSequence>(QuickSort<int>);
}

TEST(sortTest, ShellSortDistaces)
{
    LinkedListSequence<int> *list = _GenerateDistances(9);
    int expect[] = {1, 2, 3, 4, 6, 8, 9};

    showContainer(*list, list->GetSize());

    ASSERT_EQ(list->GetSize(), ARRAY_SIZE(expect, int));
    containersEqual(*list, expect, ARRAY_SIZE(expect, int));
}

TEST(sortTest, ShellSort)
{
    SortTestFunc<DynamicArraySequence>(ShellSort<int>);
    SortTestFunc<LinkedListSequence>(ShellSort<int>);
}