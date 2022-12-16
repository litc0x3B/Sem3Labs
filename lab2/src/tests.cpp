#include "TestHelper.hpp"
#include "test_tree.hpp"
#include "test_binTree.hpp"
#include "test_indexing.hpp"
#include "test_set.hpp"
#include "test_nullable.hpp"

int main() {
  testing::InitGoogleTest();
  RUN_ALL_TESTS();
  return 0;
}
