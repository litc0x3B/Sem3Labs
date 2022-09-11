#include <cstdlib>
#include <iostream>
#include <map>
#include <ostream>
#include <stdexcept>
#include <unistd.h>
#include <functional>
#include <string>

#include "sequence/dynamicArraySequence.hpp"
#include "sort.hpp"


template<class T> struct SortAlg 
{
    SortFunc<T> func;
    bool isSelected = false;
    std::string name;
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

void TestAlg()
{

}

int main(int argc, char *argv[])
{
    std::map<std::string, SortAlg<int>> algMap = 
    {
        {"bs", {BubbleSort<int>, false, "Bubble Sort"}}
    };

    bool outputFileSpecified = false;
    bool algSelected = false;
    bool useLinkedLists = false;
    bool useArrays = true;
    int opt;

    std::string outputFile = "output.txt";
    char *inputAndOututFile = nullptr;

    int startCount = 1;
    int endCount = 100000000;
    int multiplier = 10;

    //read args
    while((opt = getopt(argc, argv, "a:o:s:e:m:w:t:")) != -1)
    {
        switch (opt) 
        {
            case 'a':
                try
                {
                    algMap.at(optarg).isSelected = true;
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
                    outputFile = optarg;
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
                    useArrays = true;
                    useLinkedLists = true;
                }
                else if (arg == "linkedlist")
                {
                    useArrays = false;
                    useLinkedLists = true;
                }
                else if (arg == "array")
                {
                    useArrays = true;
                    useLinkedLists = false;
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

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;

    //case 1: random generated sequence
    for (int elements = startCount; elements <= endCount; elements *= multiplier)
    {
        if (useArrays)
        {
            DynamicArraySequence<int> arr;

            for (int i = 0; i < elements; i++)
            {
                arr.Append(rand());
            }

        }
    }

}