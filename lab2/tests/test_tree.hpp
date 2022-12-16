#pragma once

#include <functional>
#include "TestHelper.hpp"
#include "tree.hpp"

DynamicArraySequence<int> *TreeNodeToSeq(const TreeNode<int> *node) {
  DynamicArraySequence<int> *seq = new DynamicArraySequence<int>;
  auto *treeChildren = node->GetChildren(); 
  for (int i = 0; i < treeChildren->GetSize(); i++) {
    seq->Append(treeChildren->Get(i)->GetValue());
  }
  return seq;
}

TEST(treeTests, addAndPopNode) {
  Tree<int> tree(0);
  tree.Root()->Add(1);
  tree.Root()->Add(2);

  TreeNode<int> *node = tree.Root()->Add(new TreeNode<int>(3));
  node->Add(4);

  int expArr[] = {1, 2, 3};
  auto *seq = TreeNodeToSeq(tree.Root());
  containersEqual(*seq, expArr, 3);
  delete seq;
}
