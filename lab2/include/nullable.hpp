#pragma once
#include <stdexcept>

template <class T>
class Nullable
{
 private:
  bool isNull;
  T value;

 public:
  Nullable();
  Nullable(const Nullable &nullable);
  Nullable(T value);  // NOLINT
  Nullable &operator=(const T &value);
  bool IsNull() const;
  T GetValue() const;
  const static Nullable<T> Null;

  template <class TNew>
  Nullable<TNew> DynamicCast() const;
};
template <class T>
Nullable<T>::Nullable()
{
  isNull = true;
}

template <class T>
Nullable<T>::Nullable(const Nullable &nullable)
{
  this->value = nullable.value;
  this->isNull = nullable.isNull;
}

template <class T>
Nullable<T>::Nullable(T value)
{
  this->value = value;
  this->isNull = false;
}

template <class T>
Nullable<T> &Nullable<T>::operator=(const T &value)
{
  this->value = value;
  isNull = false;
  return *this;
}

template <class T>
bool Nullable<T>::IsNull() const
{
  return isNull;
}

template <class T>
T Nullable<T>::GetValue() const
{
  if (isNull)
  {
    throw std::runtime_error("attemp of getting access to a null value");
  }
  else
  {
    return value;
  }
}

template <class T>
template <class TNew>
Nullable<TNew> Nullable<T>::DynamicCast() const
{
  if (!IsNull())
  {
   return Nullable<TNew>(dynamic_cast<TNew>(GetValue()));
  }

  return Nullable<TNew>::Null;
}

template <class T>
const Nullable<T> Nullable<T>::Null = Nullable<T>();
