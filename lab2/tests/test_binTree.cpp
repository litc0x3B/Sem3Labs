#include <gtest/gtest.h>
#include <memory>
#include <string>
#include "binTree.hpp"


//                      10
//                   /       \
//                 8          13
//               /   \       /   \
//             7     9     11    14
//                           \
//                            12
//

std::shared_ptr<BinTree<int>> getTestTreeNotBalanced()
{
    std::shared_ptr<BinTree<int>> tree = std::make_shared<BinTree<int>>();
    tree->SetRandomlyBalanced(false);
    tree->Add(10);
    tree->Add(8);
    tree->Add(9);
    tree->Add(13);
    tree->Add(14);
    tree->Add(7);
    tree->Add(11);
    tree->Add(12);

    return tree;
}

std::shared_ptr<BinTree<int>> getTestTree()
{
    std::shared_ptr<BinTree<int>> tree = std::make_shared<BinTree<int>>();
    const int size = 5;

    for (int i = 0; i < size; i++)
    {
        int num = i + 1;
        tree->Add(num);
    }    

    return tree;
}

TEST(BinTreeTest, SizeBalanced)
{
    auto tree = getTestTree();
    ASSERT_EQ(tree->GetSize(), 5);
}

TEST(BinTreeTest, Search)
{
    auto tree = getTestTree();
    int foundItem;
    EXPECT_TRUE(tree->Search(5, foundItem));
    EXPECT_EQ(5, foundItem);
    EXPECT_TRUE(tree->Search(11).IsNull());
}

TEST(BinTreeTest, Where)
{
    auto tree = getTestTree();
    EXPECT_EQ(tree->Where([](int item){return item <= 3;})->ToStr(), "1 2 3");
}

TEST(BinTreeTest, Map)
{
    auto tree = getTestTree();
    EXPECT_EQ
    (
        tree->Map([](int item){return item * 10;})->ToStr(), 
        "10 20 30 40 50"
    );
}

TEST(BinTreeTest, Reduce)
{
    auto tree = getTestTree();
    EXPECT_EQ
    (
        tree->Reduce([](int prev, int cur){return prev + cur;}, 0), 
        1 + 2 + 3 + 4 + 5
    );
}

TEST(BinTreeTest, Remove)
{
    auto tree = getTestTree();
    EXPECT_EQ(tree->Remove(3).GetValue(), 3);

    EXPECT_EQ
    (
        tree->ToStr(), 
        "1 2 4 5"
    );
}

TEST(BinTreeTest, GetSubtree)
{
    auto tree = getTestTreeNotBalanced();

    EXPECT_EQ
    (
        tree->GetSubtree(13)->ToStr(), 
        "11 12 13 14"
    );
}

TEST(BinTreeTest, HasSubtree)
{
    auto tree = getTestTreeNotBalanced();

    auto subtree = tree->GetSubtree(13);
    EXPECT_TRUE(tree->HasSubtree(subtree));

    subtree->Remove(11);
    EXPECT_FALSE(tree->HasSubtree(subtree));
}

