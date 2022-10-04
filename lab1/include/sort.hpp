#pragma once

#include <functional>
#include <type_traits>
#include <map>

#include "sequence/sequence.hpp"
#include "TestHelper.hpp"

 
template<class T> using CompFunc = std::function<int(T,T)>;
template<class T> using SortFunc = std::function<Sequence<T>*(Sequence<T>*, CompFunc<T>)>;

template<class T> struct SortAlg 
{
  SortFunc<T> func;
  std::string name;
  bool isSelected = false;
};

template<class T> int DefaultComparer(T a, T b) 
{
  if (a > b) return 1;
  if (a < b) return -1;
  return 0;
}

template<class T> Sequence<T> *BubbleSort(Sequence<T> *seq, CompFunc<T> cmp = DefaultComparer) 
{
  for (int i = 0; i < seq->GetSize(); i++)
  {
    for (int j = 0; j < seq->GetSize() - i - 1; j++)
    {
      if (cmp(seq->operator[](j), seq->operator[](j + 1)) > 0)
      {
        std::swap(seq->operator[](j), seq->operator[](j + 1));
      }
    }
  }

  return seq;
}

template<class T> Sequence<T> *DoNothing(Sequence<T> *seq, CompFunc<T> cmp = DefaultComparer)
{
  return seq;
} 

std::map<std::string, SortAlg<int>> gAlgMap = 
{
  {"bs", {BubbleSort<int>, "Bubble", false}},
  {"n", {DoNothing<int>, "DoNothing", false}}
};
