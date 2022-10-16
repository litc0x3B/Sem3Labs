#pragma once
#include "sequence.hpp"
#include "linkedList.hpp"

template<typename T>
class LinkedListSequence : public Sequence<T>
{
private:
    LinkedList<T> *storage;

public:
    LinkedListSequence(const T* items, int count);
    LinkedListSequence();
    LinkedListSequence(const LinkedListSequence <T> &list);
    ~LinkedListSequence();

    T &GetFirst() override;
    T &GetLast() override;
    int GetActualSize() const override;
    int GetSize() const override;

    T &operator[](int index) const override;

    LinkedList<T> *GetStorage();
    void Append(T item) override;
    void Prepend(T item) override;
    void PopBack() override;
    void PopFront() override;
    void Delete(int index) override; 
    void InsertAt(T item, int index) override;
    void Clear() override;

    Sequence<T> *Copy(const Sequence<T> *seq) override;
    Sequence<T> *Where(const std::function<bool(T)> &func) const override;
    Sequence<T> *Concat(Sequence <T> *list) const override;
    Sequence<T> *GetSubsequence(int startIndex, int endIndex) const override;
};

template<typename T>
LinkedList<T> *LinkedListSequence<T>::GetStorage()
{
    return storage;
}

template<typename T>
void LinkedListSequence<T>::Clear()
{
    delete storage;
    storage = new LinkedList<T>();
}

template<typename T>
LinkedListSequence<T>::~LinkedListSequence()
{
    delete storage;
}

template<typename T>
LinkedListSequence<T>::LinkedListSequence (const T* items, int count)
{
    storage = new LinkedList<T>(items, count); 
}

template<typename T>
LinkedListSequence<T>::LinkedListSequence ()
{
    storage = new LinkedList<T>();
}

template<typename T>
LinkedListSequence<T>::LinkedListSequence (const LinkedListSequence <T> &list)
{
    this->storage = new LinkedList<T>(*list.storage);
}

template<typename T>
T &LinkedListSequence<T>::GetFirst()
{
    return (*storage)[0];
}

template<typename  T>
T &LinkedListSequence<T>::GetLast()
{
    return (*storage)[storage->GetSize() - 1];
}

template<typename T>
T &LinkedListSequence<T>::operator[](int index) const
{
    return (*storage)[index];
}

template<typename T>
int LinkedListSequence<T>::GetSize() const
{
    return storage->GetSize();
}

template<typename T>
int LinkedListSequence<T>::GetActualSize() const
{
    return storage->GetSize();
}

template<typename T>
void LinkedListSequence<T>::Append(T item)
{
    storage->Append(item);
}

template<typename T>
void LinkedListSequence<T>::Prepend(T item)
{
    storage->Prepend(item);
}

template<typename T>
void LinkedListSequence<T>::PopBack()
{
    storage->PopBack();
}

template<typename T>
void LinkedListSequence<T>::PopFront()
{
    storage->PopFront();
}

template<typename T>
void LinkedListSequence<T>::Delete(int index)
{
    storage->Delete(index);
}

template<typename T>
void LinkedListSequence<T>::InsertAt(T item, int index)
{
    storage->InsertAt(item, index);
}

template<typename T>
Sequence<T> *LinkedListSequence<T>::Copy(const Sequence<T> *seq)
{
    delete this->storage;

    this->storage = new LinkedList<T>;

    for (int i = 0; i < seq->GetSize(); i++)
    {
        this->Append(seq->operator[](i));
    }

    return this;
}

template<typename T>
Sequence<T> *LinkedListSequence<T>::Concat(Sequence<T> *sequence) const
{
    LinkedListSequence<T> *newList = new LinkedListSequence<T>(*this);

    for (int i = 0; i < sequence->GetSize(); i++)
    {
        newList->Append(sequence->At(i));
    }

    return newList;
}

template<typename T>
Sequence<T> *LinkedListSequence<T>::GetSubsequence(int startIndex, int endIndex) const
{
    LinkedListSequence<T> *newList = new LinkedListSequence<T>;
    delete newList->storage;
    newList->storage = new LinkedList<T>(*this->storage, startIndex, endIndex);
    return (Sequence<T>*)newList;
}

template<typename T>
Sequence<T> *LinkedListSequence<T>::Where(const std::function<bool(T)> &func) const
{
    LinkedListSequence *newList = new LinkedListSequence<T>;

    for (int i = 0; i < this->GetSize(); i++)
    {
        if (func(this->operator[](i)))
        {
            newList->Append(this->operator[](i));
        }
    }

    return newList;
}







