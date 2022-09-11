#pragma once

#include <functional>
#include <stdexcept>

enum SequenceType
    {
        undefined,
        list,
        array,
    };

template<typename T>
class Sequence
{
protected:
    SequenceType sequenceType = undefined;
public:
    virtual ~Sequence() {};

    SequenceType GetType();
    virtual void Map(const std::function<void(T&)> &func);
    virtual void Map(const std::function<void(T&)> &func, int startIndex, int endIndex);
    virtual int Find(Sequence<T> *sequence, int startIndex = 0) const;
    virtual int Find(T item, int startIndex = 0) const;
    
    virtual T Reduce(const std::function<T(T prev, T current)> &func) const;

    virtual T &GetFirst() = 0;
    virtual T &GetLast() = 0;
    virtual int GetActualSize() const = 0;
    virtual int GetSize() const = 0;

    virtual T &operator[](int index) const = 0;

    virtual void Append(T item) = 0;
    virtual void Prepend(T item) = 0;
    virtual void PopBack() = 0;
    virtual void PopFront() = 0;
    virtual void Delete(int index) = 0;
    virtual void InsertAt(T item, int index) = 0;
    virtual void Copy(const Sequence<T> *seq) = 0;

    virtual Sequence<T> *Where(const std::function<bool(T)> &func) const = 0;
    virtual Sequence<T> *Concat(Sequence <T> *sequence) const = 0;
    virtual Sequence<T> *GetSubsequence(int startIndex, int endIndex) const = 0;
};


template <typename T>
SequenceType Sequence<T>::GetType()
{
    return this->sequenceType;
}

template <typename T>
void Sequence<T>::Map(const std::function<void(T&)> &func)
{
    for (int i = 0; i < this->GetSize(); i++)
    {
        func(this->operator[](i));
    }
}

template <typename T>
void Sequence<T>::Map(const std::function<void(T&)> &func, int startIndex, int endIndex)
{
    if (startIndex > endIndex || startIndex < 0 || endIndex >= this->GetSize())
    {
        throw std::out_of_range("indices are out of range");
    }
    for (int i = startIndex; i <= endIndex; i++)
    {
        func(this->operator[](i));
    }
}

template<class T>
int Sequence<T>::Find(Sequence<T> *sequence, int startIndex) const
{
    if (sequence->GetSize() == 0)
    {
        return -1;
    }

    for (int i = startIndex; i <= this->GetSize() - sequence->GetSize(); i++)
    {
        bool found = true;
        for (int j = 0; j < sequence->GetSize(); j++)
        {
            if (this->operator[](j + i) != sequence->operator[](j))
            {
                found = false;
                break;
            }
        }

        if (found)
        {
            return i;
        }
    }

    return -1;
}

template<typename T>
T Sequence<T>::Reduce(const std::function<T(T prev, T current)> &func) const
{
    T returnValue{};
    
    if (this->GetSize() > 0)
    {
        returnValue = this->operator[](0);
    }

    for (int i = 1; i < this->GetSize(); i++)
    {
        returnValue = func(returnValue, this->operator[](i));
    }

    return returnValue;
}

template<typename T>
int Sequence<T>::Find(T item, int startIndex) const
{
    for (int i = startIndex; i < this->GetSize(); i++)
    {
        if (this->operator[](i) == item)
        {
            return i;
        }


    }    return -1;

}