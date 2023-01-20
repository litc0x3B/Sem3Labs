#include <unistd.h>

#include <climits>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>

#include "sparseVector.hpp"
#include "speedTest.hpp"

using SparseVec = SparseVector<int, INT_MAX>;
using TestFunc = std::function<SpeedTest::Results(SpeedTest::Settings)>;

std::mt19937 rng(time(nullptr));

namespace SparseVecST
{
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

void SparseVecSum(std::vector<SparseVec> &sparseVecs) { sparseVecs[0] + sparseVecs[1]; }

void SparseVecMult(std::vector<SparseVec> &sparseVecs) { sparseVecs[0] + sparseVecs[1]; }

SpeedTest::Results Test(SpeedTest::Settings settings)
{
  using namespace SpeedTest;

  TestSubjectInfo<SparseVec> sparseVecInfo(SparseVecGenerator, 2);
  CasesVec<SparseVec> testInfoVec = {{"SparseVecSum", SparseVecSum},
                                     {"SparseVecMult", SparseVecMult}};

  SpeedTester<SparseVec> sparseVecTester(settings, sparseVecInfo, testInfoVec);

  return sparseVecTester.PerformTests();
}
}  // namespace SparseVecST

int StringCmp(const std::string &str1, const std::string &str2) { return str1.compare(str2); }

Dictionary<std::string, TestFunc> g_testFuncDict(StringCmp, {{"SparseVector", SparseVecST::Test}});

int GetNotNegativeNumArg(std::string errorMessage)
{
  int ret;

  try
  {
    ret = std::stoi(optarg);
  }
  catch (const std::invalid_argument &e)
  {
    std::cout << errorMessage << std::endl;
    exit(0);
  }

  if (ret <= 0)
  {
    std::cout << errorMessage << std::endl;
    exit(0);
  }

  return ret;
}

struct Parametrs
{
  std::string outputFileName = "out.csv";
  SpeedTest::Settings testSettings = {50, 10000, 10};
  Nullable<TestFunc> testFunc;
};

Parametrs ReadArgs(int argc, char *argv[])
{
  int opt;
  Parametrs params;

  while ((opt = getopt(argc, argv, "t:o:s:e:c:")) != -1)
  {
    switch (opt)
    {
      case 't':
      {
        auto testFunc = g_testFuncDict.Get(optarg);
        if (testFunc.IsNull())
        {
          std::cout << "Не найдено теста для структуры \"" << optarg << "\"" << std::endl;
        }
        else
        {
          params.testFunc = testFunc;
        }
        break;
      }

      case 'o':
      {
        params.outputFileName = optarg;
        break;
      }

      case 's':
      {
        params.testSettings.minSize =
            GetNotNegativeNumArg("Неверное значение для начального количества элементов");
        break;
      }

      case 'e':
      {
        params.testSettings.maxSize =
            GetNotNegativeNumArg("Неверное значение для конечного количества элементов");
        break;
      }

      case 'c':
      {
        params.testSettings.iterationCount =
            GetNotNegativeNumArg("Неверное значение для количества итераций");
        break;
      }

      case '?':
      {
        std::cout << "формат: -t <название структуры> -с <количечтво итераций> -s <начальное "
                     "количество эл-ов> -e <конечное количество эл-ов> -o <имя файла>"
                  << std::endl;
        exit(0);
        break;
      }
    }
  }

  if (params.testFunc.IsNull())
  {
    std::cout << "Не указана структура для тестирования. Выход." << std::endl;
    exit(0);
  }

  return params;
}

int main(int argc, char *argv[])
{
  Parametrs params = ReadArgs(argc, argv);
  std::ofstream fout(params.outputFileName);
  std::cout << "Выполнение тестов может занять некторое время..." << std::endl;
  fout << params.testFunc.GetValue()(params.testSettings);
  std::cout << "Результаты были записаны в файл \"" << params.outputFileName << "\"" << std::endl;
}