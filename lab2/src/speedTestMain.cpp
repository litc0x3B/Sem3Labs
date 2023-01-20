#include <unistd.h>

#include <climits>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>

#include "TestIndexing.hpp"
#include "dictionary/indexing.hpp"
#include "sparseVector.hpp"
#include "speedTest.hpp"

using SparseVec = SparseVector<int, INT_MAX>;
using TestFunc = std::function<SpeedTest::Results(SpeedTest::Settings)>;

std::mt19937 rng(time(nullptr));

namespace SparseVecST
{
SparseVec Generator(int nonZeroValues)
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

void Sum(std::vector<SparseVec> &sparseVecs) { sparseVecs[0] + sparseVecs[1]; }

void Mult(std::vector<SparseVec> &sparseVecs) { sparseVecs[0] * sparseVecs[1]; }

SpeedTest::Results Test(SpeedTest::Settings settings)
{
  using namespace SpeedTest;

  TestSubjectInfo<SparseVec> sparseVecInfo(Generator, 2);
  CasesVec<SparseVec> testInfoVec = {{"Sum", Sum}, {"Mult", Mult}};

  SpeedTester<SparseVec> sparseVecTester(settings, sparseVecInfo, testInfoVec);

  return sparseVecTester.PerformTests();
}
}  // namespace SparseVecST

namespace IndexedDictST
{

std::string names[] = {
    "Мантикора",  "Русалина",    "Джавелина", "Юния",       "Николоз",    "Ульпана",
    "Октавия",    "Мечта",       "Луна",      "Квитослава", "Фелиция",    "Авиталь-Азалина",
    "Мелисса",    "Мерьела",     "Божена",    "Афина",      "Сладислава", "Олимпия",
    "Камильена",  "Апполинария", "Квита",     "Жасмин",     "Забава",     "Весенняя",
    "Эсмеральда", "Квитослава",  "Афродита",  "Карамель",   "Мальвина",   "Тиша",
    "Эсмеральда", "Санта",       "Мелодисон", "Ореста",     "Фрота",      "Росинка",
    "Сапфира",    "Влана",       "Агата",     "Кассандра",  "Мадонна",    "Глафира",
    "Герда",      "Фиона",       "Цветок",    "Фея",        "Агуник",     "Хауля",
    "Империя",    "Флорентина",  "Дьйомерси", "Златослава", "Лаян",       "Искра",
    "Орияна",     "Юнона",       "Ахилес",    "Велеслав",   "Сармат",     "Ларс",
    "Саркис",     "Стилиан",     "Хьюго",     "Никодим",    "Нектарий",   "Калеп",
    "Свитозар",   "Нимрод",      "Марс",      "Искандер",   "Волислав",   "Эльдар",
    "Златослав",  "Герман",      "Роберт",    "Мирон",      "Эдвард",     "Эрик",
    "Елисей",     "Ян",          "Радомир",   "Лучезар",    "Любомир",    "Светозар",
    "Моисей",     "Осман",       "Царь",      "Ярополк",    "Богуслав",   "Соломон",
    "Рожден",     "Мечислав",    "Феликс",    "Тамерлан",   "Одиссей",    "Савватий",
    "Ангел",      "Измаил",      "Илай",      "Спартак",    "Азарий",     "Варфоломей",
    "Принц",      "Алимп",       "Алихан",    "Вильдан",    "Довуд",      "Стратон",
    "Робин",      "Умут"};

TestIndexing::Person target = {"Сырник", 90, 9};

using Dict = IndexedDict<TestIndexing::PersonKey, TestIndexing::Person>;

std::shared_ptr<Dict> Generator(int size)
{
  auto dict = std::make_shared<Dict>(TestIndexing::attrs);
  std::uniform_int_distribution<> nameIds(0, sizeof(names) / sizeof(std::string) - 1);
  std::uniform_int_distribution<> heights(150, 190);
  std::uniform_int_distribution<> ages(1, size);
  std::uniform_int_distribution<> targetProb(1, size);
  bool isTargetAdded = false;

  for (int i = 0; i < size - 1; i++)
  {
    TestIndexing::Person person;
    if (isTargetAdded || targetProb(rng) != 9/*не срабатывает с верооятностью (1 / size)*/)
    {
      person.name = names[nameIds(rng)];
      person.age = ages(rng);
      person.height = heights(rng);
    }
    else
    {
      person = target;
      isTargetAdded = true;
      std::cout << "Добавил target при i = " << i << std::endl;
    }

    dict->Add(person);
    //    std::cout << person << std::endl;
  }

  if (!isTargetAdded)
  {
    dict->Add(target);
  }
  std::cout << dict->Size() << std::endl;
  return dict;
}

void Get(std::vector<std::shared_ptr<Dict>> &dictVec)
{
  auto found = dictVec[0]->Get({"Сырник", 90});
  if (!found.IsNull())
  {
    std::cout << "Нашёл target: " << found.GetValue() << std::endl;
  }
}

void GetInRange(std::vector<std::shared_ptr<Dict>> &dictVec)
{
  TestIndexing::PersonKey keyMin = {"А", 1};
  TestIndexing::PersonKey keyMax = {"Ая", 100};
  auto found = dictVec.at(0)->GetInRange(keyMin, keyMax);
}

SpeedTest::Results Test(SpeedTest::Settings settings)
{
  using namespace SpeedTest;

  TestSubjectInfo<std::shared_ptr<Dict>> subjInfo(Generator, 1);
  CasesVec<std::shared_ptr<Dict>> casesInfo = {{"Get", Get}, /*{"GetInRange", GetInRange}*/};

  SpeedTester<std::shared_ptr<Dict>> tester(settings, subjInfo, casesInfo);

  return tester.PerformTests();
}
}  // namespace IndexedDictST

int StringCmp(const std::string &str1, const std::string &str2) { return str1.compare(str2); }

Dictionary<std::string, TestFunc> g_testFuncDict(StringCmp, {{"SparseVector", SparseVecST::Test},
                                                             {"IndexedDict", IndexedDictST::Test}});

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

  while ((opt = getopt(argc, argv, "t:o:s:e:i:")) != -1)
  {
    switch (opt)
    {
      case 't':
      {
        auto testFunc = g_testFuncDict.Get(optarg);
        if (testFunc.IsNull())
        {
          std::cout << "Не найдено тестов для структуры \"" << optarg << "\"" << std::endl;
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

      case 'i':
      {
        params.testSettings.iterationCount =
            GetNotNegativeNumArg("Неверное значение для количества итераций");
        break;
      }

      case '?':
      {
        std::cout << "формат: -t <название структуры> -i <количечтво итераций> -s <начальное "
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

  if (params.testSettings.CalcStep() <= 0)
  {
    std::cout << "Неправильные значения для начального количества элементов и/или конечного "
                 "количества элементов и/или количества итераций (шаг <= 0)"
              << std::endl;
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