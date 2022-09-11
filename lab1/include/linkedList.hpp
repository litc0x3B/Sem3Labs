#pragma once
#include <stdexcept>
#include <iostream>

template <class T>
class Node
{
public:
    Node(Node<T> *prev, T value, Node<T> *next)
    {
        this->prev = prev;
        this->next = next;
        this->value = value;
    }

    Node<T> *next = nullptr;
    Node<T> *prev = nullptr;
    T value;
};

template <class T>
class LinkedList
{
private:
    Node<T> *tail = nullptr;
    Node<T> *head = nullptr;
    int size = 0;

    void InitFirstNode(T item);
    Node<T> *GetNode(int index) const;

public:
    LinkedList();
    LinkedList(const T *items, int size);
    LinkedList(const LinkedList<T> &list);
    LinkedList(const LinkedList<T> &list, int startIndex, int endIndex);
    ~LinkedList();

    int GetSize() const;

    void Append(T item);
    void Prepend(T item);
    void PopBack();
    void PopFront();
    void Delete(int index);

    T &operator[](int index) const;
    LinkedList<T> &operator=(const LinkedList<T> &list);
    LinkedList<T> operator+(const LinkedList<T> &list) const;
    LinkedList<T> operator+=(const LinkedList<T> &list);

    void InsertAt(T item, int index);
};

template <typename T>
void LinkedList<T>::InitFirstNode(T item)
{
    tail = new Node<T>(nullptr, item, nullptr);
    head = tail;

    head->prev = nullptr;
    tail->next = nullptr;
}

template <typename T>
Node<T> *LinkedList<T>::GetNode(int index) const
{
    if (index >= size || index < 0)
    {
        throw std::out_of_range("There is no element with such index in the list");
    }

    Node<T> *current;

    if (index <= size / 2)
    {
        current = head;
        for (int i = 0; i < index; i++)
        {
            current = current->next;
        }
    }
    else
    {
        current = tail;
        for (int i = size - 1; i > index; i--)
        {
            current = current->prev;
        }
    }

    return current;
}

template <typename T>
LinkedList<T>::LinkedList() {}

template <typename T>
LinkedList<T>::LinkedList(const T *items, int size)
{
    for (int i = 0; i < size; i++)
    {
        Append(items[i]);
    }
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T> &list)
{
    Node<T> *curFromList = list.head;
    for (int i = 0; i < list.size; i++)
    {
        Append(curFromList->value);
        curFromList = curFromList->next;
    }
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T> &list, int startIndex, int endIndex)
{

    if (startIndex < 0 || startIndex >= list.size || endIndex < startIndex)
    {
        throw std::out_of_range("invalid startIndex");
    }

    Node<T> *current = list.GetNode(startIndex);

    if (endIndex < 0 || endIndex >= list.size )
    {
        throw std::out_of_range("invalid endIndex");
    }


    for (int i = startIndex; i <= endIndex; i++)
    {
        Append(current->value);
        current = current->next;
    }

}

template <typename T>
void LinkedList<T>::Append(T item)
{
    if (head == nullptr)
    {
        InitFirstNode(item);
    }
    else
    {
        tail->next = new Node<T>(tail, item, nullptr);
        tail = tail->next;
        tail->value = item;
    }

    size++;
}

template <typename T>
void LinkedList<T>::Prepend(T item)
{
    if (head == nullptr)
    {
        InitFirstNode(item);
    }
    else
    {
        head->prev = new Node<T>(nullptr, item, head);
        head = head->prev;
        head->value = item;
    }

    size++;
}

template <typename T>
void LinkedList<T>::PopBack()
{
    if (tail == nullptr)
    {
        throw std::runtime_error("calling PopBack() in LinkedList with zero elements");
    }

    if (tail->prev == nullptr)
    {
        delete tail;
        tail = nullptr;
        head = nullptr;
    }
    else
    {
        tail = tail->prev;
        delete tail->next;
        tail->next = nullptr;
    }

    size--;
}

template <typename T>
void LinkedList<T>::PopFront()
{
    if (head == nullptr)
    {
        throw std::runtime_error("calling PopFront() in LinkedList with zero elements");
    }

    if (head->next == nullptr)
    {
        delete head;
        tail = nullptr;
        head = nullptr;
    }
    else
    {
        head = head->next;
        delete head->prev;
        head->prev = nullptr;
    }

    size--;
}

template <typename T>
void LinkedList<T>::Delete(int index)
{
    if (index < 0 || index >= size)
    {
        throw std::out_of_range("index is out of range");
    }

    if (index == size - 1)
    {
        PopBack();
        return;
    }
    else if(index == 0)
    {
        PopFront();
        return;
    }

    Node<T> *node = GetNode(index);
    node->next->prev = node->prev;
    node->prev->next = node->next;
    size--;

    delete node;
}

template <typename T>
T &LinkedList<T>::operator[](int index) const
{
    return GetNode(index)->value;
}

template <typename T>
void LinkedList<T>::InsertAt(T item, int index)
{
    if (index < 0 || index > size)
    {
        throw std::out_of_range("index is out of range");
    }

    if (index == 0)
    {
        Prepend(item);
        return;
    }
    else if (index == size)
    {
        Append(item);
        return;
    }

    Node<T> *next = GetNode(index);
    Node<T> *prev = next->prev;

    Node<T> *current = new Node<T>(prev, item, next);

    prev->next = current;
    next->prev = current;
    size++;
}

template <typename T>
LinkedList<T>::~LinkedList<T>()
{
    while (size != 0)
    {
        PopBack();
    }
}

template <typename T>
int LinkedList<T>::GetSize() const
{
    return size;
}


template <typename T>
LinkedList<T> LinkedList<T>::operator+(const LinkedList<T> &list) const
{
    LinkedList<T> newList = (*this);
    newList += list;
    return newList;
}

template <typename T>
LinkedList<T> LinkedList<T>::operator+=(const LinkedList<T> &list)
{
    Node<T> *curFromList = list.head;
    for (int i = 0; i < list.size; i++)
    {
        Append(curFromList->value);
        curFromList = curFromList->next;
    }

    return (*this);
}

template <typename T>
LinkedList<T> &LinkedList<T>::operator=(const LinkedList<T> &list)
{
    if (&list == this)
    {
        return (*this);
    }

    Node<T> *currentDest = head;
    Node<T> *currentSource = list.head;

    while (currentDest != nullptr && currentSource != nullptr)
    {
        currentDest->value = currentSource->value;
        currentDest = currentDest->next;
        currentSource = currentSource->next;
    }

    if (size < list.size)
    {
        while (currentSource != nullptr)
        {
            Append(currentSource->value);
            currentSource = currentSource->next;
        }

        size = list.size;
    }
    else
    {
        while (size != list.size)
        {
            PopBack();
        }
    }

    return (*this);
}
