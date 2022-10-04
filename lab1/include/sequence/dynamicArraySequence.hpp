#pragma once
#include "dynamicArray.hpp"
#include "sequence.hpp"
#include <ctime>
#include <stdexcept>

#include "TestHelper.hpp"

template<typename T>
class DynamicArraySequence : public Sequence<T>
{
private:
    DynamicArray<T> *array = nullptr;
    int elementsCount = 0;

public:
    DynamicArraySequence();
    DynamicArraySequence(const T* items, int count);
    DynamicArraySequence(const DynamicArraySequence <T>& array);
    ~DynamicArraySequence() override;

    T &GetFirst() override;
    T &GetLast() override;
    int GetActualSize() const override;
    int GetSize() const override;

    T &operator[](int index) const override;
    
    void Append(T item) override;
    void Prepend(T item) override;
    void PopFront() override;
    void PopBack() override;
    void Delete(int index) override;
    void InsertAt(T item, int index) override;
    void Clear() override;

    Sequence<T> *Copy(const Sequence<T> *seq) override;
    Sequence<T> *Where(const std::function<bool(T)> &func) const override;
    Sequence<T> *Concat(Sequence <T> *array) const override;
    Sequence<T> *GetSubsequence(int startIndex, int endIndex) const override;
};

template<typename T>
void DynamicArraySequence<T>::Clear()
{
    array->Resize(0);
    this->elementsCount = 0;
}

template <typename T>
DynamicArraySequence<T>::~DynamicArraySequence()
{
    delete array;
}

template <typename T>
DynamicArraySequence<T>::DynamicArraySequence()
{
    this->sequenceType = SequenceType::array;
    this->array = new DynamicArray<T>(0);
    elementsCount = 0;
}

template <typename T>
DynamicArraySequence<T>::DynamicArraySequence(const T* items, int count)
{
    this->sequenceType = SequenceType::array;
    this->array = new DynamicArray<T>(items, count);
    elementsCount = count;
}

template <typename T>
DynamicArraySequence<T>::DynamicArraySequence(const DynamicArraySequence <T>& array)
{
    this->sequenceType = SequenceType::array;
    this->array = new DynamicArray<T>(*array.array);
    this->elementsCount = array.elementsCount;
}

template<typename T>
Sequence<T> *DynamicArraySequence<T>::Copy(const Sequence<T> *seq)
{
    delete this->array;

    this->array = new DynamicArray<T>(0);

    for (int i = 0; i < seq->GetSize(); i++)
    {
        this->Append(seq->operator[](i));
    }

    return this;
}

template <typename T>
T &DynamicArraySequence<T>::GetFirst()
{
    return (*this)[0];
}

template <typename T>
T &DynamicArraySequence<T>::GetLast()
{
    return (*this)[this->elementsCount - 1];
}

template <typename T>
int DynamicArraySequence<T>::GetSize() const
{
    return this->elementsCount;
}

template<typename T>
int DynamicArraySequence<T>::GetActualSize() const
{
    return this->array->GetSize();
}

template <typename T>
void DynamicArraySequence<T>::Append(T item)
{
    if (this->elementsCount == 0)
    {
        this->array->Resize(1);
    }
    else if (this->elementsCount >= this->array->GetSize())
    {
        this->array->Resize(this->array->GetSize() * 2);
    }

    (*this->array)[elementsCount] = item;
    elementsCount++;
}

template <typename T>
void DynamicArraySequence<T>::Prepend(T item)
{
    InsertAt(item, 0);
}

template <typename T>
void DynamicArraySequence<T>::PopFront()
{
    if (this->elementsCount == 0)
    {
        throw std::runtime_error("PopFront called in DynamicArraySequence with zero elements");
    }

    Delete(0);
}

template<typename T>
void DynamicArraySequence<T>::Delete(int index)
{
    if (index < 0 || index >= this->elementsCount)
    {
        throw std::out_of_range("index is out of range");
    }

    if (this->elementsCount - 1 <= this->array->GetSize() / 2)
    {
        DynamicArray<T> *newArr = new DynamicArray<T>(this->array->GetSize() / 2);
        for (int i = 0; i < index; i++)
        {
            newArr->operator[](i) = (*this->array)[i];
        }

        for (int i = index; i < this->elementsCount - 1; i++)
        {
            newArr->operator[](i) = (*this->array)[i + 1];
        }

        delete this->array;
        this->array = newArr;
    }
    else
    {
        for (int i = index; i < elementsCount - 1; i++)
        {
            this->array->operator[](i) = this->array->operator[](i + 1) ;
        }
    }

    elementsCount--;
}

