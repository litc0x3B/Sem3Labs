#pragma once
#include "sequence.hpp"
#include "linkedList.hpp"

template<typename T>
class LinkedListSequence : public Sequence<T>
{
private:
    LinkedList<T> *list;

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

    void Append(T item) override;
    void Prepend(T item) override;
    void PopBack() override;
    void PopFront() override;
    void Delete(int index) override; 
    void InsertAt(T item, int index) override;
    void Copy(const Sequence<T> *seq) override;

    Sequence<T> *Where(const std::function<bool(T)> &func) const override;
    Sequence<T> *Concat(Sequence <T> *list) const override;
    Sequence<T> *GetSubsequence(int startIndex, int endIndex) const override;
};

template<typename T>
LinkedListSequence<T>::~LinkedListSequence()
{
    delete list;
}

template<typename T>
LinkedListSequence<T>::LinkedListSequence (const T* items, int count)
{
    list = new LinkedList<T>(items, count); 
    Sequence<T>::sequenceType = SequenceType::list; 
}

template<typename T>
LinkedListSequence<T>::LinkedListSequence ()
{
    list = new LinkedList<T>();
    Sequence<T>::sequenceType = SequenceType::list; 
}

template<typename T>
LinkedListSequence<T>::LinkedListSequence (const LinkedListSequence <T> &list)
{
    this->list = new LinkedList<T>(*list.list);
    Sequence<T>::sequenceType = SequenceType::list; 
}

template<typename T>
T &LinkedListSequence<T>::GetFirst()
{
    return (*list)[0];
}

template<typename  T>
T &LinkedListSequence<T>::GetLast()
{
    return (*list)[list->GetSize() - 1];
}

template<typename T>
T &LinkedListSequence<T>::operator[](int index) const
{
    return (*list)[index];
}

template<typename T>
int LinkedListSequence<T>::GetSize() const
{
    return list->GetSize();
}

template<typename T>
int LinkedListSequence<T>::GetActualSize() const
{
    return list->GetSize();
}

template<typename T>
void LinkedListSequence<T>::Append(T item)
{
    list->Append(item);
}

template<typename T>
void LinkedListSequence<T>::Prepend(T item)
{
    list->Prepend(item);
}

template<typename T>
void LinkedListSequence<T>::PopBack()
{
    list->PopBack();
}

template<typename T>
void LinkedListSequence<T>::PopFront()
{
    list->PopFront();
}

template<typename T>
void LinkedListSequence<T>::Delete(int index)
{
    list->Delete(index);
}

template<typename T>
void LinkedListSequence<T>::InsertAt(T item, int index)
{
    list->InsertAt(item, index);
}

template<typename T>
void Sequence<T>::Copy(const Sequence<T> *seq)
{
    delete this->list;

    this->list = new LinkedList<T>;

    for (int i = 0; i < this->GetSize(); i++)
    {
        this->Append(seq->operator[](i));
    }
}

template<typename T>
Sequence<T> *LinkedListSequence<T>::Concat(Sequence<T> *sequence) const
{
    if (sequence->GetType() != SequenceType::list)
    {
        throw std::runtime_error("Sequence is not LinkedListSequence");
    }
    LinkedListSequence<T> *list = (LinkedListSequence<T> *)sequence;

    LinkedListSequence<T> *newList = new LinkedListSequence<T>(*this);
    (*newList->list) += (*list->list);

    return newList;
}

template<typename T>
Sequence<T> *LinkedListSequence<T>::GetSubsequence(int startIndex, int endIndex) const
{
    LinkedListSequence<T> *newList = new LinkedListSequence<T>;
    delete newList->list;
    newList->list = new LinkedList<T>(*this->list, startIndex, endIndex);
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







