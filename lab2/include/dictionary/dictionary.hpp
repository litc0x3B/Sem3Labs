#pragma once
#include "binTree.hpp"
#include "binTreeDictEntry.hpp"
#include "iDictionary.hpp"

template <class TKey, class TValue>
struct DictEntry
{
  TKey key;
  TValue value;
};

template <class TKey, class TValue>
class Dictionary : public IDictionary<TKey, TValue>
{
 private:
  using DictTree = BinTree<std::shared_ptr<BinTreeDictEntry<TKey>>>;
  using EntryBase = BinTreeDictEntry<TKey>;
  using Entry = BinTreeDictEntryKeyAndValue<TKey, TValue>;
  using EntryKey = BinTreeDictEntryOnlyKey<TKey>;

  Dictionary *_CopyImpl() const override { return new Dictionary(*this); };
  const CompKeysFunc<TKey> cmp;
  std::shared_ptr<DictTree> binTree;

 public:
  Dictionary(const CompKeysFunc<TKey> &cmp) : cmp(cmp)
  {
    binTree = std::make_shared<DictTree>(
        [cmp](const std::shared_ptr<EntryBase> &entry1,
              const std::shared_ptr<EntryBase> &entry2)
        { return cmp(entry1->GetKey(), entry2->GetKey()); });
  }

  Dictionary(const CompKeysFunc<TKey> &cmp,
             const std::initializer_list<DictEntry<TKey, TValue>> &list)
      : Dictionary(cmp)
  {
    for (auto entry : list)
    {
      this->Add(entry.key, entry.value);
    }
  }

  Dictionary(const Dictionary &dict) : Dictionary(dict.GetComparerFunc())
  {
    this->binTree = dict.binTree->Copy();
  }

  std::shared_ptr<Dictionary> Copy() const
  {
    return std::shared_ptr<Dictionary>(_CopyImpl());
  }

  Nullable<TValue> Get(TKey key) const override
  {
    auto entry = std::make_shared<EntryKey>(key);
    std::shared_ptr<EntryBase> found;
    binTree->Search(entry, found);
    if (found == nullptr)
    {
      return Nullable<TValue>::Null;
    }
    return std::dynamic_pointer_cast<Entry>(found)->GetValue();
  }

  void ForEach(const std::function<void(const TValue &)> &func) const override
  {
    binTree->ForEach(
        [func](const std::shared_ptr<EntryBase> &entryBase)
        {
          auto entry = std::dynamic_pointer_cast<Entry>(entryBase);
          func(entry->GetValue());
        });
  }

  void ForEach(
      const std::function<void(const TKey &key, const TValue &value)> &func) const
  {
    binTree->ForEach(
        [func](const std::shared_ptr<EntryBase> &entryBase)
        {
          auto entry = std::dynamic_pointer_cast<Entry>(entryBase);
          func(entry->GetKey(), entry->GetValue());
        });
  }

  void Add(const TKey &key, const TValue &value)
  {
    auto entry = std::make_shared<Entry>(key, value);
    binTree->Add(entry);
  }

  Nullable<TValue> Remove(TKey key) override
  {
    auto entry = std::make_shared<EntryKey>(key);
    auto removedEntry = binTree->Remove(entry);
    if (!removedEntry.IsNull())
    {
      return std::dynamic_pointer_cast<Entry>(removedEntry.GetValue())
          ->GetValue();
    }

    else
      return Nullable<TValue>::Null;
  }

  CompKeysFunc<TKey> GetComparerFunc() const { return cmp; }
  int Size() const override { return binTree->GetSize(); }
};
