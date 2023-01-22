#pragma once
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <vector>

namespace Graph::Oriented
{
template <class TEdge>
struct Node
{
  int id;

  // first int is destination node id
  std::map<int, TEdge> outgoingEdges;

  explicit Node(int id) : id(id) {}
};

template <class TEdge>
class ImmutableNode
{
 private:
  const Node<TEdge> *node;

 public:
  ImmutableNode(const Node<TEdge> &node) { this->node = &node; }

  const TEdge &GetEdgeTo(int id) const { return node->outgoingEdges.at(id); }
  bool HasEdgeTo(int id) const { return node->outgoingEdges.find(id) != node->outgoingEdges.end(); }
  int GetId() const { return node->id; }
  typename std::map<int, TEdge>::const_iterator begin() const
  {
    return node->outgoingEdges.begin();
  }
  typename std::map<int, TEdge>::const_iterator end() const { return node->outgoingEdges.end(); }
};

template <class TWeight>
class WeightedEdge
{
 private:
  TWeight weight;
  Node<WeightedEdge> *tail;
  Node<WeightedEdge> *head;

 public:
  TWeight GetWeight() const { return weight; }
  ImmutableNode<WeightedEdge> GetTail() const { return *tail; }
  ImmutableNode<WeightedEdge> GetHead() const { return *head; }

 public:
  WeightedEdge(Node<WeightedEdge> &tail, Node<WeightedEdge> &head, TWeight weight) : weight(weight)
  {
    this->tail = &tail;
    this->head = &head;
  }

  bool operator>(const WeightedEdge &edge) { this->weight > edge.weight; }
};

template <class TNode, class TEdge, class TWeight>
struct Path
{
 private:
  TWeight distance{};
  std::vector<TEdge> edges;

 public:
  explicit Path(const std::vector<TEdge> &&edges) : edges(edges)
  {
    for (const auto &edge : this->edges)
    {
      distance = distance + edge.GetWeight();
    }
  }

  Path(const Path &&path) noexcept : edges(std::move(path.edges)), distance(path.distance)
  {
    path.distance = TWeight();
  }

  Path() = default;
  Path(const Path &path) = default;
  Path &operator=(const Path &path) = default;
  const std::vector<TEdge> &GetEdges() const { return edges; }
  TNode GetStart() { return edges.at(0).GetTail(); }
  TNode GetDest() { return edges.at(edges.size() - 1).GetHead(); }
  TWeight GetDistance() const { return distance; }
};

template <class TWeight>
class WeightedGraph
{
 private:
  int nextNodeId = 1;
  using Edge = WeightedEdge<TWeight>;
  std::map<int, Node<Edge>> nodes;

  const WeightedGraph &Copy(const WeightedGraph &graph)
  {
    this->nodes = graph.nodes;

    // fix edges
    for (auto &nodeEntry : nodes)
    {
      for (auto &edgeEntry : nodeEntry.second.outgoingEdges)
      {
        SetEdge(edgeEntry.second.GetTail().GetId(), edgeEntry.second.GetHead().GetId(),
                edgeEntry.second.GetWeight());
      }
    }

    return *this;
  }

 public:
  WeightedGraph() = default;

  WeightedGraph(const WeightedGraph &graph) { this->Copy(graph); }

  WeightedGraph &operator=(const WeightedGraph &graph) { this->Copy(graph); }

  ImmutableNode<Edge> AddNode()
  {
    Node<Edge> node(nextNodeId++);
    return nodes.insert_or_assign(node.id, node).first->second;
  }

  std::map<int, ImmutableNode<Edge>> AddNodesRetMap(int count)
  {
    std::map<int, const Node<Edge> &> retNodes;
    for (int i = 0; i < count; i++)
    {
      const auto &node = AddNode();
      retNodes[node.id] = node;
    }
    return retNodes;
  }

  int AddNodesRetFirstId(int count)
  {
    int firstId = nextNodeId + 1;
    for (int i = 0; i < count; i++)
    {
      AddNode();
    }
    return firstId;
  }

  ImmutableNode<Edge> GetNode(int id) const { return nodes.at(id); }

  Edge SetEdge(int idTail, int idHead, TWeight weight)
  {
    Node<Edge> &tail = nodes.at(idTail);
    Node<Edge> &head = nodes.at(idHead);
    Edge edge(tail, head, weight);

    return nodes.at(idTail).outgoingEdges.insert_or_assign(idHead, edge).first->second;
  }

  void RemoveEdge(int idTail, int idHead) { nodes[idTail].outgoingEdges.erase(idHead); }

  Edge GetEdge(int idTail, int idHead) const { return nodes.at(idTail).outgoingEdges.at(idHead); }

  bool HasEdge(int idTail, int idHead) const
  {
    auto nodeIt = nodes.find(idTail);
    if (nodeIt != nodes.end())
    {
      return nodeIt->second.outgoingEdges.find(idTail) != nodeIt->second.outgoingEdges.end();
    }

    return false;
  }

  int GetNodeCount() const { return nextNodeId - 1; }

  // maps destinations ids onto paths to destinations
  std::map<int, std::shared_ptr<Path<ImmutableNode<Edge>, Edge, TWeight>>> Dijkstra(int startId)
  {
    using Path = Path<ImmutableNode<Edge>, Edge, TWeight>;
    using PathPtr = std::shared_ptr<Path>;

    auto cmp = [](const PathPtr &a, const PathPtr &b) { return a->GetDistance() > b->GetDistance(); };

    std::map<int, PathPtr> visitedPaths;
    std::priority_queue<PathPtr, std::vector<PathPtr>, decltype(cmp)> queue(cmp);

    auto curNode = GetNode(startId);
    visitedPaths[curNode.GetId()] = std::make_shared<Path>();

    for (const std::pair<int, Edge> &pair : curNode)
    {
      Edge edge = pair.second;
      std::vector<Edge> vec;
      vec.push_back(edge);
      queue.push(std::make_shared<Path>(std::move(vec)));
    }

    while (!queue.empty())
    {
      PathPtr curPath = queue.top();
      queue.pop();
      curNode = curPath->GetDest();

      auto prevPathIt = visitedPaths.find(curNode.GetId());
      if (prevPathIt == visitedPaths.end() || prevPathIt->second->GetDistance() > curPath->GetDistance())
      {
        visitedPaths[curNode.GetId()] = curPath;
      }

      if(prevPathIt != visitedPaths.end())
      {
        continue;
      }

      for (const std::pair<int, Edge> &pair : curNode)
      {
        Edge edge = pair.second;
        std::vector<Edge> edges = curPath->GetEdges();
        edges.push_back(edge);
        queue.push(std::make_shared<Path>(std::move(edges)));
      }
    }

    return visitedPaths;
  }
};

}  // namespace Graph::Oriented