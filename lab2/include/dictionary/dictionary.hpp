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
  using DictTree = BinTree<BinTreeDictEntry<TKey> *>;
  using EntryBase = BinTreeDictEntry<TKey>;
  using Entry = BinTreeDictEntryKeyAndValue<TKey, TValue>;
  using EntryKey = BinTreeDictEntryOnlyKey<TKey>;

  Dictionary *_CopyImpl() const override { return new Dictionary(*this); };

  const CompKeysFunc<TKey> cmp;
  std::shared_ptr<DictTree> binTree;

 public:
  explicit Dictionary(const CompKeysFunc<TKey> &cmp = defaultCompFunc<TKey>) : cmp(cmp)
  {
    binTree = std::make_shared<DictTree>([cmp](const EntryBase *entry1, const EntryBase *entry2)
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

  Dictionary(const std::initializer_list<DictEntry<TKey, TValue>> &list) : Dictionary(defaultCompFunc<TKey>, list) {}

  Dictionary(const Dictionary &dict) : Dictionary(dict.GetComparerFunc())
  {
    dict.ForEach([this](const TKey &key, const TValue &value) { this->Add(key, value); });
  }

  ~Dictionary()
  {
    binTree->ForEach([](const EntryBase *entry) { delete entry; });
  }

  Dictionary &operator=(const Dictionary &dict) = delete;

  std::shared_ptr<Dictionary> Copy() const { return std::shared_ptr<Dictionary>(_CopyImpl()); }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"
  Nullable<TValue> Get(TKey key) const override
  {
    auto *entry = new EntryKey(key);
    EntryBase *found = nullptr;
    binTree->Search(entry, found);
    delete entry;

    if (found == nullptr)
    {
      return Nullable<TValue>::Null;
    }

    TValue retValue = dynamic_cast<Entry *>(found)->GetValue();
    return retValue;
  }
#pragma clang diagnostic pop

  void ForEach(const std::function<void(const TValue &)> &func) const override
  {
    binTree->ForEach(
        [func](const EntryBase *entryBase)
        {
          auto *entry = dynamic_cast<const Entry *>(entryBase);
          func(entry->GetValue());
        });
  }

  void ForEach(const std::function<void(const TKey &key, const TValue &value)> &func) const
  {
    binTree->ForEach(
        [func](const EntryBase *entryBase)
        {
          auto *entry = dynamic_cast<const Entry *>(entryBase);
          func(entry->GetKey(), entry->GetValue());
        });
  }

  bool Add(const TKey &key, const TValue &value)
  {
    auto *entry = new Entry(key, value);
    if (binTree->Add(entry))
    {
      return true;
    }

    delete entry;
    return false;
  }

  Nullable<TValue> Remove(TKey key) override
  {
    auto *entry = new EntryKey(key);
    Nullable<EntryBase *> removedEntry = binTree->Remove(entry);
    delete entry;

    if (!removedEntry.IsNull())
    {
      TValue foundValue = dynamic_cast<Entry *>(removedEntry.GetValue())->GetValue();
      delete removedEntry.GetValue();
      return foundValue;
    }

    else
      return Nullable<TValue>::Null;
  }

  CompKeysFunc<TKey> GetComparerFunc() const { return cmp; }
  int Size() const override { return binTree->GetSize(); }
};
