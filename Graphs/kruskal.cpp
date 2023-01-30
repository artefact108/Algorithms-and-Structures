#include <algorithm>
#include <iostream>
#include <vector>

class DisjointSetUnion {
private:
  std::vector<std::pair<int, int>> dsu_;

public:
  DisjointSetUnion() = default;
  DisjointSetUnion(int n) {
    dsu_.push_back({0, 0});
    for (int i = 0; i < n; i++) {
      dsu_.push_back(std::make_pair(i + 1, 1));
    }
  }

  DisjointSetUnion &operator=(DisjointSetUnion &second) {
    if (*this == second) {
      return *this;
    } else {
      dsu_ = second.dsu_;
      return *this;
    }
  }

  bool operator==(DisjointSetUnion &second) const {
    return dsu_ == second.dsu_;
  }

  int Root(int a) {
    if (dsu_[a].first == a) {
      return a;
    }
    return dsu_[a].first = Root(dsu_[a].first);
  }

  void ToUnion(int a, int b) {
    a = Root(dsu_[a].first);
    b = Root(dsu_[b].first);
    if (a != b) {
      if (dsu_[a].second > dsu_[b].second) {
        dsu_[b].first = a;
        dsu_[a].second += dsu_[b].second;

      } else {
        dsu_[a].first = b;
        dsu_[b].second += dsu_[a].second;
      }
    }
  }
};

template <bool is_oriented> class Graph {
public:
  Graph() = default;
  explicit Graph(int num_vertices)
      : num_vertices_(num_vertices),
        vertex_incidence_(
            std::vector<std::vector<std::pair<int, int>>>(num_vertices + 1)) {}

  void AddEdge(int from, int to, int weight) {
    list_of_edges_.push_back(
        {weight, {std::min(from, to), std::max(to, from)}});
    vertex_incidence_[from].push_back({to, weight});
    if constexpr (!is_oriented) {
      vertex_incidence_[to].push_back({from, weight});
    }
  }

  Graph<false> &operator=(Graph<false> &second) {
    if (*this != second) {
      num_vertices_ = second.num_vertices_;
      vertex_incidence_ = second.vertex_incidence_;
      list_of_edges_ = second.list_of_edges_;
    }
    return *this;
  }

  bool operator==(Graph &second) const {
    return (num_vertices_ == second.num_vertices_) &&
           (vertex_incidence_ == second.vertex_incidence_);
  }

  std::vector<std::pair<int, std::pair<int, int>>> GetEdges() {
    return list_of_edges_;
  }

  int Size() { return num_vertices_; }

  void ReadGraph(int number_of_edges) {
    int vertex_1;
    int vertex_2;
    int weight;
    for (int i = 0; i < number_of_edges; ++i) {
      std::cin >> vertex_1 >> vertex_2 >> weight;
      AddEdge(vertex_1, vertex_2, weight);
    }
  }

private:
  int num_vertices_ = 0;
  std::vector<std::vector<std::pair<int, int>>> vertex_incidence_;
  std::vector<std::pair<int, std::pair<int, int>>> list_of_edges_;
};

class MSTBuilder {
public:
  virtual int MST(Graph<false>) = 0;
};

class KruskalMethod : public MSTBuilder {
private:
  DisjointSetUnion dsu_;

public:
  KruskalMethod() = default;

  int MST(Graph<false> gr) {
    DisjointSetUnion dsu1(gr.Size());
    dsu_ = dsu1;
    long long ans = 0;
    auto list_of_edges = gr.GetEdges();
    sort(list_of_edges.begin(), list_of_edges.end());
    for (size_t i = 0; i < list_of_edges.size(); ++i) {
      if (dsu_.Root(list_of_edges[i].second.first) !=
          dsu_.Root(list_of_edges[i].second.second)) {
        ans += list_of_edges[i].first;
        dsu_.ToUnion(list_of_edges[i].second.first,
                     list_of_edges[i].second.second);
      }
    }
    return ans;
  }
};

int main() {
  int num_of_vertices;
  int num_of_edges;
  std::cin >> num_of_vertices >> num_of_edges;
  Graph<false> g(num_of_vertices);
  g.ReadGraph(num_of_edges);
  KruskalMethod solution;
  std::cout << solution.MST(g);
}
