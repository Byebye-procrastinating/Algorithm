// graph.hpp
#pragma once
#include <algorithm>
#include <functional>
#include <map>
#include <numeric>
#include <vector>

struct Edge {
  int from, to, weight;
  Edge() = default;
  Edge(int _from, int _to, int _weight = 1):
      from(_from), to(_to), weight(_weight) {}
};

class Graph {
public:
  Graph() = default;
  Graph(int _node_num): node_num(_node_num) { edges.resize(_node_num); }

  int size() const { return node_num; }
  // 加入一条由 from 到 to, 边权为 weight 的无向边
  void addEdge(int from, int to, int weight = 1) {
    edges[from].push_back(Edge(from, to, weight));
    edges[to].push_back(Edge(to, from, weight));
  }

  const std::vector<Edge>& operator[] (const int& idx) const {
    return edges[idx];
  }
  std::vector<Edge>& operator[] (const int& idx) { return edges[idx]; }

private:
  int node_num;
  std::vector<std::vector<Edge>> edges;
};