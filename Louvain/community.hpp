// community.hpp
#pragma once

#include <algorithm>
#include <chrono>
#include <functional>
#include <map>
#include <numeric>
#include <random>
#include <vector>

#include "graph.hpp"

// 离散化 vec 中的数值到 0 开始的整数
// 要求类型 vecType 支持 operator <
// 记 vec 大小为 n, 时间复杂度 O(n log n)
template<typename vecType>
std::vector<int> discretize(const std::vector<vecType>& vec) {
  auto tmp = vec;
  std::sort(tmp.begin(), tmp.end());
  tmp.erase(std::unique(tmp.begin(), tmp.end()), tmp.end());

  int n = vec.size();
  std::vector<int> result(n);
  for (int i = 0; i < n; ++i) {
    result[i] = lower_bound(tmp.begin(), tmp.end(), vec[i]) - tmp.begin();
  }
  return result;
}

// 给定网络 G 和一种划分 community, 计算模块度 (Modularity)
// 对于两个结点 i, j, 两者在同一社区等价于 community[i] == community[j]
// 要求 community 包含的数是 0 开始的连续整数
// 时间复杂度 O(n + m)
double Modularity(const Graph& G, const std::vector<int>& community) {
  int node_num = G.size();
  int com_num = 1 + *max_element(community.begin(), community.end());
  // inner[c]: 社区 c 中所有边的边权和. 每条边计算两次, 边的两个端点都在 c 中
  // sum_k[c]: 社区 c 中所有点对应边的边权和
  std::vector<double> inner(com_num), sum_k(com_num);
  double M2 = 0.0, Q = 0.0;
  for (int u = 0; u < node_num; ++u) {
    int c = community[u];
    for (const auto& [from, to, weight]: G[u]) {
      M2 += weight;
      sum_k[c] += weight;
      if (community[to] == c)
        inner[c] += weight;
    }
  }
  for (int c = 0; c < com_num; ++c) {
    double d0 = inner[c] / M2, d1 = sum_k[c] / M2;
    Q += d0 - d1 * d1;
  }
  return Q;
}

// Louvain
// 时间复杂度 O((n + m) log n)
std::vector<int> LouvainCommunityDetect(const Graph& G) {
  // node_num: G 的点数
  int node_num = G.size();

  // adj: G 对应邻接矩阵
  std::vector<std::unordered_map<int, int>> adj(node_num);
  // M: 所有边权和
  double M = 0.0;
  for (int u = 0; u < node_num; ++u) {
    for (const auto& [from, to, weight]: G[u]) {
      M += weight;
      adj[from][to] += weight;
    }
  }
  M /= 2.0;

  // community: G 的一种社区划分方式, 利用并查集维护
  std::vector<int> community(node_num);
  std::iota(community.begin(), community.end(), 0);
  std::function<int(int)> findfa = [&](int u) {
    return (community[u] == u) ? u : community[u] = findfa(community[u]);
  };
  auto nodes = community;

  while (true) {
    bool improvement = false;
    std::vector<double> k(node_num), d(node_num);
    for (auto from: nodes) {
      for (auto [to, weight]: adj[from])
        k[from] += weight;
      d[from] = k[from];
    }
    for (auto from: nodes) {
      int u = findfa(from), best_community = u;
      d[u] -= k[from];
      std::vector<double> inner_weight(node_num);
      for (auto [to, weight]: adj[from]) {
        if (from == to)
          continue;
        int v = findfa(to);
        inner_weight[v] += weight;
      }
      double best_delta_Q = 0.0;
      for (auto [to, weight]: adj[from]) {
        if (from == to)
          continue;
        int v = findfa(to);
        double delta_Q = inner_weight[v] / M - d[v] / M * k[from] / M / 2.0;
        if (delta_Q > best_delta_Q) {
          best_community = v;
          best_delta_Q = delta_Q;
        }
      }
      d[best_community] += k[from];
      if (best_community != u) {
        improvement = true;
        community[from] = best_community;
      }
    }
    if (!improvement)
      break;
    std::vector<int> next_nodes;
    for (int u = 0; u < node_num; ++u)
      community[u] = findfa(u);
    for (auto from: nodes)
      next_nodes.push_back(community[from]);
    std::sort(next_nodes.begin(), next_nodes.end());
    next_nodes.erase(std::unique(next_nodes.begin(), next_nodes.end()),
                     next_nodes.end());
    std::vector<std::unordered_map<int, int>> next_adj(node_num);
    for (auto from: nodes) {
      int u = findfa(from);
      for (auto [to, weight]: adj[from]) {
        int v = findfa(to);
        next_adj[u][v] += weight;
      }
    }
    std::swap(nodes, next_nodes);
    std::swap(adj, next_adj);
  }

  return discretize(community);
}

// Label Propagetion Algorithm
// 使用异步更新
std::vector<int> LabelPropagation(const Graph& G) {
  int node_num = G.size();

  std::vector<int> community(node_num);
  std::iota(community.begin(), community.end(), 0);

  auto rng_seed = std::chrono::steady_clock::now().time_since_epoch().count();
  std::mt19937 rng(rng_seed);

  while (true) {
    bool improvement = false;
    for (int u = 0; u < node_num; ++u) {
      std::unordered_map<int, int> du;
      for (auto [from, to, weight]: G[u]) {
        int v = community[to];
        du[v] += weight;
      }
      int best_du = -1;
      for (auto [com, val]: du)
        if (val > best_du) best_du = val;
      std::vector<int> choices;
      for (auto [com, val]: du)
        if (val == best_du) choices.push_back(com);
      std::shuffle(choices.begin(), choices.end(), rng);
      int best_community = *choices.begin();
      if (community[u] != best_community) {
        improvement = true;
        community[u] = best_community;
      }
    }
    if (!improvement)
      break;
  }
  
  return discretize(community);
}
