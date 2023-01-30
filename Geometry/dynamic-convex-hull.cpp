#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>
#include <vector>

template <typename T> class Point {
public:
  T x;
  T y;
  Point(T x_, T y_) : x(x_), y(y_){};

  T Distance(const Point<T> &p) {
    T x_ = p.x - x;
    T y_ = p.y - y;
    return sqrt(x_ * x_ + y_ * y_);
  }

  bool operator<(const Point &p) const {
    return (x < p.x) || (x == p.x && y < p.y);
  }

  bool operator==(const Point<T> &p) const { return (x == p.x && y == p.y); }
};

// template <typename T> bool cmp(const Point<T> &p1, const Point<T> &p2) {
//   return (p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y));
// }

template <typename T> class Vector {
public:
  T x;
  T y;
  Vector(T x_, T y_) : x(x_), y(y_){};

  Vector(const Point<T> &p_1, const Point<T> &p_2)
      : x(p_2.x - p_1.x), y(p_2.y - p_1.y) {}

  T length() const { return sqrt(x * x + y * y); }

  T product(const Vector<T> &v) const { return x * 1LL * v.y - (y)*1LL * v.x; }

  T scalar_prod(const Vector<T> &v) const { return x * v.x + y * v.y; }
};

template <typename T>
T OrientValue(const Point<T> &p1, const Point<T> &p2, const Point<T> &p3) {
  Vector<T> v1(Point<T>(0, 0), p1);
  Vector<T> v2(Point<T>(0, 0), p2);
  Vector<T> v3(Point<T>(0, 0), p3);
  return (v1.product(v2) + v2.product(v3) + v3.product(v1));
}

template <typename T>
bool IsOriented(const Point<T> &p1, const Point<T> &p2, const Point<T> &p3) {
  return (OrientValue(p1, p2, p3) > 0);
}

template <typename T>
bool IsIntersectLine(const Point<T> &a, const Point<T> &b,
                     const Point<T> &begin, const Point<T> &end) {

  T prod1 = Vector<T>(a, b).product(Vector<T>(a, begin)); // AC

  T prod2 = Vector<T>(a, b).product(Vector<T>(a, end)); // AD

  T prod4 = Vector<T>(end, begin).product(Vector<T>(end, a));

  T prod5 = Vector<T>(end, begin).product(Vector<T>(end, b));

  return (prod1 * prod2 <= 0 && prod4 * prod5 <= 0);
}

bool AddInLower(std::set<Point<long long>> &lower, const Point<long long> &p) {
  auto iter = lower.lower_bound(p);
  if (iter == lower.begin() || iter == lower.end()) {
    return true;
  }

  auto prev_it = std::prev(iter);
  Vector<long long> v_1(*prev_it, *iter);
  Vector<long long> v_2(*prev_it, p);

  // std::cout << "v_1: " << v_1.x << " " << v_1.y << '\n';
  // std::cout << "v_2: " << v_2.x << " " << v_2.y << '\n';

  return (v_2.product(v_1) > 0);
}

bool AddInUpper(std::set<Point<long long>> &upper, const Point<long long> &p) {
  auto iter = upper.lower_bound(p);
  if (iter == upper.begin() || iter == upper.end()) {
    return true;
  }
  auto prev_it = std::prev(iter);
  Vector<long long> v_1(*prev_it, *iter);
  Vector<long long> v_2(*prev_it, p);
  //  std::cout << "u v_1: " << v_1.x << " " << v_1.y << '\n';
  // std::cout << "u v_2: " << v_2.x << " " << v_2.y << '\n';

  return (v_1.product(v_2) > 0);
}

