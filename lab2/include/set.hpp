#pragma once
#include <functional>
#include <memory>
#include <system_error>

#include "binTree.hpp"
#include "icollection.hpp"

template <class T>
class Set : public InheritFromICollection<Set, T>
{
 private:
  std::shared_ptr<BinTree<T>> tree;

  Set *_MapImpl(const std::function<T(const T &item)> &handlerFunc) const override
  {
    auto *newSet = new Set<T>(GetComparerFunc());
    newSet->tree = tree->Map(handlerFunc);
    return newSet;
  }                                 

  Set *_WhereImpl(const std::function<bool(const T& item)> &handlerFunc) const override
  {
    auto *newSet = new Set<T>(GetComparerFunc());
    newSet->tree = tree->Where(handlerFunc);
    return newSet;
  }

 public:
  explicit Set(ComparerFunc<T> comparer = defaultCompFunc<T>) {tree = std::make_unique<BinTree<T>>(comparer);}

  explicit Set(const std::function<T(const std::string &)> &strToItem, const std::string &str,
               std::string delim = " ", ComparerFunc<T> comparer = defaultCompFunc<T>)
  {
    tree = std::make_unique<BinTree<T>>(strToItem, str, delim, comparer);
  }

  Set(const Set<T> &set) {tree = set.tree->Copy();}

  Set(const BinTree<T> &tree) {tree = tree->Copy();}


  Set<T> operator=(const Set<T> &set) { tree = set.tree->Copy();}

  ComparerFunc<T> GetComparerFunc() const { return tree->GetComparerFunc(); }
  int GetSize() const override { return tree->GetSize(); }

  void Add(const T &item) override { tree->Add(item); }
  void Remove(const T &item) override { tree->Remove(item); }
  bool Search(const T &item) const override { return tree->Search(item); }
  bool Search(const T &item, T &foundItem) const override { return tree->Search(item, foundItem); }

  T Reduce(const std::function<T(const T &, const T &)> &handlerFunc, T identity) const override
  {
    return tree->Reduce(handlerFunc, identity);
  }

  void AddFromStr(const std::function<T(const std::string &)> &strToItem, std::string str,
                  std::string delim = " ") override
  {
    tree->AddFromStr(strToItem, str, delim);
  }

  std::string ToStr(const std::function<std::string(const T &)> &itemToStr =
                        static_cast<std::string (*)(T)>(std::to_string),
                    std::string delim = " ") const override
  {
    return tree->ToStr(itemToStr, delim);
  }

  std::shared_ptr<Set<T>> Union(const std::shared_ptr<Set<T>> &set) const
  {
    Set<T> *newSet = new Set<T>(tree->GetComparerFunc());
    newSet->tree->InsertFromTree(set->tree);
    newSet->tree->InsertFromTree(tree);

    return std::shared_ptr<Set<T>>(newSet);
  }

  std::shared_ptr<Set<T>> Intersection(const std::shared_ptr<Set<T>> &set) const
  {
    Set<T> *newSet = new Set<T>(tree->GetComparerFunc());

    tree->TraverseConst([&set, newSet](const T &item) {
      if (set->tree->Search(item))
      {
        newSet->tree->Add(item);
      }
    });

    return std::shared_ptr<Set<T>>(newSet);
  }

  std::shared_ptr<Set<T>> Difference(const std::shared_ptr<Set<T>> &set) const
  {
    Set<T> *newSet = new Set<T>(tree->GetComparerFunc());

    tree->TraverseConst([&set, newSet](const T &item) {
      if (!set->tree->Search(item))
      {
        newSet->tree->Add(item);
      }
    });

    return std::shared_ptr<Set<T>>(newSet);
  }

  bool Has(const Set &set) const
  {
    if (GetSize() < set.GetSize())
    {
      return false;
    }

    bool subset = true;

    set.tree->TraverseConst([&subset, this](const T &item) {
      if (!tree->Search(item))
      {
        subset = false;
      }
    });

    return subset;
  }

  bool IsEqualTo(const std::shared_ptr<Set<T>> &set) {
    return *this == *set;
  }

  bool operator==(const Set &set) const
  {
    if (set.GetSize() != GetSize())
    {
      return false;
    }

    bool equal = true;

    tree->TraverseConst([&equal, &set](const T &item) {
      if (!set.tree->Search(item))
      {
        equal = false;
      }
    });

    return equal;
  }

  operator BinTree<T>() { return tree; }

  void ForEach(const std::function<void(const T &)> &handlerFunc) const override { tree->ForEach(handlerFunc); }

  bool operator!=(const Set &set) const { return !std::operator==(set); }
};
