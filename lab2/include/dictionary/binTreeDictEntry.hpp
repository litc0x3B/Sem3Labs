#pragma once

template <class TKey, class TValue>
using GetKeyFunc = std::function<TKey(const TValue &)>;
template <class TKey>
using CompKeysFunc = std::function<int(const TKey &, const TKey &)>;

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
class BinTreeDictEntryKeyAndValue : public BinTreeDictEntry<TKey>
{
 private:
  TKey key;
  TValue value;

 public:
  BinTreeDictEntryKeyAndValue(TKey key, TValue value) : key(key), value(value) {}
  TValue GetValue() const {return value;}
  TValue &GetValueNonConst()  {return value;}
  TKey GetKey() const override {return key;}
};

