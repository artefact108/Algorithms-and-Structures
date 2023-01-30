#pragma GCC optimize("Ofast,no-stack-protector")
#include <iostream>
#include <map>
#include <vector>

std::vector<std::vector<int>> g;
std::vector<bool> used;
std::vector<int> component;
int timer = 0;
int number = 0;
int current_number;
std::vector<int> tin;
std::vector<int> tup;
std::map<std::pair<int, int>, std::vector<int>> multiple;

void dfs(int v, int p) {
  timer++;
  tin[v] = timer;
  tup[v] = timer;
  used[v] = true;
  for (int i = 0; i < g[v].size(); ++i) {
    int u = g[v][i];
    if (u != p) {
      if (used[u]) {
        tup[v] = std::min(tin[u], tup[v]);
      } else {
        dfs(u, v);
        tup[v] = std::min(tup[v], tup[u]);
      }
    }
  }
}

void MakeComponents(int v, int p, int current_number) {
  used[v] = true;
  for (int i = 0; i < g[v].size(); ++i) {
    if (g[v][i] != p) {
      if (!used[g[v][i]]) {
        if (tin[v] <= tup[g[v][i]]) {
          number++;
          for (int j = 0;
               j <
               multiple[{std::min(v, g[v][i]), std::max(v, g[v][i])}].size();
               ++j) {
            component[multiple[{std::min(v, g[v][i]), std::max(v, g[v][i])}]
                              [j]] = number - 1;
          }
          MakeComponents(g[v][i], v, number - 1);
        } else {
          for (int j = 0;
               j <
               multiple[{std::min(v, g[v][i]), std::max(v, g[v][i])}].size();
               ++j) {
            component[multiple[{std::min(v, g[v][i]), std::max(v, g[v][i])}]
                              [j]] = current_number;
          }
          MakeComponents(g[v][i], v, current_number);
        }
      } else {
        if (tin[g[v][i]] < tin[v]) {
          for (int j = 0;
               j <
               multiple[{std::min(v, g[v][i]), std::max(v, g[v][i])}].size();
               ++j) {
            component[multiple[{std::min(v, g[v][i]), std::max(v, g[v][i])}]
                              [j]] = current_number;
          }
        }
      }
    }
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n, m, u, v;
  std::cin >> n >> m;
  g.resize(n + 1);
  tin.assign(n + 1, 0);
  tup.assign(n + 1, 0);

  for (int i = 0; i < m; ++i) {
    std::cin >> u >> v;
    u++;
    v++;
    g[u].push_back(v);
    g[v].push_back(u);
    multiple[{std::min(u, v), std::max(u, v)}].push_back(i);
  }
  component.assign(m + 1, 0);
  used.assign(n + 1, false);
  for (int i = 1; i <= n; ++i) {
    if (!used[i]) {
      dfs(i, 0);
    }
  }
  used.assign(n + 1, false);
  number = 0;
  for (int i = 1; i <= n; ++i) {
    if (!used[i]) {
      MakeComponents(i, 0, number);
    }
  }
  std::cout << number << "\n";
  std::map<int, std::vector<int>> ans;
  for (int i = 0; i < m; ++i) {
    ans[component[i]].push_back(i);
  }
  for (int i = 0; i < number; ++i) {
    std::cout << ans[i].size() << " ";
    for (int j = 0; j < ans[i].size(); ++j) {
      std::cout << ans[i][j] << " ";
    }
    std::cout << '\n';
  }
}
