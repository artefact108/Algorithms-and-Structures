#include <iostream>
#include <vector>

template <bool IsOriented> class Graph {
public:
  explicit Graph(int num_vertices)
      : num_vertices_(num_vertices),
        vertex_incidence(std::vector<std::vector<int>>(num_vertices + 1)) {}

  void AddEdge(int from, int to) {
    vertex_incidence[from].push_back(to);
    if constexpr (!IsOriented) {
      vertex_incidence[to].push_back(from);
    }
  }

  void Dfs(int start, std::vector<int> &used) {
    if (has_cycle) {
      return;
    }
    used[start] = 1;
    cycle.push_back(start);
    for (int i = 0; i < vertex_incidence[start].size(); ++i) {
      if (used[vertex_incidence[start][i]] == 0) {
        Dfs(vertex_incidence[start][i], used);
      } else {
        if (used[vertex_incidence[start][i]] == 1) {
          cycle.push_back(vertex_incidence[start][i]);
          has_cycle = true;
          return;
        }
      }
      if (has_cycle) {
        return;
      }
    }
    used[start] = 2;
    cycle.pop_back();
  }

  void CheckCycle() {
    std::vector<int> used(num_vertices_ + 1, 0);
    for (int i = 1; i <= num_vertices_; ++i) {
      if (used[i] == 0) {
        Dfs(i, used);
        if (has_cycle) {
          return;
        }
      }
    }
  }

  void PrintCycle() {
    int vertex = cycle.back();
    size_t i = cycle.size() - 2;
    while (cycle[i] != vertex) {
      i--;
    }
    for (size_t j = i; j < cycle.size() - 1; ++j) {
      std::cout << cycle[j] << " ";
    }
  }

  bool HasCycle() { return has_cycle; }

private:
  bool has_cycle = false;
  std::vector<int> cycle;
  int num_vertices_ = 0;
  std::vector<std::vector<int>> vertex_incidence;
};

int main() {
  int n, m, u, v;
  std::cin >> n >> m;
  Graph<true> g(n + 1);
  for (int i = 0; i < m; ++i) {
    std::cin >> u >> v;
    g.AddEdge(u, v);
  }
  g.CheckCycle();
  if (!g.HasCycle()) {
    std::cout << "NO";
  } else {
    std::cout << "YES" << '\n';
    g.PrintCycle();
  }
}
