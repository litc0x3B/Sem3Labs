#pragma once

#include <functional>
#include <iostream>
#include "sequence/dynamicArraySequence.hpp"

template <class T> using IsEqualFunc = std::function<bool(T&)>;

template <class T> class TreeNode {
private:
  DynamicArraySequence<TreeNode<T> *> *childrenSeq;
  TreeNode *parent = nullptr;
  T value;
  

public:
  TreeNode(T value, TreeNode *parentNode = nullptr) {
    childrenSeq = new DynamicArraySequence<TreeNode *>;
    this->parent = parentNode;
    this->value = value;
  }

  ~TreeNode() {
    for (int i = 0; i < childrenSeq->GetSize(); i++) {
      delete childrenSeq->At(i);
    }

    delete childrenSeq;
  }

  T GetValue() const {return value;}

  T &Value() {return value;}

  DynamicArraySequence<TreeNode<T>*> *Children() {
    return childrenSeq; 
  }

  DynamicArraySequence<TreeNode<T>*> const *GetChildren() const {
    return childrenSeq; 
  }

  TreeNode<T> *Add(T value) {
    TreeNode<T> *newNode = new TreeNode<T>(value, this); 
    childrenSeq->Append(newNode);
    return newNode;
  }

  TreeNode<T> *Add(TreeNode<T> *node) {
    childrenSeq->Append(node);
    node->parent = this;
    return node;
  }

  T FindChild(const IsEqualFunc<T> &findFunc, bool &isSuccesful) const {
    for (int i = 0; i < childrenSeq->GetSize(); i++) {
      if (findFunc(childrenSeq->Get(i)->Value())) {
        isSuccesful = true;
        return childrenSeq->Get(i)->Value();
      }
   }

    isSuccesful = false;
    return T();
  }

  T FindChild(const IsEqualFunc<T> &findFunc) const {
    bool placeholder;
    return FindChild(findFunc, placeholder);
  }
  
  TreeNode<T> *Parent() const {  return parent;}
  TreeNode<T> const *GetParent() const { return parent; }

  TreeNode<T> *Extract() {
    for (int i = 0; i < parent->childrenSeq->GetSize(); i++) {
      auto curNode = parent->childrenSeq->At(i);
      if (this == curNode) {
        parent->childrenSeq->Delete(i);
        return this;
      }
    }
    return this;
  }
};

template <class T> class Tree {
private:
  TreeNode<T> *root;

public:
  Tree(T rootValue) { root = new TreeNode<T>(rootValue); }
  ~Tree() {delete root;}

  const TreeNode<T> *GetRoot() const { return root; }
  TreeNode<T> *Root() {return root;}

};
