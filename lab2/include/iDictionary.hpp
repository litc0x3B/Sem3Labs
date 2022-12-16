#pragma once
#include "nullable.hpp"

template <class TKey, class TValue>
struct DictionaryEntry
{
  TKey key;
  Nullable<TValue> value;
};

template <class TKey, class TValue>
class IDictionary
{
 public:
  virtual Nullable<TValue> Get(TKey key) = 0;
  // virtual bool Add(DictionaryEntry<TKey, TValue> entry) = 0;
  virtual void Remove(TKey key) = 0;
};
