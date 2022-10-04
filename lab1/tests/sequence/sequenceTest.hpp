#pragma once

#include "TestHelper.hpp"
#include "sequence/dynamicArraySequence.hpp"
#include "sequence/linkedListSequence.hpp"
#include "sequence/sequence.hpp"

template <template<class> class SeqTypeDest, template<class> class SeqTypeSrc>
void CopySeqFunc()
{
    Sequence<int> *src = new SeqTypeSrc<int>(TEST_ARR, TEST_ARR_SIZE);
    Sequence<int> *dest = (new SeqTypeDest<int>)->Copy(src);

    containersEqual(*src, *dest, TEST_ARR_SIZE);
}

TEST(sequenceTest, copyArrList)
{
    CopySeqFunc<DynamicArraySequence, LinkedListSequence>();
}

TEST(sequenceTest, copyListList)
{
    CopySeqFunc<LinkedListSequence, LinkedListSequence>();
}

TEST(sequenceTest, copyListArr)
{
    CopySeqFunc<LinkedListSequence, DynamicArraySequence>();
}

TEST(sequenceTest, copyArrArr)
{
    CopySeqFunc<DynamicArraySequence, DynamicArraySequence>();
}