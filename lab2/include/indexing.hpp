#pragma once
#include <functional>
#include <initializer_list>
#include <memory>
#include <string>

#include "binTree.hpp"
#include "iDictionary.hpp"
#include "icollection.hpp"
#include "sequence/dynamicArraySequence.hpp"
#include "set.hpp"

template <class TKey>
class BinTreeDictEntry;

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

  // for new[] only better not use in other situations
  AttributeInfo() {}

  int CompareKeys(const TKey &key1, const TKey &key2) const { return compKeys(key1, key2); }
  std::string GetName() const { return name; }

  bool operator==(const AttributeInfo &attrInfo) const { return this->name == attrInfo.name; }
  bool operator!=(const AttributeInfo &attrInfo) const { return !(attrInfo == *this); }
};

template <class TKey>
class BinTreeDictEntry
{
 public:
  virtual TKey GetKey() const = 0;
  virtual ~BinTreeDictEntry(){};
};

template <class TKey, class TValue>
class BinTreeDictEntryOnlyValue : public BinTreeDictEntry<TKey>
{
 private:
  GetKeyFunc<TKey, TValue> getKey;
  TValue value;

 public:
  BinTreeDictEntryOnlyValue(const GetKeyFunc<TKey, TValue> &getKey, TValue value)
  {
    this->getKey = getKey;
    this->value = value;
  }

  TKey GetKey() const override { return getKey(value); }
  TValue GetValue() const { return value; }
};

template <class TKey>
class BinTreeDictEntryOnlyKey : public BinTreeDictEntry<TKey>
{
 private:
  TKey key;

 public:
  BinTreeDictEntryOnlyKey(TKey key) { this->key = key; }
  TKey GetKey() const override { return key; }
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
    this->attributes = new DynamicArraySequence<AttributeInfo<TKey>>(*attrs.attributes);
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

  int Compare(const BinTreeDictEntry<TKey> *entry1, const BinTreeDictEntry<TKey> *entry2) const
  {
    TKey key1 = entry1->GetKey();
    TKey key2 = entry2->GetKey();

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

  TKey GetKey(const TValue &value) const { return getKey(value); }

  ~AttributesInfo() { delete attributes; }
};

template <class TKey, class TValue>
class IndexedDict : public IDictionary<TKey, TValue>
{
 private:
  AttributesInfo<TKey, TValue> attributes;
  std::shared_ptr<BinTree<BinTreeDictEntry<TKey> *>> binTree;

 public:
  IndexedDict(AttributesInfo<TKey, TValue> attributes) : attributes(attributes)
  {
    binTree = std::shared_ptr<BinTree<BinTreeDictEntry<TKey> *>>(
        new BinTree<BinTreeDictEntry<TKey> *>([attributes](const BinTreeDictEntry<TKey> *entry1,
                                                           const BinTreeDictEntry<TKey> *entry2) {
          return attributes.Compare(entry1, entry2);
        }));
  }

  IndexedDict(AttributesInfo<TKey, TValue> attributes, std::shared_ptr<Set<TValue>> collection)
      : IndexedDict(attributes)
  {
    collection->ForEach([this](TValue value) { this->Add(value); });
  }

  template <class Container>
  IndexedDict(AttributesInfo<TKey, TValue> attributes, Container container)
      : IndexedDict(attributes)
  {
    for (const TValue &value : container)
    {
      Add(value);
    }
  }

  IndexedDict(const IndexedDict &dict) : attributes(dict.attributes)
  {
    *this->binTree = *dict.binTree;
  }

  ~IndexedDict()
  {
    binTree->ForEach([](const BinTreeDictEntry<TKey> *entry) { delete entry; });
  }

  IndexedDict operator=(const IndexedDict &dict)
  {
    this->attributes = dict.attributes;
    *this->attributes = *dict.binTree;
  }

  int Size() const override { return binTree->GetSize(); }

  Nullable<TValue> Get(TKey key) const override
  {
    BinTreeDictEntry<TKey> *foundEntry;
    auto *entryKey = new BinTreeDictEntryOnlyKey<TKey>(key);

    if (binTree->Search(entryKey, foundEntry))
    {
      delete entryKey;
      return dynamic_cast<BinTreeDictEntryOnlyValue<TKey, TValue> *>(foundEntry)->GetValue();
    }

    delete entryKey;
    return Nullable<TValue>::Null;
  };

  std::shared_ptr<IndexedDict> GetInRange(const TKey &min, const TKey &max)
  {
    auto *entryMin = new BinTreeDictEntryOnlyKey<TKey>(min);
    auto *entryMax = new BinTreeDictEntryOnlyKey<TKey>(max);
    std::shared_ptr<BinTree<BinTreeDictEntry<TKey> *>> found =
        binTree->GetInRange(entryMin, entryMax);

    auto *newDict = new IndexedDict(attributes);
    found->ForEach([newDict](BinTreeDictEntry<TKey> *const& entry) 
    {

      TValue value = dynamic_cast<const BinTreeDictEntryOnlyValue<TKey, TValue>*>(entry)->GetValue();
      newDict->Add(value);
    });

    return std::shared_ptr<IndexedDict>(newDict);
  }

  
  void ForEach(const std::function<void(const TValue &)> &handlerFunc) 
  {
    binTree->ForEach([handlerFunc](const BinTreeDictEntry<TKey> *entry) 
    {
      TValue value = dynamic_cast<const BinTreeDictEntryOnlyValue<TKey, TValue> *>(entry)->GetValue();
      handlerFunc(value);
    });
  }

  void Remove(TKey key) override
  {
    auto *entry = new BinTreeDictEntryOnlyKey<TKey>(key);
    binTree->Remove(entry);
    delete entry;
  }

  void Add(TValue value)
  {
    auto *dictEntry = new BinTreeDictEntryOnlyValue<TKey, TValue>(
        [this](const TValue &value) { return attributes.GetKey(value); }, value);

    binTree->Add(dictEntry);
  }
};