void addPoint(std::set<Point<long long>> &lower,
              std::set<Point<long long>> &upper, Point<long long> p) {
  // add a point long

  // if in upper
  if (AddInUpper(upper, p)) {
    upper.insert(p);
    auto iter = upper.find(p);

    auto next_iter = std::next(iter);
    while (next_iter != upper.end() && std::next(next_iter) != upper.end()) {
      auto next_next_iter = std::next(next_iter);
      Vector<long long> v_1(p, *next_iter);
      Vector<long long> v_2(*next_iter, *next_next_iter);
      if (v_1.product(v_2) >= 0) {
        upper.erase(next_iter);
        next_iter = std::next(iter);
      } else {
        break;
      }
    }

    auto prev_iter = std::prev(iter);
    while (iter != upper.begin() && prev_iter != upper.begin()) {
      auto prev_prev_iter = std::prev(prev_iter);
      Vector<long long> v_1(p, *prev_iter);
      Vector<long long> v_2(*prev_iter, *prev_prev_iter);
      if (v_1.product(v_2) <= 0) {
        upper.erase(prev_iter);
        prev_iter = std::prev(iter);
      } else {
        break;
      }
    }
  }

  if (AddInLower(lower, p)) {

    lower.insert(p);
    auto iter = lower.lower_bound(p);

    auto next_iter = std::next(iter);
    while (next_iter != lower.end() && std::next(next_iter) != lower.end()) {
      auto next_next_iter = std::next(next_iter);
      Vector<long long> v_1(p, *next_iter);
      Vector<long long> v_2(*next_iter, *next_next_iter);
      if (v_1.product(v_2) <= 0) {
        lower.erase(next_iter);
        next_iter = std::next(iter);
      } else {
        break;
      }
    }

    auto prev_iter = std::prev(iter);
    while (iter != lower.begin() && prev_iter != lower.begin()) {
      auto prev_prev_iter = std::prev(prev_iter);
      Vector<long long> v_1(p, *prev_iter);
      Vector<long long> v_2(*prev_iter, *prev_prev_iter);
      if (v_1.product(v_2) >= 0) {
        lower.erase(prev_iter);
        prev_iter = std::prev(iter);
      } else {
        break;
      }
    }
  }
}

template <typename T>
std::pair<std::set<Point<T>>, std::set<Point<T>>>
ConvexHull(const std::vector<Point<T>> &C) {
  std::vector<Point<T>> our_points;
  for (auto &point : C) {
    our_points.push_back(point);
  }
  std::sort(our_points.begin(), our_points.end() /*&cmp<T>*/);

  Point<T> point1 = our_points[0];
  Point<T> point2 = our_points.back();
  std::vector<Point<T>> up, down;
  up.push_back(point1);
  down.push_back(point1);

  for (size_t i = 1; i < our_points.size(); ++i) {
    if (i == our_points.size() - 1 ||
        IsOriented(point1, our_points[i], point2)) {
      while (up.size() >= 2 &&
             !IsOriented(up[up.size() - 2], up.back(), our_points[i])) {
        up.pop_back();
      }
      up.push_back(our_points[i]);
    }
    if (i == our_points.size() - 1 ||
        (OrientValue<T>(point1, our_points[i], point2) != 0 &&
         !IsOriented<T>(point1, our_points[i], point2))) {
      while (down.size() >= 2 &&
             (OrientValue(down[down.size() - 2], down.back(), our_points[i]) >=
              0)) {
        down.pop_back();
      }
      down.push_back(our_points[i]);
    }
  }
  our_points.clear();
  std::set<Point<T>> upper;
  std::set<Point<T>> lower;
  for (size_t i = 0; i < up.size(); ++i) {
    upper.insert(up[i]);
  }
  upper.insert(point2);
  for (long long i = down.size() - 1; i >= 0; --i)
    lower.insert(down[i]);
  return {upper, lower};
}

int main() {

  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  long long q, type;
  long long x, y;
  std::vector<Point<long long>> points;

  std::cin >> q;
  long long cnt = 0;
  bool was_second = false;
  std::set<Point<long long>> lower;
  std::set<Point<long long>> upper;
  while (q > 0) {
    q--;
    ++cnt;
    std::cin >> type >> x >> y;
    Point<long long> newpoint(x, y);
    if (type == 1) {
      if (cnt < 4) {
        points.push_back(newpoint);
      } else {
        addPoint(lower, upper, newpoint);
      }
    } else {
      if (!AddInLower(lower, newpoint) && !AddInUpper(upper, newpoint)) {
        std::cout << "YES" << '\n';
      } else {
        std::cout << "NO" << '\n';
      }
    }
    if (cnt == 3) {
      was_second = true;
      auto ch = ConvexHull(points);
      upper = ch.second;
      lower = ch.first;
    }
  }
}
