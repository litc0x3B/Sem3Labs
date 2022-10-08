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
    SortTestFunc<LinkedListSequence>(BubbleSort<int>);
    SortTestFunc<DynamicArraySequence>(BubbleSort<int>);
}