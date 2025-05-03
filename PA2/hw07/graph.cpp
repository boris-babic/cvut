#ifndef __PROGTEST__
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <compare>
#include <cstdlib>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <set>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std::literals;

class CDumbString {
 public:
  CDumbString(std::string v) : m_Data(std::move(v)) {}
  bool operator==(const CDumbString& rhs) const = default;

 private:
  std::string m_Data;
};
#endif /* __PROGTEST__ */

template <typename T>
class CNet {
 public:
  // default constructor
  // add ( x, y, cost )
  CNet& add(const T& x, const T& y, int cost) {
    int x_index = get_node_index(x);
    int y_index = get_node_index(y);
    this->edges[x_index].emplace_back(y_index, cost);
    this->edges[y_index].emplace_back(x_index, cost);

    return *this;
  }
  // optimize ()
  CNet& optimize() {
    for (size_t i = 0; i < nodes.size(); i++) {
      this->dijkstra(i);
    }
    return *this;
  }
  int totalCost(T x, T y) {
    auto it_x = std::find(nodes.begin(), nodes.end(), x);
    auto it_y = std::find(nodes.begin(), nodes.end(), y);
    if (it_x == nodes.end() || it_y == nodes.end()) return -1;
    int x_index = std::distance(nodes.begin(), it_x);
    int y_index = std::distance(nodes.begin(), it_y);
    if (this->cost_table[x_index][y_index] == -1)
      return -1;
    else
      return this->cost_table[x_index][y_index];
  }

 private:
  std::vector<T> nodes;
  std::vector<std::vector<std::pair<int, int>>> edges;
  std::vector<std::vector<int>> cost_table;

  int get_node_index(const T& x) {
    auto it = std::find(nodes.begin(), nodes.end(), x);
    if (it != nodes.end()) return std::distance(nodes.begin(), it);

    nodes.push_back(x);
    edges.emplace_back();
    return nodes.size() - 1;
  }

  void dijkstra(int start) {
    const int n = nodes.size();
    std::vector<int> dist(n, INT_MAX);
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,
                        std::greater<>>
        pq;

    dist[start] = 0;
    pq.emplace(0, start);

    while (!pq.empty()) {
      const auto [d, u] = pq.top();
      pq.pop();

      if (d > dist[u]) continue;

      for (const auto& [v, cost] : edges[u]) {
        if (dist[v] > dist[u] + cost) {
          dist[v] = dist[u] + cost;
          pq.emplace(dist[v], v);
        }
      }
    }

    cost_table.push_back(dist);
  }

  static int get_minimum(std::vector<int> data, std::set<int> visited) {
    int minvalue = INT32_MAX;
    int index = -1;
    for (size_t i = 0; i < data.size(); i++) {
      if (data[i] <= minvalue &&
          std::find(visited.begin(), visited.end(), i) == visited.end()) {
        minvalue = data[i];
        index = i;
      }
    }
    return index;
  }
};

