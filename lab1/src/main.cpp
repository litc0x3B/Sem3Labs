#include <cstdlib>
#include <iostream>
#include <map>
#include <ostream>
#include <stdexcept>
#include <unistd.h>
#include <functional>
#include <string>
#include <chrono>
#include <fstream>
#include <iomanip>

#include "sequence/dynamicArraySequence.hpp"
#include "sequence/linkedListSequence.hpp"
#include "sequence/sequence.hpp"
#include "sort.hpp"

enum class SeqTypes
{
  list, array, both
};

struct Parametrs
{
  int strWidth = 15;

  bool outputFileSpecified = false;
  bool algSelected = false;

  std::string timeFileName = "output.txt";
  char *seqFileName = nullptr;

  int startCount = 1;
  int endCount = 10000;
  int multiplier = 1;
  int step = 100;

  SeqTypes seqTypes = SeqTypes::array;  
};

bool isListSelected(SeqTypes type)
{
  return type == SeqTypes::both || type == SeqTypes::list;
}

bool isArraySelected(SeqTypes type)
{
  return type == SeqTypes::both || type == SeqTypes::array;
}


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


template<template <class> class SequenceType>
Sequence<int> *TestAlg(const Sequence<int> *seq, SortAlg<int> alg, int &elapsed)
{
  Sequence<int> *seqCopy = (new SequenceType<int>)->Copy(seq);

  using std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using std::chrono::milliseconds;

  auto t1 = high_resolution_clock::now();
  alg.func(seqCopy, DefaultComparer<int>);
  auto t2 = high_resolution_clock::now();

  elapsed = duration_cast<milliseconds>(t2 - t1).count();

  return seqCopy;
}


void WriteWithCheck(std::ofstream &ofstream, const std::string &str)
{
  if (ofstream.is_open())
  {
    ofstream << str << std::endl;
  }
}


template<template <class> class SeqType>
void TestAlgWithSeq(const Sequence<int> *seq, std::ofstream &timeOutput, std::ofstream &seqOutput, SortAlg<int> alg, int strWidth)
{
  int elapsed;
  Sequence<int> *sortedSeq = TestAlg<SeqType>(seq, alg, elapsed);

  timeOutput << std::setw(strWidth) << elapsed;
  for (int i = 0; i < sortedSeq->GetSize() && seqOutput.is_open(); i++)
  {
    seqOutput << sortedSeq->operator[](i) << " ";
  }

  WriteWithCheck(seqOutput, "");
  delete sortedSeq;
}


void WriteSelectedAlgs(std::ofstream &timeOutput, Parametrs params)
{
  timeOutput << std::setw(params.strWidth) << "Elements";

  for (auto const &el : gAlgMap)
  { 
    if (el.second.isSelected) 
    {
      if (isListSelected(params.seqTypes))
        timeOutput << std::setw(params.strWidth) << el.second.name << "(list)";
      if (isArraySelected(params.seqTypes))
        timeOutput << std::setw(params.strWidth) << el.second.name << "(arr)";
    }
  }

  timeOutput << std::endl;
}

void Test(const Sequence<int> *seq, Parametrs params, std::ofstream &timeOutput, std::ofstream &seqOutput)
{
  WriteWithCheck(seqOutput, "Исходная последовательность:");
  for (int i = 0; i < seq->GetSize() && seqOutput.is_open(); i++)
  {
    seqOutput << seq->operator[](i) << " ";
  }
  WriteWithCheck(seqOutput, "");

  timeOutput << std::setw(params.strWidth) << seq->GetSize();

  for (auto const &alg : gAlgMap)
  {

    if (alg.second.isSelected)
    {
      if (isArraySelected(params.seqTypes)) 
      {
        WriteWithCheck(seqOutput, "отсортированный DynamicArraySequence:");
        TestAlgWithSeq<DynamicArraySequence>(seq, timeOutput, seqOutput, alg.second, params.strWidth);
      }
      else if (isListSelected(params.seqTypes)) 
      {
        WriteWithCheck(seqOutput, "отсортированный LinkedListSequence:");
        TestAlgWithSeq<LinkedListSequence>(seq, timeOutput, seqOutput, alg.second, params.strWidth);
      }
    }
  }

  WriteWithCheck(seqOutput, "************************************");
  timeOutput << std::endl;
}


Parametrs ReadArgs(int argc, char *argv[])
{
  int opt;
  Parametrs params;
  bool algSelected = false;

  while((opt = getopt(argc, argv, "a:o:s:e:m:w:t:p:i:")) != -1)
  {
    switch (opt) 
    {
      case 'a':
        try
        {
          gAlgMap.at(optarg).isSelected = true;
          algSelected = true;
        }
        catch (const std::out_of_range &e)
        {
          std::cout << "Не найден алгоритм: " << optarg << std::endl;
          exit(0);
        }
      break;

      case 'o':
        if (!params.outputFileSpecified)
        {
          params.timeFileName = optarg;
          params.outputFileSpecified = true;
        }
        else 
        {
          std::cout << "Невозможно указать более чем один файл для вывода" << std::endl;
          exit(0);
        }
      break;

      case 's':
        params.startCount = GetNotNegativeNumArg("Неверное значение для начального количества элементов");
      break;

      case 'e':
        params.endCount = GetNotNegativeNumArg("Неверное значение для конечного количества элементов");
      break;

      case 'm':
        params.multiplier = GetNotNegativeNumArg("Неверное значение для монжителя");
      break;

      case 'p':
        params.step = GetNotNegativeNumArg("Неверное значение для шага");
      break;

      case 'w':
        params.seqFileName = optarg;
      break;

      case '?':
        exit(0);
      break;


      case 't':
        std::string arg = optarg;

        if (arg == "both")    
        {
          params.seqTypes = SeqTypes::both;
        }
        else if (arg == "list") 
        {
          params.seqTypes = SeqTypes::list;
        }
        else if (arg == "array") 
        { 
          params.seqTypes = SeqTypes::array;
        }
        else
        {
          std::cout << "Неправильный аргумент для типа последовательности" << std::endl;
        }
      break; 
    }
  }

  if (!algSelected)
  {
    std::cout << "Не указаны алгоритмы для тестирования" << std::endl;
    exit(0);
  }

  return params;
}

int main(int argc, char *argv[])
{
  Parametrs params = ReadArgs(argc, argv);
  
  
  //algorithms tests
  std::srand(std::time(0));

  std::ofstream timeOutput(params.timeFileName);
  std::ofstream seqOutput;

  if (params.seqFileName != nullptr)
  {
    seqOutput.open(params.seqFileName);
  }

  //case 1: random generated sequence
  WriteSelectedAlgs(timeOutput, params);
  for (int elements = params.startCount; elements <= params.endCount; elements = (elements + params.step) * params.multiplier)
  {
    auto *seq = new DynamicArraySequence<int>();

    for (int i = 0; i < elements; i++)
    {
      int rndNum = rand();
      seq->Append(rndNum);
    }

    Test(seq, params, timeOutput, seqOutput);
    delete seq;
  }

  timeOutput.close();

  if (seqOutput.is_open())
  {
    seqOutput.close();
  }
}