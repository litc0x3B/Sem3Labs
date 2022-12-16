#pragma once
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <stdexcept>
#include <string>
#include <random>

#include "icollection.hpp"

template <class T>
using ComparerFunc = std::function<int(const T&, const T&)>;


template <class T> struct BinTreeNode
{

    BinTreeNode *rightNode = nullptr;
    BinTreeNode *leftNode = nullptr;
    int size = 1;
    T value;


    BinTreeNode(const T value)
    {
        this->value = value;
    }
};

template <class T, class WrappedFuncArg> class HandlerFuncWrapper
{
protected:
    std::function<void(WrappedFuncArg)> handlerFunc;

public:
    HandlerFuncWrapper(const std::function <void(WrappedFuncArg)> &handlerFunc)
    {
        this->handlerFunc = handlerFunc;
    }

    virtual void Call(BinTreeNode<T>**) = 0;
};

template <class T> class HandlerNodeArg : public HandlerFuncWrapper<T, BinTreeNode<T>**>
{
public:
    HandlerNodeArg(const std::function <void(BinTreeNode<T>**)> &handlerFunc) : HandlerFuncWrapper<T, BinTreeNode<T>**>(handlerFunc) {}

    void Call(BinTreeNode<T>** node) override
    {
        this->handlerFunc(node);
    }
};

template <class T> class HandlerValueArg : public HandlerFuncWrapper<T, T&>
{
public:
    HandlerValueArg(const std::function <void(T&)> &handlerFunc) : HandlerFuncWrapper<T, T&>(handlerFunc) {}

    void Call(BinTreeNode<T>** node) override
    {
        this->handlerFunc((*node)->value);
    }
};

template <class T> int defaultCompFunc(const T &a, const T &b)
{
    if (a > b) return 1;
    if (a < b) return -1;
    return 0;
}


