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

template<class T> int _Partition(Sequence<T> *seq, CompFunc<T> cmp, int start, int end)
{
  int pivotInd = (end - start) / 2 + start;

  int i = start;
  int j = end;
  while (i <= pivotInd && pivotInd <= j)
  {
    while(cmp(seq->At(pivotInd), seq->At(i)) > 0 && pivotInd >= j) i++;
    while(cmp(seq->At(pivotInd), seq->At(j)) < 0 && pivotInd <= j) j--;

    if (i != j)
    {
      std::swap(seq->At(i++), seq->At(j--));
    }
  }

  return pivotInd;
}

template<class T> void _QuickSort(Sequence<T> *seq, CompFunc<T> cmp, int start, int end)
{
    if (start >= end)   
      return;

    int pivotInd = _Partition(seq, cmp, start, end);
    _QuickSort(seq, cmp, start, pivotInd - 1);
    _QuickSort(seq, cmp, pivotInd + 1, end);
}

template<class T> Sequence<T> *QuickSort(Sequence<T> *seq, CompFunc<T> cmp = DefaultComparer) 
{
  _QuickSort(seq, cmp, 0, seq->GetSize() - 1);
  return seq;
}

template<class T> Sequence<T> *DoNothing(Sequence<T> *seq, CompFunc<T> cmp = DefaultComparer)
{
  return seq;
} 


std::map<std::string, SortAlg<int>> gAlgMap = 
{
  {"n", {DoNothing<int>, "DoNothing", false}},
  {"bs", {BubbleSort<int>, "BubbleSort", false}},
  {"qs", {QuickSort<int>, "QuickSort", false}}
};
