#include <iostream>
#include <set>
#include <vector>

const long long kInf = 1e9;

template <bool IsOriented> class Graph {
public:
  explicit Graph(int num_vertices)
      : num_vertices_(num_vertices),
        vertex_incidence(
            std::vector<std::vector<std::pair<int, int>>>(num_vertices + 1)) {}

  void AddEdge(int from, int to, int weight) {
    vertex_incidence[from].push_back({to, weight});
    if constexpr (!IsOriented) {
      vertex_incidence[to].push_back({from, weight});
    }
  }

  std::vector<std::pair<int, int>> GetNeighbours(int vertex) {
    return vertex_incidence[vertex];
  }

  int GetPath(int from, int to) {
    std::set<std::pair<int, int>> edges;
    std::vector<int> distances(num_vertices_ + 1, kInf);
    edges.insert({0, from});
    distances[from] = 0;
    while (!edges.empty()) {
      int current_vertex = (*(edges.begin())).second;
      edges.erase(edges.begin());

      for (int i = 0; i < vertex_incidence[current_vertex].size(); ++i) {
        int vertex = vertex_incidence[current_vertex][i].first;
        int weight = vertex_incidence[current_vertex][i].second;
        if ((distances[vertex] > distances[current_vertex] + weight)) {
          if (distances[vertex] != kInf) {
            edges.erase(edges.find({distances[vertex], vertex}));
          }
          distances[vertex] = distances[current_vertex] + weight;
          edges.insert({distances[vertex], vertex});
        }
      }
    }
    return (distances[to] == kInf ? -1 : distances[to]);
  }

private:
  int num_vertices_ = 0;
  std::vector<std::vector<std::pair<int, int>>> vertex_incidence;
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n, m, start, finish;
  std::cin >> n >> m;
  std::cin >> start >> finish;
  int from, to, weight;
  Graph<false> g(n);
  for (int i = 0; i < m; ++i) {
    std::cin >> from >> to >> weight;
    g.AddEdge(from, to, weight);
  }
  std::cout << g.GetPath(start, finish);
}