template <class T> class BinTree : public ICollection<T>
{
private:
    BinTreeNode<T> *root = nullptr;
    ComparerFunc<T> comparer;
    bool randomlyBalanced = true;

    static void _CheckTraverseOptions(std::string orderOptions)
    {
        if
        (
            orderOptions.size() != 3 ||
            orderOptions.find("L") == std::string::npos ||
            orderOptions.find("R") == std::string::npos ||
            orderOptions.find("N") == std::string::npos
        )
        {
            throw std::invalid_argument("Invalid traverse order options");
        }
    }

    static int _GetSize(BinTreeNode<T> *node)
    {
        if (node != nullptr)
        {
            return node->size;
        }
        else
        {
            return 0;
        }
    }

    static void _FixSize(BinTreeNode<T> *node)
    {
        node->size = _GetSize(node->leftNode) + _GetSize(node->rightNode) + 1;
    }


    static BinTreeNode<T> *_RotateRight(BinTreeNode<T> *rightNode)
    {
        BinTreeNode<T> *leftNode = rightNode->leftNode;

        if (leftNode == nullptr)
        {
            return rightNode;
        }

        rightNode->leftNode = leftNode->rightNode;
        leftNode->rightNode = rightNode;

        leftNode->size = rightNode->size;
        _FixSize(rightNode);

        return leftNode;
    }

    static BinTreeNode<T> *_RotateLeft(BinTreeNode<T> *leftNode)
    {
        BinTreeNode<T> *rightNode = leftNode->rightNode;

        if (rightNode == nullptr)
        {
            return leftNode;
        }

        leftNode->rightNode = rightNode->leftNode;
        rightNode->leftNode = leftNode;

        rightNode->size = leftNode->size;
        _FixSize(leftNode);

        return rightNode;
    }

    static bool _AreEqual(const BinTreeNode<T> *root1, const BinTreeNode<T> *root2)
    {
        if (root1 == nullptr && root2 == nullptr)
        {
            return true;
        }

        if (root1 == nullptr || root2 == nullptr)
        {
            return false;
        }

        return
        (
            root1->value == root2->value &&
            _AreEqual(root1->leftNode, root2->leftNode) &&
            _AreEqual(root1->rightNode, root2->rightNode)
        );
    }

    static bool _HasSubtree(const BinTreeNode<T> *treeRoot, const BinTreeNode<T> *subtreeRoot)
    {
        if (subtreeRoot == nullptr)
        {
            return true;
        }

        if (treeRoot == nullptr)
        {
            return false;
        }

        return
        (
            _AreEqual(treeRoot, subtreeRoot) ||
            _HasSubtree(treeRoot->leftNode, subtreeRoot) ||
            _HasSubtree(treeRoot->rightNode, subtreeRoot)
        );
    }

    void _Map(BinTree<T> *tree, const std::function <T(const T&)> &handlerFunc) const
    {
        TraverseConst([handlerFunc, tree](const T &value)
        {
            T newValue = handlerFunc(value);
            tree->Add(newValue);
        });
    }

    void _Where(BinTree<T> *tree, const std::function<bool(const T&)> &handlerFunc) const
    {
        TraverseConst([tree, handlerFunc](const T &value)
        {
            if (handlerFunc(value))
            {
                tree->Add(value);
            }
        });
    }

    BinTreeNode<T> *_Join(BinTreeNode<T> *leftRoot, BinTreeNode<T> *rightRoot) 
    {
        if (leftRoot == nullptr)
        {
            return rightRoot;
        }
        if (rightRoot == nullptr)
        {
            return leftRoot;
        }

        if (std::rand() % (_GetSize(rightRoot) + _GetSize(leftRoot)) < _GetSize(rightRoot) || !randomlyBalanced)
        {
            rightRoot->leftNode = _Join(leftRoot, rightRoot->leftNode);
            _FixSize(rightRoot);
            return rightRoot;
        }
        else
        {
            leftRoot->rightNode = _Join(leftRoot->rightNode, rightRoot);
            _FixSize(leftRoot);
            return leftRoot;
        }
    }

    BinTreeNode<T> *_InsertRoot(BinTreeNode<T> *node, const T &value)
    {
        if (node == nullptr)
        {
            return new BinTreeNode<T>(value);
        }

        int comp = comparer(value, node->value);

        if (comp > 0)
        {
            node->rightNode = _InsertRoot(node->rightNode, value);
            return _RotateLeft(node);
        }
        else if (comp < 0)
        {
            node->leftNode = _InsertRoot(node->leftNode, value);
            return _RotateRight(node);
        }
        else
        {
            return node;
        }

    }

    BinTreeNode<T> *_Insert(BinTreeNode<T> *node, const T &value)
    {
        if (node == nullptr)
        {
            return new BinTreeNode<T>(value);
        }

        if (randomlyBalanced && std::rand() % (_GetSize(node) + 1) == 0)
        {
            return _InsertRoot(node, value);
        }

        int comp = comparer(value, node->value);

        if (comp > 0)
        {
            node->rightNode = _Insert(node->rightNode, value);
        }
        else if (comp < 0)
        {
            node->leftNode = _Insert(node->leftNode, value);
        }

        _FixSize(node);
        return node;
    }

    template<class WrappedFuncArg>
    void _Traverse(HandlerFuncWrapper<T, WrappedFuncArg> *wrapper, std::string orderOptions, BinTreeNode<T> **nextNode)
    {

        if (*nextNode == nullptr)
        {
            return;
        }

        BinTreeNode<T> *node = *nextNode;
        for (int i = 0; i < 3; i++)
        {
            char nextAction = orderOptions[i];
            switch (nextAction)
            {
                case 'L': _Traverse(wrapper, orderOptions, &node->leftNode); break;
                case 'R': _Traverse(wrapper, orderOptions, &node->rightNode); break;
                case 'N': wrapper->Call(nextNode); break;
                default: throw std::invalid_argument("Invalid order options passed to _Traverse function (it's not your fault)"); break;
            }
        }
    }

    void _TraverseConst(std::function<void(const T&)> handlerFunc, std::string orderOptions, const BinTreeNode<T> *nextNode) const
    {

        if (nextNode == nullptr)
        {
            return;
        }

        for (int i = 0; i < 3; i++)
        {
            char nextAction = orderOptions[i];
            switch (nextAction)
            {
                case 'L': _TraverseConst(handlerFunc, orderOptions, nextNode->leftNode); break;
                case 'R': _TraverseConst(handlerFunc, orderOptions, nextNode->rightNode); break;
                case 'N': handlerFunc(nextNode->value); break;
                default: throw std::invalid_argument("Invalid order options passed to _TraverseConst function (it's not your fault)"); break;
            }
        }
    }

    void _TraverseNodes(const std::function <void(BinTreeNode<T>**)> handlerFunc, BinTreeNode<T> **root, std::string orderOptions = "LNR")
    {
        _CheckTraverseOptions(orderOptions);
        HandlerNodeArg<T> wrapper(handlerFunc);
        _Traverse<BinTreeNode<T>**>(&wrapper, orderOptions, root);
    }

    void _TraverseNodes(const std::function <void(BinTreeNode<T>**)> &handlerFunc, std::string orderOptions = "LNR")
    {
        _TraverseNodes(handlerFunc, &root, orderOptions);
    }

    void _Assign(const BinTree<T> &tree)
    {
        if (this == &tree)
        {
            return;
        }

        this->comparer = tree.comparer;
        this->randomlyBalanced = tree.randomlyBalanced;
        this->Clear();
        this->InsertFromTree(tree);

        return;
    }

    BinTreeNode<T> **_Search(const T &item, BinTreeNode<T> **curNode) const
    {
        if (*curNode == nullptr)
        {
            return nullptr;
        }

        int comp = comparer(item, (*curNode)->value);

        if (comp > 0)
        {
            return _Search(item, &(*curNode)->rightNode);
        }
        else if (comp < 0)
        {
            return _Search(item, &(*curNode)->leftNode);
        }
        else
        {
            return curNode;
        }
    }

    BinTreeNode<T> *const*_SearchConst(const T &item, BinTreeNode<T> *const*curNode) const
    {
        if (*curNode == nullptr)
        {
            return nullptr;
        }

        int comp = comparer(item, (*curNode)->value);

        if (comp > 0)
        {
            return _SearchConst(item, &(*curNode)->rightNode);
        }
        else if (comp < 0)
        {
            return _SearchConst(item, &(*curNode)->leftNode);
        }
        else
        {
            return curNode;
        }
    }

    BinTreeNode<T> *_GetMin(BinTreeNode<T> *node) const
    {   
        if (node->leftNode != nullptr)
        {
            return _GetMin(node->leftNode);
        }
        else 
        {
            return node;
        }
    }

    BinTreeNode<T> *_GetMax(BinTreeNode<T> *node) const
    {   
        if (node->rightNode != nullptr)
        {
            return _GetMax(node->rightNode);
        }
        else 
        {
            return node;
        }
    }

public:
    explicit BinTree(const ComparerFunc<T> &comparer = defaultCompFunc<T>, bool randomlyBalanced = true)
    {
        this->comparer = comparer;
        this->randomlyBalanced = randomlyBalanced;
        std::srand(time(NULL));
    }

    explicit BinTree(const std::function<T(const std::string&)> &strToItem, std::string str, std::string delim = " ", const std::function <int(const T&, const T&)> &comparer = defaultCompFunc<T>, bool randomlyBalanced = true)
    {
        this->comparer = comparer;
        this->randomlyBalanced = randomlyBalanced;
        AddFromStr(strToItem, str, delim);
        std::srand(time(NULL));
    }


    ~BinTree()
    {
        Clear();
    }

    BinTree(BinTree<T> &&tree)
    {
        _Assign(tree);
    }

    BinTree(BinTree<T> &tree)
    {
        _Assign(tree);
    }

    BinTree<T> &operator=(const BinTree<T> &tree)
    {
        _Assign(tree);
        return *this;
    }

    T GetMin()
    {
        if (root == nullptr)
        {
            throw std::logic_error("Tree is empty");
        }

        BinTreeNode<T> *node = _GetMin(root);
        return node->value;
    }

    T GetMax()
    {
        if (root == nullptr)
        {
            throw std::logic_error("Tree is empty");
        }

        BinTreeNode<T> node = _GetMax(root);
        return node.value;
    }

    void Clear()
    {
        _TraverseNodes([](BinTreeNode<T> **node)
        {
            delete *node;
        }, "LRN");

        root = nullptr;
    }

    void InsertFromTree(const BinTree<T> &tree)
    {
        tree.TraverseConst([&](const T &value)
        {
            this->Add(value);
        }, "NLR");
    }

    int GetSize() const override
    {
        return _GetSize(root);
    }

    bool Search(const T &item) const override
    {
        return _SearchConst(item, &root) != nullptr;
    }

    bool Search(const T &item, T &foundItem) const override
    {
        BinTreeNode<T> *const*result = _SearchConst(item, &root);

        if (result != nullptr)
        {
            foundItem = (*result)->value;
            return true;
        }

        return false;
    }

    void Traverse(const std::function <void(T&)> &handlerFunc, std::string orderOptions = "LNR")
    {
        _CheckTraverseOptions(orderOptions);
        HandlerValueArg<T> wrapper(handlerFunc);
        _Traverse<T&>(&wrapper, orderOptions, &root);
    }

    void TraverseConst(const std::function <void(const T&)> &handlerFunc, std::string orderOptions = "LNR") const
    {
        _CheckTraverseOptions(orderOptions);
        _TraverseConst(handlerFunc, orderOptions, root);
    }

    BinTree<T> Where(const std::function<bool(const T&)> &handlerFunc) const
    {
        BinTree<T> newTree(comparer, randomlyBalanced);
        _Where(&newTree, handlerFunc);
        return newTree;
    }

    BinTree<T> *WhereC(const std::function<bool(const T&)> &handlerFunc) const override
    {
        auto *newTree = new BinTree<T>(comparer, randomlyBalanced);
        _Where(newTree, handlerFunc);
        return newTree;
    }

    BinTree<T> Map(const std::function <T(const T&)> &handlerFunc) const
    {
        BinTree<T> newTree(comparer, randomlyBalanced);
        _Map(&newTree, handlerFunc);
        return newTree;
    }

    BinTree<T> *MapC(const std::function <T(const T&)> &handlerFunc) const override
    {
        auto *newTree = new BinTree<T>(comparer, randomlyBalanced);
        _Map(newTree, handlerFunc);
        return newTree;
    }

    T Reduce(const std::function <T(const T&, const T&)> &handlerFunc, T identity) const override
    {
        T prevValue = identity;

        TraverseConst([handlerFunc, &prevValue](const T& item)
        {
            prevValue = handlerFunc(prevValue, item);
        });
        
        return prevValue;
    }

    void Add(const T &item) override
    {
        root = _Insert(root, item);
    }
    
    void Remove(const T &item) override
    {
        BinTreeNode<T> **targetNode = _Search(item, &root);
        if (targetNode == nullptr)
        {
            return;
        }

        BinTreeNode<T> *newNode = _Join((*targetNode)->leftNode, (*targetNode)->rightNode);
        delete *targetNode;
        *targetNode = newNode;
    }

    std::string ToStr(const std::function<std::string(const T&)> &itemToStr, std::string delim, std::string orderOptions) const
    {
        if (root == nullptr)
        {
            return "";
        }

        std::string str;

        TraverseConst([&str, itemToStr, delim](const T& item)
        {
            str += itemToStr(item) + delim;
        }, orderOptions);

        str.pop_back();
        return str;
    }

    std::string ToStr(const std::function<std::string(const T&)> &itemToStr = static_cast<std::string(*)(T)>(std::to_string),
                     std::string delim = " ") const override
    {
        return ToStr(itemToStr, delim, "LNR");
    }

    std::string ToStr(std::string delim, std::string orderOptions = "LNR") const
    {
        return ToStr(static_cast<std::string(*)(T)>(std::to_string), delim, orderOptions);
    }

    void AddFromStr(const std::function<T(const std::string&)> &strToItem, std::string str, std::string delim = " ") override
    {
        size_t pos;
        while (pos = str.find(delim), pos != std::string::npos)
        {
            std::string token = str.substr(0, pos);
            Add(strToItem(token));
            str.erase(0, pos + delim.length());
        }

        if (str.length() != 0)
        {
            Add(strToItem(str));
        }
    }

    BinTree<T> GetSubtree(T rootItem) const
    {
        BinTree<T> newTree(comparer, randomlyBalanced);
        BinTreeNode<T> *const*newTreeRoot = _SearchConst(rootItem, &root);

        if (newTreeRoot == nullptr)
        {
            return newTree;
        }

        _TraverseConst([&newTree](const T &item)
        {
            newTree.Add(item);
        }, "NLR", *newTreeRoot);

        return newTree;
    }

    bool HasSubtree(const BinTree<T> &tree) const
    {
        return _HasSubtree(this->root, tree.root);
    }
    

    bool IsRandomlyBalanced() const
    {
        return randomlyBalanced;
    }

    void SetRandomlyBalanced(bool value)
    {
        randomlyBalanced = value;
    }

    ComparerFunc<T> GetComparerFunc() const
    {
        return comparer;
    }

};
