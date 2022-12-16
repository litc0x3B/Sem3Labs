#pragma once
#include "binTree.hpp"
#include "icollection.hpp"
#include <functional>
#include <system_error>

template<class T> class Set : public ICollection<T>
{
private: 
    BinTree<T> tree;

public:
    explicit Set(ComparerFunc<T> comparer = defaultCompFunc<T>)
    {
        tree = BinTree<T>(comparer);
    }

    explicit Set(const std::function<T(const std::string&)> &strToItem, const std::string &str, std::string delim = " ", ComparerFunc<T> comparer = defaultCompFunc<T>)
    {
        tree = BinTree<T>(strToItem, str, delim, comparer);
    }

    Set(const Set<T> &set)
    {
        tree = set.tree;
    }

    Set<T> operator=(const Set<T> &set)
    {
        tree = set.tree;
    }

    ComparerFunc<T> GetComparerFunc() const {return tree.GetComparerFunc();}
    int GetSize() const override {return tree.GetSize();}

    void Add(const T &item) override {tree.Add(item);}
    void Remove(const T &item) override {tree.Remove(item);}
    bool Search(const T &item) const override {return tree.Search(item);}
    bool Search(const T &item, T &foundItem) const override {return tree.Search(item, foundItem);}

    Set<T> Map(const std::function<T(const T& item)> &handlerFunc) const 
    {
        Set<T> newSet(GetComparerFunc());
        newSet.tree = tree.Map(handlerFunc);
        return newSet;
    }

    Set<T> *MapC(const std::function<T(const T& item)> &handlerFunc) const override
    {
        auto newSet = new Set<T> (GetComparerFunc());
        newSet->tree = tree.Map(handlerFunc);
        return newSet;
    }

    Set<T> Where(const std::function<bool(const T& item)> &handlerFunc) const
    {
        Set<T> newSet(GetComparerFunc());
        newSet.tree = tree.Where(handlerFunc);
        return newSet;
    }

    Set<T> *WhereC(const std::function<bool(const T& item)> &handlerFunc) const override
    {
        auto newSet = new Set<T> (GetComparerFunc());
        newSet->tree = tree.Where(handlerFunc);
        return newSet;
    }

    T Reduce(const std::function<T(const T&, const T&)> &handlerFunc, T identity) const override {return tree.Reduce(handlerFunc, identity);} 

    void AddFromStr(const std::function<T(const std::string&)> &strToItem, std::string str, std::string delim = " ") override {tree.AddFromStr(strToItem, str, delim);}

    std::string ToStr(const std::function<std::string(const T&)> &itemToStr = static_cast<std::string(*)(T)>(std::to_string), std::string delim = " ") const override
    {
        return tree.ToStr(itemToStr, delim);
    }

    Set<T> Union(const Set &set) const
    {
        Set<T> newSet(tree.GetComparerFunc());
        newSet.tree.InsertFromTree(set.tree);
        newSet.tree.InsertFromTree(tree);

        return newSet;
    }

    Set<T> Intersection(const Set &set) const
    {
        Set<T> newSet(tree.GetComparerFunc());

        tree.TraverseConst([&set, &newSet](const T &item)
        {
            if (set.tree.Search(item))
            {
                newSet.tree.Add(item);
            }
        });

        return newSet;
    }

    Set<T> Difference(const Set &set) const
    {
        Set<T> newSet(tree.GetComparerFunc());

        tree.TraverseConst([&set, &newSet](const T &item)
        {
            if (!set.tree.Search(item))
            {
                newSet.tree.Add(item);
            }
        });

        return newSet; 
    }

    bool Has(const Set &set) const
    {
        if (GetSize() < set.GetSize())
        {
            return false;
        }

        bool subset = true;

        set.tree.TraverseConst([&subset, this](const T &item)
        {
            if (!tree.Search(item))
            {
                subset = false;
            }
        });

        return subset;   
    }

    Set<T> operator+(const Set &set) const {return Union(set);}
    Set<T> operator*(const Set &set) const {return Intersection(set);}
    Set<T> operator-(const Set &set) const {return Difference(set);}

    bool operator==(const Set &set) const
    {
        if (set.GetSize() != GetSize())
        {
            return false;
        }

        bool equal = true;

        tree.TraverseConst([&equal, &set](const T &item)
        {
            if (!set.tree.Search(item))
            {
                equal = false;
            }
        });

        return equal;
    }

    operator BinTree<T>() {
        return tree;
    }

    void Traverse(const std::function<void(T&)> &handlerFunc) {
        tree.Traverse(handlerFunc, "NLR");
    }

    bool operator!=(const Set &set) const
    {
        return !std::operator==(set);
    }
};
