#include <algorithm>
#include <cstddef>
#include <format>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <ranges>
#include <set>
#include <sstream>
#include <tuple>
#include <utility>
#include <vector>

typedef std::vector<std::vector<float>> Graph;

class Costs {
  std::map<size_t, float> costs;
  std::set<size_t> finished;

public:
  float get(size_t i) {
    if (costs.count(i) == 0) {
      // lazy insert
      costs.emplace(i, std::numeric_limits<float>::max());
    }
    return costs[i];
  }

  void update(size_t i, float c) {
    costs[i] = c;
    std::cout << std::format("update cost of ({}) to {}\n", i, c);
  }

  void finish(size_t i) {
    std::cout << std::format("finished probing from ({})\n", i);
    finished.insert(i);
  }

  void push(size_t i, float c) { costs.emplace(i, c); }

  void show() {
    std::cout << "[*] costs for all nodes: ";
    for (const auto &x : costs) {
      std::cout << std::format("[{}]:{} ", x.first, x.second);
    }
    std::cout << "\n\n";
  }

  std::optional<size_t> peek() {
    if (finished.size() == costs.size())
      return std::nullopt;

    auto not_finished = costs | std::ranges::views::keys |
                        std::ranges::views::filter([&](auto node) {
                          return finished.count(node) == 0;
                        });
    size_t min_node = 0;
    auto min_cost = std::numeric_limits<float>::max();
    for (const auto &x : not_finished) {
      if (costs[x] < min_cost) {
        min_cost = costs[x];
        min_node = x;
      }
    }
    return min_node;
  }
};

// from, to, weight
Graph read_graph() {
  std::cout << "input sample:\n3 4 1.3\nto set the weight of edge from node 3 "
               "to node 4 to be 1.3\ninput others to end input...\n";
  size_t from, to, max_node = 0;
  float weight;
  std::vector<std::tuple<size_t, size_t, float>> edges;
  while (std::cin >> from >> to >> weight) {
    max_node = std::max(std::max(from, to), max_node);
    edges.emplace_back(from, to, weight);
  }
  auto G = std::vector(max_node + 1, std::vector<float>(max_node + 1, 0));
  for (auto &x : edges) {
    from = std::get<0>(x);
    to = std::get<1>(x);
    weight = std::get<2>(x);
    G[from][to] = weight;
  }
  return std::move(G);
}

std::pair<size_t, size_t> read_start_end() {
  std::cout << "input start and end node:\n";
  int start, end;
  std::cin.clear();
  std::string line;
  while (std::getline(std::cin, line)) {
    std::istringstream iss(line);
    if (iss >> start >> end)
      break;
  }
  return std::pair(start, end);
}

// returns paths, length
std::pair<std::vector<size_t>, double> dijkstra(const Graph &G, size_t start,
                                                size_t end) {
  size_t size = G.size();
  if (start > size - 1 || end > size - 1) {
    std::cerr << std::format(
        "out of boundry(biggest node in graph: {}), node start: {}, end: {}\n", size - 1,
        start, end);
  }
  auto parents = std::vector<size_t>(size, -1); // parents[i]: parent of node i
  Costs costs;
  costs.update(start, 0);
  while (auto n = costs.peek()) {
    auto nv = n.value();
    std::cout << std::format("probing from ({})\n", nv);
    auto cost_nv = costs.get(nv);
    float cost_step, cost_new, cost_old; // for neighbor nodes
    // find all nodes that can be accessed from node n
    for (size_t i = 0; i < size; i++) {
      cost_step = G[nv][i];
      if (cost_step) {
        // otherwise there is no edge.
        cost_new = cost_nv + cost_step;
        cost_old = costs.get(i);
        std::cout << std::format("to ({}), new cost {}, old_cost {}\n",
                                 nv, i, cost_new, cost_old);
        if (cost_new < cost_old) {
          // found a shorter path from n to i
          costs.update(i, cost_new);
          parents[i] = nv;
        }
      }
    }
    costs.finish(nv);
    costs.show();
  }

  // get results
  std::vector path{end};
  double len = costs.get(end);
  size_t n = end;
  while (n != start) {
    n = parents[n]; // if n == -1, failed.
    path.insert(path.begin(), n);
  }
  return std::pair(std::move(path), len);
}

const void print_path(const std::vector<size_t> &path) {
  std::cout << path[0];
  for (size_t i = 1; i < path.size(); i++) {
    std::cout << " -> " << path[i];
  }
  std::cout << std::endl;
}

int main() {
  auto G = read_graph();
  auto [start, end] = read_start_end();
  auto [path, length] = dijkstra(G, start, end);
  std::cout << std::format(
      "The shortest path(length: {}) from node {} to node {} is:\n", length,
      start, end);
  print_path(path);
}
