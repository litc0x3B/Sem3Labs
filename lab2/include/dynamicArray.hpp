#pragma once

#include <stdexcept>

template <typename T>
class DynamicArray
{
private:
    T *storage;
    int size;

    void Init(int size);

public:
    DynamicArray(const T *items, int size);
    DynamicArray(int size);
    DynamicArray(const DynamicArray<T> &dynamicArray);

    ~DynamicArray();

    T &operator[](int index);

    int GetSize() const;
    void Resize(int newSize);
};

#pragma region constructors and destructors
template <typename T>
void DynamicArray<T>::Init(int size)
{
    if (size == 0)
    {
        this->size = 0;
        return;
    }

    if (size < 0)
    {
        throw std::bad_array_new_length();
    }

    this->size = size;
    storage = new T[size];
}

template <typename T>
DynamicArray<T>::DynamicArray(const T *items, int size)
{
    Init(size);
    for (int i = 0; i < size; i++)
    {
        storage[i] = items[i];
    }
}

template <typename T>
DynamicArray<T>::DynamicArray(int size)
{
    Init(size);
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T> &dynamicArray)
{
    Init(dynamicArray.GetSize());
    for (int i = 0; i < dynamicArray.GetSize(); i++)
    {
        storage[i] = dynamicArray.storage[i];
    }
}

template <typename T>
DynamicArray<T>::~DynamicArray()
{
    Resize(0);
}
#pragma endregion constructors and destructors

template <typename T>
T &DynamicArray<T>::operator[](int index)
{
    if (index < 0 || index >= size)
    {
        throw std::out_of_range("Index is out of range" );
    }
    return storage[index];
}

template <typename T>
int DynamicArray<T>::GetSize() const
{
    return size;
}

template <typename T>
void DynamicArray<T>::Resize(int newSize)
{
    if (newSize == 0)
    {
        if (size != 0) 
        {
            delete[] storage;
        }

        size = 0;
        return;
    }

    if (newSize < 0)
    {
        throw std::bad_array_new_length();
    }

    T *tempStorage = new T[newSize];

    int minSize = size < newSize ? size : newSize;
    for (int i = 0; i < minSize; i++)
    {
        tempStorage[i] = storage[i];
    }

    if (size != 0)
    {
        delete[] storage;
    }

    storage = tempStorage;
    size = newSize;
}
