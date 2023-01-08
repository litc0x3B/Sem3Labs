#pragma once
#include <functional>
#include <memory>
#include <string>
#include "nullable.hpp"

template<class T> 
class ICollection
{
public:
    virtual int GetSize() const = 0;
    virtual void Add(const T &item) = 0;
    virtual Nullable<T> Remove(const T &item) = 0;
    virtual Nullable<T> Search(const T &item) const = 0;

    virtual T Reduce(const std::function<T(const T&, const T&)> &handlerFunc, T identity) const = 0;
    virtual void ForEach(const std::function<void(const T& item)> &handlerFunc) const = 0;

    virtual std::string ToStr(const std::function<std::string(const T&)> &itemToStr = static_cast<std::string(*)(T)>(std::to_string), 
                             std::string delim = " ") const = 0;
    virtual void AddFromStr(const std::function<T(const std::string&)> &strToItem, std::string str, std::string delim = " ") = 0;

    std::shared_ptr<ICollection<T>> Map(const std::function<T(const T& item)> &handlerFunc) const 
    {
        return std::shared_ptr<ICollection<T>>(_MapImpl(handlerFunc));
    }

    std::shared_ptr<ICollection<T>> Where(const std::function<bool(const T& item)> &handlerFunc) const 
    {
        return std::shared_ptr<ICollection<T>>(_WhereImpl(handlerFunc));
    }

private:
    virtual ICollection<T> *_MapImpl(const std::function<T(const T& item)> &handlerFunc) const = 0;
    virtual ICollection<T> *_WhereImpl(const std::function<bool(const T& item)> &handlerFunc) const = 0;
    template <class collection>
    static void AssertICollection() 
    {
        static_assert(std::is_base_of<ICollection<T>, collection>::value, "type parameter must derive from ICollection");
    }
};

template<template <class> class Derived, class T>
class InheritFromICollection : public ICollection<T>
{
public:
    std::shared_ptr<Derived<T>> Map(const std::function<T(const T& item)> &handlerFunc) const 
    {
        return std::shared_ptr<Derived<T>>(static_cast<Derived<T> *>(_MapImpl(handlerFunc)));
    }

    std::shared_ptr<Derived<T>> Where(const std::function<bool(const T& item)> &handlerFunc) const 
    {
        return std::shared_ptr<Derived<T>>(static_cast<Derived<T> *>(_WhereImpl(handlerFunc)));

    }

private:
    virtual InheritFromICollection *_MapImpl(const std::function<T(const T& item)> &handlerFunc) const = 0;
    virtual InheritFromICollection *_WhereImpl(const std::function<bool(const T& item)> &handlerFunc) const = 0;
};
