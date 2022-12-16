#pragma once
#include <functional>
#include <initializer_list>
#include <memory>
#include <string>

#include "binTree.hpp"
#include "iDictionary.hpp"
#include "sequence/dynamicArraySequence.hpp"
#include "set.hpp"

template <class TKey>
using CompKeysFunc = std::function<int(const TKey &, const TKey &)>;
template <class TKey, class TValue>
using GetKeyFunc = std::function<TKey(const TValue &)>;

template <class TKey>
class AttributeInfo
{
 private:
  CompKeysFunc<TKey> compKeys;
  std::string name;

 public:
  AttributeInfo(std::string name, CompKeysFunc<TKey> compKeysFunc)
  {
    this->compKeys = compKeysFunc;
    this->name = name;
  }

  // created only for new[] better not use in other situations
  AttributeInfo() {}

  int CompareKeys(const TKey &key1, const TKey &key2)
  {
    return compKeys(key1, key2);
  }
  std::string GetName() { return name; }

  bool operator== (const AttributeInfo &attrInfo) {return this->name == attrInfo.name;}
  bool operator!= (const AttributeInfo &attrInfo) {return !(attrInfo == *this);}
};

template <class TKey>
class BinTreeDictEntry
{
 public:
  virtual TKey GetKey() = 0;
};

template <class TKey, class TValue>
class BinTreeDictEntryOnlyValue : public BinTreeDictEntry<TKey>
{
 private:
  GetKeyFunc<TKey, TValue> getKey;
  TValue value;

 public:
  BinTreeDictEntryOnlyValue(const GetKeyFunc<TKey, TValue> &getKey,
                            TValue value)
  {
    this->getKey = getKey;
    this->value = value;
  }

  TKey GetKey() override { return getKey(value); }
  TValue GetValue() { return value; }
};

template <class TKey>
class BinTreeDictEntryOnlyKey : public BinTreeDictEntry<TKey>
{
 private:
  TKey key;

 public:
  BinTreeDictEntryOnlyKey(TKey key) { this->key = key; }
  TKey GetKey() override { return key; }
};

template <class TKey, class TValue>
class AttributesInfo
{
 private:
  DynamicArraySequence<AttributeInfo<TKey>> *attributes = nullptr;
  GetKeyFunc<TKey, TValue> getKey;

 public:
  AttributesInfo(const AttributesInfo &attrs)
  {
    this->attributes =
        new DynamicArraySequence<AttributeInfo<TKey>>(*attrs.attributes);
    this->getKey = attrs.getKey;
  }

  AttributesInfo operator=(const AttributesInfo &attrs)
  {
    this->attributes->Copy(attrs.attributes);
    this->getKey = attrs.getKey;
  }

  AttributesInfo(const std::initializer_list<AttributeInfo<TKey>> &attributes,
                 GetKeyFunc<TKey, TValue> getKey)
  {
    this->attributes = new DynamicArraySequence<AttributeInfo<TKey>>;
    for (const auto &attr : attributes)
    {
      this->attributes->Append(attr);
    }

    this->getKey = getKey;
  }

  int Compare(const BinTreeDictEntry<TKey> *entry1,
              const BinTreeDictEntry<TKey> *entry2)
  {
    TValue key1 = entry1->GetKey();
    TValue key2 = entry2->GetKey();

    for (int i = 0; i < attributes->GetSize(); i++)
    {
      if (attributes->At(i).CompareKeys(key1, key2) > 0)
      {
        return 1;
      }
      else if (attributes->At(i).CompareKeys(key1, key2) < 0)
      {
        return -1;
      }
    }
    return 0;
  }

  ~AttributesInfo() { delete attributes; }
};

template <class TKey, class TValue>
class IndexedDict : public IDictionary<TKey, TValue>
{
 private:
  AttributesInfo<TKey, TValue> attributes;
  BinTree<std::unique_ptr<BinTreeDictEntry<TKey>>> *binTree;

  std::unique_ptr<BinTreeDictEntry<TKey>> _ToKeyTreeEntry(TKey key)
  {
    return std::move(std::unique_ptr<BinTreeDictEntry<TKey>>(
        new BinTreeDictEntryOnlyKey<TKey>(key)));
  }

 public:
  ~IndexedDict() { delete binTree; }
  IndexedDict(AttributesInfo<TKey, TValue> attributes)
  {
    this->attributes = attributes;
  }

  IndexedDict(AttributesInfo<TKey, TValue> attributes, Set<TValue> *set)
      : IndexedDict(attributes)
  {
    set->Traverse([this](TValue value) { this->Add(value); });
  }

  Nullable<TValue> Get(TKey key) override
  {
    std::unique_ptr<BinTreeDictEntry<TKey>> foundEntry;
    if (binTree->Search(_ToKeyTreeEntry(key), foundEntry))
    {
      return dynamic_cast<BinTreeDictEntryOnlyValue<TKey, TValue> *>(
                 foundEntry.release())
          ->GetValue();
    }

    return Nullable<TValue>::Null();
  };

  void Remove(TKey key) override { binTree->Remove(_ToKeyTreeEntry(key)); }

  void Add(TValue value)
  {
    binTree->Add(std::unique_ptr<BinTreeDictEntry<TKey>>(
        new BinTreeDictEntryOnlyValue<TKey, TValue>(value,
                                                    attributes.Compare())));
  }
};
