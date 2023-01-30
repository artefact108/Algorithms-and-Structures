#include <iostream>
#include <vector>

int main() {
  int n;
  std::cin >> n;
  std::vector<std::vector<int>> minimal_weight(n + 1, std::vector<int>(n, 0));
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      std::cin >> minimal_weight[i][j];
    }
  }
  for (int k = 0; k < n; ++k) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        minimal_weight[i][j] = std::min(
            minimal_weight[i][j], minimal_weight[i][k] + minimal_weight[k][j]);
      }
    }
  }
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      std::cout << minimal_weight[i][j] << " ";
    }
    std::cout << "\n";
  }
}
