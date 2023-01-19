#include "TestHelper.hpp"
#include "sparseVector.hpp"

TEST(SparseVector, GetAndSet)
{
  SparseVector<int, 100> vec;
  vec.Set(3, 1);
  vec.Set(50, 2);
  vec.Set(70, 3);

  for (int i = 0; i < vec.Dimension(); i++)
  {
    int value = vec.Get(i);
    switch (i)
    {
      case 3:
        ASSERT_EQ(value, 1);
        break;
      case 50:
        ASSERT_EQ(value, 2);
        break;
      case 70:
        ASSERT_EQ(value, 3);
        break;
      default:
        ASSERT_EQ(value, 0);
        break;
    }
  }
}

TEST(SparseVector, IsEqual)
{
  ASSERT_TRUE(SparseVector({1, 0, 0}) ==
              SparseVector({1, 0, 0}));

  ASSERT_FALSE(SparseVector({0, 1, 1}) ==
               SparseVector({0, 2, 1}));

  ASSERT_FALSE(SparseVector({0, 0, 1}) ==
               SparseVector({1, 0, 1}));
}

TEST(SparseVector, SumDiffAndMult)
{
  SparseVector a({1, 1, 1});
  SparseVector b({0, 0, 1});

  ASSERT_EQ(a + b, SparseVector({1, 1, 2}));
  ASSERT_EQ(a - b, SparseVector({1, 1, 0}));
  ASSERT_EQ(a * b, 1);
}

TEST(SparseVector, Copy)
{
  SparseVector a({1, 1, 1});

  SparseVector b = a;
  SparseVector c(a);

  ASSERT_EQ(a, b);
  ASSERT_EQ(a, c);
}