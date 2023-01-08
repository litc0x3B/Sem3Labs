#pragma once
#include "dictionary/dictionary.hpp"
#include "sequence/dynamicArraySequence.hpp"

template <class T, int dimension>
class SparseVector
{
 private:
  static int _Compare(int a, int b)
  {
    if (a > b) return 1;
    if (a < b) return -1;
    return 0;
  }

  using Dict = Dictionary<int, T>;
  std::shared_ptr<Dict> dict;

 public:
  SparseVector() { dict = std::make_shared<Dict>(_Compare); }

  SparseVector(Sequence<T> *seq) : SparseVector()
  {
    if (seq->GetSize() > dimension)
    {
      throw std::runtime_error("Sequence size is larger than SparseVector dimension");
    }

    for (int i = 0; i < seq->GetSize(); i++)
    {
      T num = seq->Get(i);
      if (!(num == T()))
      {
        dict->Add(i, num);
      }
    }
  }

  SparseVector(const SparseVector &vec) : SparseVector() { this->dict = vec.dict->Copy(); }

  void Set(int ind, const T &value)
  {
    if (ind < 0 || ind >= dimension)
    {
      throw std::out_of_range("index is out of range");
    }

    if (!dict->Get(ind).IsNull())
    {
      dict->Remove(ind);
    }

    if (!(value == T()))
    {
      dict->Add(ind, value);
    }
  }

  T Get(int ind)
  {
    if (ind < 0 || ind >= dimension)
    {
      throw std::out_of_range("index is out of range");
    }

    Nullable<T> retValue = dict->Get(ind);
    if (!retValue.IsNull())
    {
      return retValue.GetValue();
    }

    return T();
  }

  SparseVector &operator+(const SparseVector &vec)
  {
    vec.dict->ForEach([this](int ind, const T &value) { this->Set(ind, this->Get(ind) + value); });
    return *this;
  }

  SparseVector &operator-(const SparseVector &vec)
  {
    vec.dict->ForEach([this](int ind, const T &value) { this->Set(ind, this->Get(ind) - value); });
    return *this;
  }

  T operator*(const SparseVector &vec)
  {
    T retValue;
    vec.dict->ForEach([this, retValue](int ind, const T &value)
                      { retValue = retValue + this->Get(ind) * value; });
    return *this;
  }
};
