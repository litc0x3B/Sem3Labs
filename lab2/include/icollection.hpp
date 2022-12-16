#pragma once
#include <functional>
#include <string>

template<class T> class ICollection
{
public:
    virtual int GetSize() const = 0;
    virtual void Add(const T &item) = 0;
    virtual void Remove(const T &item) = 0;
    virtual bool Search(const T &item) const = 0;
    virtual bool Search(const T &item, T &foundItem) const = 0;

    virtual T Reduce(const std::function<T(const T&, const T&)> &handlerFunc, T identity) const = 0;
    virtual ICollection<T> *MapC(const std::function<T(const T& item)> &handlerFunc) const = 0;
    virtual ICollection<T> *WhereC(const std::function<bool(const T& item)> &handlerFunc) const = 0;

    virtual std::string ToStr(const std::function<std::string(const T&)> &itemToStr = static_cast<std::string(*)(T)>(std::to_string), 
                             std::string delim = " ") const = 0;
    virtual void AddFromStr(const std::function<T(const std::string&)> &strToItem, std::string str, std::string delim = " ") = 0;

};
