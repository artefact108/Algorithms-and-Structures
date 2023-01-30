#include <iostream>
#include <set>
#include <vector>

const long long INF = 1 << 30;

struct Edge {
  long long from;
  long long to;
  long long cap;
  long long cost;

  Edge() = default;
  Edge(long long from_, long long to_, long long cap_, long long cost_)
      : from(from_), to(to_), cap(cap_), cost(cost_) {}
};

template <bool is_oriented> class Graph {
public:
  Graph() = default;
  explicit Graph(long long num_vertices)
      : num_vertices_(num_vertices),
        vertex_incidence_(std::vector<std::vector<long long>>(1050)),
        p(std::vector<long long>(1050)), par(std::vector<long long>(1050)),
        distances(std::vector<long long>(1050, INF)) {}

  void AddEdge(long long from, long long to, long long cap, long long cost) {
    Edge current(from, to, cap, cost);
    edges.push_back(current);
    vertex_incidence_[from].push_back(edges.size() - 1);

    if constexpr (!is_oriented) {
      Edge current(to, from, 0, -cost);
      edges.push_back(current);

      vertex_incidence_[to].push_back(edges.size() - 1);
    }
  }

  Graph<false> &operator=(Graph<false> &second) {
    if (*this != second) {
      num_vertices_ = second.num_vertices_;
      vertex_incidence_ = second.vertex_incidence_;
    }
    return *this;
  }

  bool operator==(Graph &second) const {
    return (num_vertices_ == second.num_vertices_) &&
           (vertex_incidence_ == second.vertex_incidence_);
  }

  std::vector<std::vector<long long>> &GetNeighboursList() {
    return vertex_incidence_;
  }

  void FordBellman(long long start) {

    distances[1] = 0;
    while (true) {
      bool check = false;
      for (long long j = 0; j < edges.size(); ++j) {
        if (distances[edges[j].from] < INF &&
            distances[edges[j].to] > distances[edges[j].from] + edges[j].cost) {
          distances[edges[j].to] = distances[edges[j].from] + edges[j].cost;
          check = true;
        }
      }
      if (!check) {
        break;
      }
    }
  }

  void GetPath(long long from) {
    std::set<std::pair<long long, long long>> edges_;
    distances.assign(1050, INF);
    edges_.insert({0, 1});
    distances[from] = 0;
    while (!edges_.empty()) {
      long long current_vertex = (*(edges_.begin())).second;
      long long current_dist = (*(edges_.begin())).first;
      edges_.erase(edges_.begin());
      for (long long i = 0; i < vertex_incidence_[current_vertex].size(); ++i) {
        long long num = vertex_incidence_[current_vertex][i];
        long long vertex = edges[num].to;
        long long weight =
            edges[num].cost + p[current_vertex] - p[edges[num].to];
        if (edges[num].cap > 0 &&
            distances[edges[num].to] > weight + current_dist) {
          // if (edges_.find({distances[vertex], vertex}) != edges_.end()) {
          edges_.erase({distances[vertex], vertex});
          //}
          distances[vertex] = current_dist + weight;
          par[vertex] = num;
          edges_.insert({distances[vertex], vertex});
        }
      }
    }
  }

  long long MinCost() {
    long long ans = 0;
    while (true) {
      GetPath(1);
      for (long long i = 0; i <= num_vertices_; ++i)
        p[i] = distances[i];

      if (distances[num_vertices_] == INF) {
        //    std::cout << "i am here "  << "\n";
        break;
        return ans;
      }
      for (long long i = 0; i <= num_vertices_; i++)
        p[i] = std::min(p[i] + distances[i], INF);
      long long push = INF;
      long long cur = num_vertices_;
      while (cur != 1) {
        //  std::cout << "i am here 2"    << "\n";

        Edge current_edge = edges[par[cur]];
        push = std::min(push, current_edge.cap);
        cur = current_edge.from;
      }
      cur = num_vertices_;
      while (cur != 1) {
        //    std::cout << "i am here" << cur << "\n";
        Edge current_edge = edges[par[cur]];
        edges[par[cur]].cap -= push;
        edges[par[cur] ^ 1].cap += push;
        ans += push * current_edge.cost;
        cur = current_edge.from;
      }
    }
    return ans;
  }

  long long Size() { return num_vertices_; }

  void ReadGraph(long long number_of_edges) {
    long long vertex_1;
    long long vertex_2;
    long long weight;
    for (long long i = 0; i < number_of_edges; ++i) {
      std::cin >> vertex_1 >> vertex_2 >> weight;
      AddEdge(vertex_1, vertex_2, weight);
    }
  }

private:
  long long num_vertices_ = 0;
  std::vector<std::vector<long long>> vertex_incidence_;
  std::vector<long long> distances;
  std::vector<Edge> edges;
  std::vector<long long> p;
  std::vector<long long> par;
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  long long n, m, v1, v2, cap, cost;
  std::cin >> n >> m;
  Graph<false> g(n);
  // for (long long i = 1; i <= n; ++i) {
  //  g.AddEdge(0, i, 1, 0);
  // }
  // for (long long i = 1; i <= n; ++i) {
  //   g.AddEdge(i + n, 2 * n + 1, 1, 0);
  // }
  for (long long i = 0; i < m; ++i) {
    std::cin >> v1 >> v2 >> cap >> cost;
    g.AddEdge(v1, v2, cap, cost);
  }
  std::cout << g.MinCost();
}
