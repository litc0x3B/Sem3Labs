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

#include "sequence/dynamicArraySequence.hpp"
#include "sequence/linkedListSequence.hpp"
#include "sequence/sequence.hpp"
#include "sort.hpp"

enum class SeqTypes
{
    list, array, both
};

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
        
    }

    if (ret <= 0)
    {
        std::cout << errorMessage << std::endl;
        exit(0);
    }

    return -1;
}


template<template <class> class SequenceType>
int TestAlg(Sequence<int> *seq, SortAlg<int> alg)
{
    Sequence<int> *seqCopy = (new SequenceType<int>)->Copy(seq);

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();
    alg.func(seqCopy, DefaultComparer<int>);
    auto t2 = high_resolution_clock::now();

    auto delta = duration_cast<milliseconds>(t2 - t1);

    delete seqCopy;

    return delta.count();
}


void TestAlgs(Sequence<int> *seq, SeqTypes seqTypes, std::ofstream &output)
{
    for (auto const &alg : gAlgMap)
        {
            if (alg.second.isSelected)
            {
                if (seqTypes == SeqTypes::array)
                {
                    int time = TestAlg<DynamicArraySequence>(seq, alg.second);
                    output << time << "\t\t\t";
                }
                else if (seqTypes == SeqTypes::list)
                {
                    int time = TestAlg<LinkedListSequence>(seq, alg.second);
                    output << time << "\t\t\t";
                }
                else if (seqTypes == SeqTypes::both)
                {
                    int timeArr = TestAlg<DynamicArraySequence>(seq, alg.second);
                    int timeList = TestAlg<LinkedListSequence>(seq, alg.second);
                    output << timeArr<< "\t\t\t" << timeList << "\t\t\t";
                }
            }
        }
}

int main(int argc, char *argv[])
{
    bool outputFileSpecified = false;
    bool algSelected = false;
    int opt;

    std::string outputFileName = "output.txt";
    char *inputAndOututFile = nullptr;

    int startCount = 1;
    int endCount = 100000000;
    int multiplier = 10;

    SeqTypes seqTypes = SeqTypes::array;        

    //read args
    while((opt = getopt(argc, argv, "a:o:s:e:m:w:t:")) != -1)
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
                    return 0;
                }
            break;

            case 'o':
                if (!outputFileSpecified)
                {
                    outputFileName = optarg;
                    outputFileSpecified = true;
                }
                else 
                {
                    std::cout << "Невозможно указать более чем один файл для вывода" << std::endl;
                    return 0;
                }
            break;

            case 's':
                startCount = GetNotNegativeNumArg("Неверное значение для начального количества элементов");
            break;

            case 'e':
                endCount = GetNotNegativeNumArg("Неверное значение для конечного количества элементов");
            break;

            case 'm':
                endCount = GetNotNegativeNumArg("Неверное значение для монжителя");
            break;

            case 'w':
                inputAndOututFile = optarg;
            break;

            case 't':
                std::string arg = optarg;

                if (arg == "both")       
                {
                    seqTypes = SeqTypes::both;
                }
                else if (arg == "linkedlist") 
                {
                    seqTypes = SeqTypes::list;
                }
                else if (arg == "array") 
                {  
                    seqTypes = SeqTypes::array;
                }
                else
                {
                    std::cout << "Неправильный аргумент для типа последовательности" << std::endl;
                }

        }
    }

    if (!algSelected)
    {
        std::cout << "Не указаны алгоритмы для тестирования" << std::endl;
        return 0;
    }


    //algorithms tests
    std::srand(std::time(0));

    //case 1: random generated sequence
    for (int elements = startCount; elements <= endCount; elements *= multiplier)
    {
        auto *seq =  new DynamicArraySequence<int>();

        for (int i = 0; i < elements; i++)
        {
            int rndNum = rand();
            seq->Append(rndNum);
            if (seq != nullptr)
            {
                seq->Append(rndNum);
            }
        }

        std::ofstream outputFileStream;
        outputFileStream.open(outputFileName);
        TestAlgs(seq, seqTypes, outputFileStream);
    }

}