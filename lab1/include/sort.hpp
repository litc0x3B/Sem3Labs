#pragma once

#include <functional>
#include <type_traits>
#include <map>

#include "linkedList.hpp"
#include "sequence/linkedListSequence.hpp"
#include "sequence/sequence.hpp"
#include "TestHelper.hpp"
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
  T pivot = seq->At(start);
  int count = 0;
  for (int i = start + 1; i <= end; i++)
  {
    if (pivot > seq->At(i))
    {
      count++;
    }
  }

  int pivotInd = start + count;
  std::swap(seq->At(start), seq->At(pivotInd));

  int i = start;
  int j = end;
  while (i < pivotInd && pivotInd < j)
  {
    while(cmp(pivot, seq->At(i)) > 0 && pivotInd >= i) i++;
    while(cmp(pivot, seq->At(j)) <= 0 && pivotInd <= j) j--;

    if (i < pivotInd && pivotInd < j)
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

LinkedListSequence<int> *_GenerateDistances(int n)
{
  LinkedListSequence<int> *list = new LinkedListSequence<int>;
  LinkedList<int> *storage = list->GetStorage();

  list->Append(1);

  Node<int> *node2 = storage->GetNode(0);
  Node<int> *node3 = storage->GetNode(0);

  while (list->GetLast() < n) 
  {
    int num2 = 2 * node2->value;
    int num3 = 3 * node3->value;
    list->Append(num2 < num3 ? num2 : num3);

    if (num2 <= num3)
    {
      node2 = node2->next;
    }
    if (num2 >= num3)
    {
      node3 = node3->next;
    }
  }

  return list;
}

template<class T> Sequence<T> *ShellSort(Sequence<T> *seq, CompFunc<T> cmp = DefaultComparer)
{
  LinkedListSequence<int> *distances = _GenerateDistances(seq->GetSize() - 1);

  if (seq->GetSize() <= 1)
  {
    return seq;
  }

  while (distances->GetSize() != 0)
  {
    if (distances->GetLast() >= seq->GetSize())
    {
      distances->PopBack();
    }

    int distance = distances->GetLast();
    distances->PopBack();

    for (int i = distance; i < seq->GetSize(); i++)
    {
      T temp = seq->At(i);
      int j;
      for (j = i; j >= distance && seq->At(j - distance) > temp; j -= distance)
      {
        seq->At(j) = seq->At(j - distance);
      }
      seq->At(j) = temp;
    }
  }

  delete distances;
  return seq;
}

std::map<std::string, SortAlg<int>> gAlgMap = 
{
  {"n", {DoNothing<int>, "DoNothing", false}},
  {"bs", {BubbleSort<int>, "BubbleSort", false}},
  {"qs", {QuickSort<int>, "QuickSort", false}},
  {"ss", {ShellSort<int>, "ShellSort", false}},
};