#ifndef __PROGTEST__
#include <chrono>
int main() {
  auto start = std::chrono::high_resolution_clock::now();
  CNet<std::string> a;
  a.add("Adam", "Bob", 100)
      .add("Bob", "Carol", 200)
      .add("Dave", "Adam", 300)
      .add("Eve", "Fiona", 120)
      .add("Kate", "Larry", 270)
      .add("Ivan", "John", 70)
      .add("Kate", "Ivan", 300)
      .add("George", "Henry", 10)
      .add("Eve", "George", 42)
      .add("Adam", "Eve", 75)
      .add("Ivan", "George", 38)
      .add("Boris", "Slotaland", 29)
      .add("Ava", "Blake", 120)
      .add("Cleo", "Dane", 210)
      .add("Elio", "Freya", 95)
      .add("Gale", "Hazel", 185)
      .add("Iris", "Jude", 75)
      .add("Kian", "Lyra", 100)
      .add("Mira", "Nico", 140)
      .add("Orla", "Pax", 60)
      .add("Quin", "Raya", 180)
      .add("Sage", "Troy", 130)
      .add("Uma", "Viktor", 200)
      .add("Wynn", "Xena", 115)
      .add("Yves", "Zuri", 105)
      .add("Alaric", "Bria", 155)
      .add("Cyra", "Dario", 170)
      .add("Enzo", "Fauna", 145)
      .add("Gio", "Hera", 90)
      .add("Isla", "Jax", 160)
      .add("Kael", "Lumi", 85)
      .add("Myles", "Nyra", 220)
      .add("Ozzy", "Pia", 190)
      .add("Roux", "Sela", 135)
      .add("Thane", "Uri", 80)
      .add("Vera", "Wade", 150)
      .add("Xavi", "Yana", 125)
      .add("Zeno", "Anya", 110)
      .add("Bryn", "Calix", 100)
      .add("Demi", "Elric", 140)
      .add("Fawn", "Griff", 175)
      .add("Haley", "Ines", 90)
      .add("Jett", "Kora", 155)
      .add("Lior", "Macy", 115)
      .add("Nina", "Odin", 130)
      .add("Porter", "Rina", 180)
      .add("Silas", "Teya", 105)
      .add("Ulric", "Vana", 145)
      .add("Wren", "Xyla", 95)
      .add("Yuki", "Zack", 165)
      .add("Arlo", "Bea", 125)
      .add("Cass", "Dahl", 135)
      .add("Eira", "Fynn", 115)
      .add("Gaia", "Hugh", 150)
      .add("Ivo", "Jess", 140)
      .add("Kyra", "Laz", 90)
      .add("Moe", "Nell", 130)
      .add("Oona", "Perry", 120)
      .add("Rey", "Suki", 100)
      .add("Tess", "Ugo", 110)
      .add("Vik", "Willa", 170)
      .add("Xeno", "Yani", 125)
      .add("Zia", "Aksel", 105)
      .add("Bree", "Cruz", 150)
      .add("Dion", "Esme", 85)
      .add("Fife", "Gwen", 145)
      .add("Holt", "Iman", 135)
      .add("Jay", "Kali", 160)
      .add("Lark", "Mina", 180)
      .add("Nash", "Orel", 115)
      .add("Pia", "Roan", 140)
      .add("Suri", "Tobias", 190)
      .add("Uli", "Veda", 100)
      .add("Wil", "Xani", 95)
      .add("Ymir", "Zani", 130)
      .add("Aero", "Brin", 105)
      .add("Cian", "Daya", 120)
      .add("Ezra", "Fae", 135)
      .add("Gray", "Hani", 150)
      .add("Ilia", "Joan", 165)
      .add("Kace", "Lena", 115)
      .add("Milo", "Nyla", 125)
      .add("Orin", "Paxie", 90)
      .add("Rami", "Sana", 175)
      .add("Tali", "Uria", 130)
      .add("Vann", "Wylla", 110)
      .add("Xim", "Yani", 140)
      .add("Zora", "Axl", 150)
      .add("Beck", "Cleo", 100)
      .add("Dax", "Eve", 125)
      .add("Finn", "Gem", 135)
      .add("Haze", "Ione", 145)
      .add("Juno", "Kip", 155)
      .add("Lina", "Monty", 95)
      .add("Nico", "Olive", 105)
      .add("Paz", "Quila", 165)
      .add("Rune", "Sia", 175)
      .add("Thora", "Ulix", 185)
      .add("Vira", "Wynn", 120)
      .add("Xara", "Yves", 160)
      .add("Zain", "Aura", 90)
      .add("Basil", "Cira", 130)
      .add("Drea", "Egan", 140)
      .add("Flint", "Gala", 100)
      .add("Hani", "Isen", 180)
      .add("Jaya", "Kano", 190)
      .add("Luz", "Maks", 200)
      .add("Nira", "Orin", 210)
      .add("Pres", "Rae", 220)
      .optimize();

  assert(a.totalCost("Adam", "Bob") == 100);
  assert(a.totalCost("John", "Eve") == 150);
  assert(a.totalCost("Dave", "Henry") == 427);
  assert(a.totalCost("Carol", "Larry") == 1025);
  assert(a.totalCost("George", "George") == 0);
  assert(a.totalCost("Alice", "Bob") == -1);
  assert(a.totalCost("Thomas", "Thomas") == -1);

  CNet<int> b;
  b.add(0, 1, 100)
      .add(1, 2, 200)
      .add(3, 0, 300)
      .add(4, 5, 120)
      .add(10, 11, 270)
      .add(8, 9, 70)
      .add(10, 8, 300)
      .add(6, 7, 10)
      .add(4, 6, 42)
      .add(0, 4, 75)
      .add(8, 6, 38)
      .optimize();
  assert(b.totalCost(0, 1) == 100);
  assert(b.totalCost(9, 4) == 150);
  assert(b.totalCost(3, 7) == 427);
  assert(b.totalCost(2, 11) == 1025);
  assert(b.totalCost(6, 6) == 0);
  assert(b.totalCost(0, 1) == 100);
  assert(b.totalCost(19, 19) == -1);

  CNet<CDumbString> c;
  c.add("Adam"s, "Bob"s, 100)
      .add("Bob"s, "Carol"s, 200)
      .add("Dave"s, "Adam"s, 300)
      .add("Eve"s, "Fiona"s, 120)
      .add("Kate"s, "Larry"s, 270)
      .add("Ivan"s, "John"s, 70)
      .add("Kate"s, "Ivan"s, 300)
      .add("George"s, "Henry"s, 10)
      .add("Eve"s, "George"s, 42)
      .add("Adam"s, "Eve"s, 75)
      .add("Ivan"s, "George"s, 38)
      .optimize();
  assert(c.totalCost("Adam"s, "Bob"s) == 100);
  assert(c.totalCost("John"s, "Eve"s) == 150);
  assert(c.totalCost("Dave"s, "Henry"s) == 427);
  assert(c.totalCost("Carol"s, "Larry"s) == 1025);
  assert(c.totalCost("George"s, "George"s) == 0);
  assert(c.totalCost("Alice"s, "Bob"s) == -1);
  assert(c.totalCost("Thomas"s, "Thomas"s) == -1);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end - start;
  std::cout << "run time: " << duration.count() << std::endl;
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
