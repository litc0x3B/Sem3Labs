#include <gtest/gtest.h>

#include "graph.hpp"

using namespace Graph;
using namespace Graph::Oriented;

/*
        1
 ┌─1──────────►2
 │ ▲           │9
 │ │           ▼
2│ └───►3◄────►4
 │  4      7   │
 │             │2
 │             ▼
 └►5──────────►6
        8
 */
WeightedGraph<int> GenerateGraph()
{
  WeightedGraph<int> graph;

  const auto &node1 = graph.AddNode();
  graph.AddNodesRetFirstId(5);

  graph.SetEdge(1, 2, 1);
  graph.SetEdge(1, 5, 2);
  graph.SetEdge(1, 3, 4);

  graph.SetEdge(2, 4, 9);
  graph.SetEdge(3, 4, 7);
  graph.SetEdge(3, 1, 4);
  graph.SetEdge(4, 6, 2);
  graph.SetEdge(5, 6, 8);

  return graph;
}

TEST(WeightedGraph, GettersAndSetters)
{
  auto graph = GenerateGraph();

  ASSERT_EQ(graph.AddNode().GetId(), 7);
  auto node1 = graph.SetEdge(7, 1, 1).GetHead();
  ASSERT_TRUE(node1.HasEdgeTo(5));
  ASSERT_TRUE(node1.HasEdgeTo(3));
  ASSERT_TRUE(node1.HasEdgeTo(2));
  ASSERT_FALSE(node1.HasEdgeTo(4));
  ASSERT_EQ(node1.GetEdgeTo(5).GetHead().GetId(), 5);
  ASSERT_EQ(graph.GetEdge(5, 6).GetTail().GetId(), 5);
}

TEST(WeightedGraph, NodeIterators)
{
  auto graph = GenerateGraph();

  auto node = graph.GetNode(1);
  for (const auto &[id, edge] : node)
  {
    ASSERT_TRUE(node.HasEdgeTo(id));
  }
}

int main()
{
  auto graph = GenerateGraph();
  auto paths = graph.Dijkstra(1);

  for (const auto &pair : paths)
  {
    std::vector<WeightedEdge<int>> edges = pair.second->GetEdges();
    for (const auto &edge : edges)
    {
      std::cout << "(" << edge.GetTail().GetId() << ", " << edge.GetHead().GetId() << "), ";
    }
    std::cout << "distance: " << pair.second->GetDistance() << std::endl;
  }
  testing::InitGoogleTest();
  RUN_ALL_TESTS();


  return 0;
}


