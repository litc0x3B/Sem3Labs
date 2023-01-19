#include <climits>
#include <cmath>
#include <iostream>

#include "sparseVector.hpp"
#include "speedTest.hpp"

using SparseVec = SparseVector<int, INT_MAX>;

std::mt19937 rng(time(nullptr));

SparseVec SparseVecGenerator(int nonZeroValues)
{
  SparseVec vec;

  std::uniform_int_distribution<> indices(0, INT_MAX - 1);
  std::uniform_int_distribution<> values(0, 100);

  for (int i = 0; i < nonZeroValues; i++)
  {
    vec.Set(indices(rng), values(rng));
  }

  return vec;
}

SparseVec SparseVecSum(const std::vector<SparseVec> &sparseVecs)
{
  return sparseVecs[0] + sparseVecs[1];
}

SparseVec SparseVecMult(const std::vector<SparseVec> &sparseVecs)
{
   sparseVecs[0] * sparseVecs[1];
   return sparseVecs[0];
}

int main()
{
  using namespace SpeedTest;

  Settings sparseVectorSettings = {50, 100000, 10};
  TestSubjectInfo<SparseVec> sparseVecInfo(SparseVecGenerator, 2);
  CasesVec<SparseVec> testInfoVec= {{"SparseVecSum", SparseVecSum},
                                     {"SparseVecMult", SparseVecMult}};

  SpeedTester<SparseVec> sparseVecTester(sparseVectorSettings, sparseVecInfo, testInfoVec);


  Results<SparseVec> results = sparseVecTester.PerformTests();
  results.OutputElapsedTime(std::cout);
}