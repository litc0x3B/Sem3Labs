#pragma once
#include "nullable.hpp"

template <class TKey, class TValue>
class IDictionary
{
 private:
  virtual IDictionary *_CopyImpl() const = 0;
 public:
  virtual Nullable<TValue> Get(TKey key) const = 0;
  virtual Nullable<TValue> Remove(TKey key) = 0;
  virtual int Size() const = 0;
  virtual void ForEach(const std::function<void(const TValue &)> &) const = 0;
  std::shared_ptr<IDictionary> Copy() const {return std::shared_ptr<IDictionary>(_CopyImpl());}
};
