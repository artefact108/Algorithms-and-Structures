#include <iostream>
#include <vector>

bool has_cycle = false;

std::vector<int> sorted_graph;

void top_sort(int v, std::vector<std::vector<int>> &graph,
              std::vector<int> &used) {
  used[v] = 1;
  for (int i = 0; i < graph[v].size(); ++i) {
    if (used[graph[v][i]] == 1) {
      has_cycle = true;
      break;
    }
    if (used[graph[v][i]] == 0) {
      top_sort(graph[v][i], graph, used);
    }
  }
  used[v] = 2;
  sorted_graph.push_back(v);
}

int main() {
  int n, m, u, v;
  std::cin >> n >> m;
  std::vector<std::vector<int>> graph(m + n);
  for (int i = 0; i < m; ++i) {
    std::cin >> u >> v;
    graph[u - 1].push_back(v - 1);
  }
  std::vector<int> used(n, 0);
  for (int i = 0; i < n; ++i) {
    if (used[i] == 0) {
      top_sort(i, graph, used);
      if (has_cycle) {
        break;
      }
    }
  }
  if (has_cycle) {
    std::cout << -1;
  } else {
    for (int i = sorted_graph.size() - 1; i >= 0; i--) {
      std::cout << sorted_graph[i] + 1 << " ";
    }
  }
}
