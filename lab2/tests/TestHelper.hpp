#pragma once

#include <gtest/gtest.h>

#include "dictionary/iDictionary.hpp"

#define ARRAY_SIZE(array) sizeof(array) / sizeof(array[0])

const int TEST_ARR[] = {0, 1, 2, 3, 4, 5, 6, 7};
const int TEST_ARR_SIZE = ARRAY_SIZE(TEST_ARR);

//checks if dictionary has elements for all of these keys and only for these keys
template <class TKey, class TValue, unsigned long size>
void doesDictHave(IDictionary<TKey, TValue> *dict, std::array<TKey, size> keys, std::array<TValue, size> values)
{
  for (int i = 0; i < keys.size(); i++) 
  {
    EXPECT_EQ(dict->Get(keys.at(i)).GetValue(), values.at(i));
  }

  EXPECT_EQ(dict->Size(), (int)values.size());
}

template <typename T>
std::stringstream containerToStream(T &container, int size)
{
    std::stringstream stream;
    stream << "{";
    for (int i = 0; i < size; i++)
    {
        stream << container[i];
        if (i != size - 1)
        {
            stream << ", ";
        }
    }
    stream << "}";

    return stream;
}

template <typename T1, typename T2>
void containersEqual(T1 &container1, T2 &container2, int size)
{
   bool areEqual = true;

   for (int i = 0; i < size; i++)
   {
      areEqual = container1[i] == container2[i];
      if (!areEqual)
      {
         break;
      }
   }

   if (!areEqual)
   {
       std::stringstream errorStream;
       errorStream << "Expected equality of these values:" << std::endl;
       errorStream << "\t" << containerToStream(container1, size).rdbuf() << std::endl;
       errorStream << "\t" << containerToStream(container2, size).rdbuf() << std::endl;
       ADD_FAILURE() << errorStream.rdbuf();
   }
}

template<typename T>
void showContainer(T &container, int size)
{
    std::cerr << containerToStream(container, size).rdbuf() << std::endl;
}
