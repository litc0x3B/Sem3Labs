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
  SparseVector() {
    static_assert(dimension > 0, "dimension value cannot be negative");
    dict = std::make_shared<Dict>(_Compare);
  }

  explicit SparseVector(Sequence<T> *seq) : SparseVector()
  {
    if (seq->GetSize() != dimension)
    {
      throw std::runtime_error("Sequence size isn't equal to vector dimension");
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

  explicit SparseVector(const T(&arr)[dimension]) : SparseVector()
  {
    for (int i = 0; i < dimension; i++)
    {
      T num = arr[i];
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

    dict->Remove(ind);

    if (!(value == T()))
    {
      dict->Add(ind, value);
    }
  }

  T Get(int ind) const
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

  int Dimension()
  {
    return dimension;
  }

  SparseVector operator+(const SparseVector &vec) const
  {
    SparseVector newVec = *this;
    vec.dict->ForEach([&newVec](int ind, const T &value) { newVec.Set(ind, newVec.Get(ind) + value); });
    return newVec;
  }

  SparseVector operator-(const SparseVector &vec) const
  {
    SparseVector newVec = *this;
    vec.dict->ForEach([&newVec](int ind, const T &value) { newVec.Set(ind, newVec.Get(ind) - value); });
    return newVec;
  }

  T operator*(const SparseVector &vec) const
  {
    T retValue = T();
    vec.dict->ForEach([this, &retValue](int ind, const T &value)
                      { retValue = retValue + this->Get(ind) * value; });
    return retValue;
  }

  bool operator==(const SparseVector &vec) const
  {
    if (this->dict->Size() != vec.dict->Size())
    {
      return false;
    }

    bool flag = true;
    this->dict->ForEach([&vec, &flag](int ind, const T &value)
                        {
                          Nullable<T> found = vec.dict->Get(ind);
                          if (found.IsNull() || found.GetValue() != value)
                          {
                            flag = false;
                          }
                        });
    return flag;
  }

  bool operator!=(const SparseVector &vec) const
  {
    return !(*this == vec);
  }
};
