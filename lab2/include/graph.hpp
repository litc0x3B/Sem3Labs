#pragma once
#include "dictionary/dictionary.hpp"
#include "set.hpp"

namespace Graph
{

struct Node
{
 private:
  inline static int lastId = 0;

 public:
  int id;
  Set<int> outgoingEdgesIds;

  Node();
  Node(const Node &node);
};

Node::Node() : id(lastId)
{
  lastId++;
}

Node::Node(const Node &node) : Node()
{
  this->outgoingEdgesIds = node.outgoingEdgesIds;
}


template<class Cost>
struct Edge
{
  private:
  inline static int lastId = 0;

 public:
  int id;
  int toId;
  int fromId;
  Cost cost;

  explicit Edge(Cost cost);
  Edge(const Edge &edge);
  Edge &operator=(const Edge &edge);
};
template <class Cost>
Edge<Cost> &Edge<Cost>::operator=(const Edge &edge)
{
  return <#initializer #>;
}
template <class Cost>
Edge<Cost>::Edge(const Edge &edge)
{

}
template <class Cost>
Edge<Cost>::Edge(Cost cost)
{

}

}