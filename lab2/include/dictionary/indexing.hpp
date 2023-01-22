#pragma once
#include <functional>
#include <initializer_list>
#include <memory>
#include <string>

#include "binTree.hpp"
#include "iDictionary.hpp"
#include "icollection.hpp"
#include "nullable.hpp"
#include "sequence/dynamicArraySequence.hpp"
#include "set.hpp"
#include "binTreeDictEntry.hpp"


template <class TKey>
class AttributeInfo
{
 private:
  CompKeysFunc<TKey> compKeys;
  std::string name;

 public:
  AttributeInfo(const std::string &name, CompKeysFunc<TKey> compKeysFunc)
  {
    this->compKeys = compKeysFunc;
    this->name = name;
  }

  // for new[] only better not use in other situations
  AttributeInfo() {}

  int CompareKeys(const TKey &key1, const TKey &key2) const { return compKeys(key1, key2); }

  [[nodiscard]]
  std::string GetName() const { return name; }


  bool operator==(const AttributeInfo &attrInfo) const { return this->name == attrInfo.name; }
  bool operator!=(const AttributeInfo &attrInfo) const { return !(attrInfo == *this); }
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

  AttributesInfo &operator=(const AttributesInfo &attrs)
  {
    if (&attrs == this)
    {
      return *this;
    }

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

  AttributesInfo(const Sequence<AttributeInfo<TKey>> *seq, GetKeyFunc<TKey, TValue> getKey)
  {
    this->attributes = (new DynamicArraySequence<AttributeInfo<TKey>>)->Copy(seq);
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

  DynamicArraySequence<AttributeInfo<TKey>> *GetAttributes() const
  {
    return (new DynamicArraySequence<AttributeInfo<TKey>>)->Copy(attributes);
  }

  [[nodiscard]]
  int NumberOfAttrs() const { return attributes->GetSize(); }

  ~AttributesInfo() { delete attributes; }
};

// template <class TKey, class TValue>
// class IndexedDict;

// template <class TKey, class TValue>
// void debugFunc(std::shared_ptr<BinTree<BinTreeDictEntry<TKey> *>> binTree)
// {
//   binTree->ForEach([](const BinTreeDictEntry<TKey> *entry) {
//     TValue value = dynamic_cast<const BinTreeDictEntryOnlyValue<TKey, TValue> *>(entry)->GetValue();
//     std::cout << value.testName << " " << value.age << " " << entry << std::endl;
//   });
//   std::cout << "***********************************" << std::endl;
// }

template <class TKey, class TValue>
class IndexedDict : public IDictionary<TKey, TValue>
{
 private:
  using DictTree = BinTree<BinTreeDictEntry<TKey> *>;
  using Entry = BinTreeDictEntry<TKey>;
  using EntryValue = BinTreeDictEntryOnlyValue<TKey, TValue>;
  using EntryKey = BinTreeDictEntryOnlyKey<TKey>;

  AttributesInfo<TKey, TValue> attributes;
  std::shared_ptr<DictTree> binTree;

  IndexedDict *_CopyImpl() const override
  {
    return new IndexedDict(*this);
  }

  static std::shared_ptr<DictTree> _NewBinTree(const AttributesInfo<TKey, TValue> &attributes)
  {
    return std::shared_ptr<DictTree>(
        new DictTree([attributes](const Entry *entry1, const Entry *entry2) {
          return attributes.Compare(entry1, entry2);
        }));
  }

  IndexedDict(AttributesInfo<TKey, TValue> attributes, std::shared_ptr<DictTree> binTree)
      : IndexedDict(attributes)
  {
    binTree->ForEach([this](const Entry *entry)
    {
      this->Add(dynamic_cast<const EntryValue*>(entry)->GetValue());
    });
  }

 public:
  explicit IndexedDict(AttributesInfo<TKey, TValue> attributes) : attributes(attributes)
  {
    binTree = _NewBinTree(attributes);
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
    this->binTree = _NewBinTree(dict.attributes);
    dict.ForEach([this](const TValue &value) { this->Add(value); });
  }

  ~IndexedDict()
  {
    binTree->ForEach([](const Entry *entry) { delete entry; });
  }

  IndexedDict &operator=(const IndexedDict &dict)
  {
    this->binTree = _NewBinTree(dict.attributes);
    dict.ForEach([this](const TValue &value) { this->Add(value); });
  }

  [[nodiscard]]
  int Size() const override { return binTree->GetSize(); }

  Nullable<TValue> Get(TKey key) const override
  {
    Entry *foundEntry;
    auto *entryKey = new EntryKey(key);

    if (binTree->Search(entryKey, foundEntry))
    {
      delete entryKey;
      return dynamic_cast<EntryValue *>(foundEntry)->GetValue();
    }

    delete entryKey;
    return Nullable<TValue>::Null;
  };

  std::shared_ptr<IndexedDict> GetInRange(const TKey &min, const TKey &max) const
  {
    auto *entryMin = new EntryKey(min);
    auto *entryMax = new EntryKey(max);
    // debugFunc<TKey, TValue>(this->binTree);

    std::shared_ptr<DictTree> newTree = this->binTree;
    for (int i = 0; i < attributes.NumberOfAttrs(); i++)
    {
      newTree = newTree->GetInRange(entryMin, entryMax, [i, this](const Entry* entry1, const Entry* entry2)
      {
        TKey key1 = entry1->GetKey();
        TKey key2 = entry2->GetKey();

        DynamicArraySequence<AttributeInfo<TKey>> *attrsArr = attributes.GetAttributes();

        int ret = attrsArr->At(i).CompareKeys(key1, key2);
        delete attrsArr;
        return ret;
      });
      //  debugFunc<TKey, TValue>(newTree);
    }

    delete entryMin;
    delete entryMax;

    return std::shared_ptr<IndexedDict>(new IndexedDict(attributes, newTree));
  }

  std::shared_ptr<IndexedDict> Copy() const { return std::shared_ptr<IndexedDict> (_CopyImpl()); }

  void ForEach(const std::function<void(const TValue &)> &handlerFunc) const override
    {
      binTree->ForEach([handlerFunc](const Entry *entry) {
        TValue value = dynamic_cast<const EntryValue *>(entry)->GetValue();
        handlerFunc(value);
      });
    }

  Nullable<TValue> Remove(TKey key) override
  {
    auto *entry = new EntryKey(key);
    Nullable<Entry *> removedEntry = binTree->Remove(entry);
    delete entry;

    if (!removedEntry.IsNull())
    {
      TValue removedValue = dynamic_cast<EntryValue*> (removedEntry.GetValue())->GetValue();
      delete removedEntry.GetValue();
      return removedValue;
    }

    return Nullable<TValue>::Null;
  }

  //TODO: Fix leak and change return value
  void Add(TValue value)
  {
    auto *dictEntry =
        new EntryValue([this](const TValue &value) { return attributes.GetKey(value); }, value);

    binTree->Add(dictEntry);
  }

  [[nodiscard]]
  AttributesInfo<TKey, TValue> GetAttrsInfo() { return attributes; }
};