template <typename T>
void DynamicArraySequence<T>::PopBack()
{
    if (this->elementsCount == 0)
    {
        throw std::runtime_error("PopBack called in DynamicArraySequence with zero elements");
    }

    if (this->elementsCount - 1 <= this->array->GetSize() / 2)
    {
        this->array->Resize(this->array->GetSize() / 2);
    }

    elementsCount--;
}

template <typename T>
void DynamicArraySequence<T>::InsertAt(T item, int index)
{
    if (index > this->elementsCount || index < 0)
    {
        throw std::out_of_range("index is out of range");
    }

    if (index == this->elementsCount)
    {
        Append(item);
        return;
    }

    if (this->elementsCount >= this->array->GetSize())
    {
        DynamicArray<T> *newArr;
        if (this->elementsCount != 0)
        {
            newArr = new DynamicArray<T>(this->array->GetSize() * 2);
        }
        else
        {
            newArr = new DynamicArray<T>(1);
        }

        int i = 0;
        for (; i < index; i++)
        {
            newArr->operator[](i) = (*this->array)[i];
        }

        newArr->operator[](i++) = item;

        for (; i < this->elementsCount + 1; i++)
        {
            newArr->operator[](i) = (*this->array)[i - 1];
        }

        delete this->array;
        this->array = newArr;
    }
    else
    {
        for (int i = this->elementsCount + 1; i > index; i--)
        {
            this->array->operator[](i) = this->array->operator[](i - 1);
        }

        this->array->operator[](index) = item;
    }

    elementsCount++;
}

template <typename T>
T &DynamicArraySequence<T>::operator[](int index) const
{
    if (index < 0 || index >= this->elementsCount)
    {
        throw std::out_of_range("index is out of range");
    }

    return (*this->array)[index];
}

template <typename T>
Sequence<T> *DynamicArraySequence<T>::Concat(Sequence <T> *array) const
{

    if (array->GetType() != SequenceType::array)
    {
        throw std::runtime_error("Argument isn't a DynamicArraySequence");
    }

    DynamicArraySequence<T> *typecastedArray = (DynamicArraySequence<T> *)array;
    DynamicArraySequence<T> *ret = new DynamicArraySequence<T>();

    ret->array->Resize(this->elementsCount + typecastedArray->elementsCount);

    for (int i = 0; i < this->elementsCount; i++)
    {
        (*ret->array)[i] = (*this->array)[i];
    }

    for (int i = 0; i < typecastedArray->elementsCount; i++)
    {
        (*ret->array)[i + this->elementsCount] = (*typecastedArray->array)[i];
    }

    ret->elementsCount = this->elementsCount + typecastedArray->elementsCount;

    return ret;
}

template <typename T>
Sequence<T> *DynamicArraySequence<T>::GetSubsequence(int startIndex, int endIndex) const
{
    if (startIndex < 0 || endIndex < startIndex || endIndex >= this->elementsCount)
    {
        throw std::out_of_range("indices are out of range");
    }

    DynamicArraySequence<T> *ret = new DynamicArraySequence<T>();
    ret->array->Resize(endIndex - startIndex + 1);

    for (int i = startIndex, j = 0; i <= endIndex; i++, j++)
    {
        (*ret->array)[j] = (*this->array)[i];
    }

    ret->elementsCount = endIndex - startIndex + 1;
    return ret;
}

template<typename T>
Sequence<T> *DynamicArraySequence<T>::Where(const std::function<bool(T)> &func) const
{
    int counter = 0;
    for (int i = 0; i < this->GetSize(); i++)
    {
        if (func(this->operator[](i)))
        {
            counter++;
        }
    }

    DynamicArraySequence *newArr = new DynamicArraySequence<T>();
    newArr->array->Resize(counter);

    for (int i = 0; i < this->GetSize(); i++)
    {
        if (func(this->operator[](i)))
        {
            newArr->Append(this->operator[](i));
        }
    }

    return newArr;
}
