#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class CTransport {
 public:
  // konstruktor
  // destruktor

  CTransport& addLine(std::istream& is) {
    std::vector<std::string> line;
    std::string station;
    while (!is.eof()) {
      std::getline(is, station, '\n');
      line.push_back(station);
    }
    line.pop_back();
    for (size_t i = 0; i < line.size() - 1; i++) {
      connections.emplace(line[i], line[i + 1]);
      connections.emplace(line[i + 1], line[i]);
    }
    return *this;
  }

  std::set<std::string> meetingPoints(const std::vector<std::string>& fromList, int& sumCost) const {
    std::vector<std::map<std::string, int>> lines;

    if (fromList.size() == 1) {
      sumCost = 0;
      return {fromList[0]};
    }

    for (auto i: fromList) {
      lines.push_back(this->bfs(i));
    }
    for (auto station : fromList) {
      for (auto line : lines) {
        if (line.count(station) == 0) {
          return {};
        }
      }
    }
    for (auto i : lines) {
      for (auto j : i) {
        std::cout << j.first << "  -  " << j.second << std::endl;
      }
    }
    auto result = add_maps(lines);
    if (result.second < 0) {
      return {};
    }
    sumCost = result.second;
    return result.first;
  }
  void print() {
    for (auto i : connections) {
      std::cout << i.first << "  ->  " << i.second << std::endl;
    }
  }

 private:
  std::multimap<std::string, std::string> connections;

  std::map<std::string, int> bfs(const std::string & from) const {
    std::set<std::string> visited;
    std::queue<std::pair<std::string, int>> to_visit;
    std::map<std::string, int> result;
    to_visit.push(std::make_pair(from, 0));

    while (to_visit.size() > 0) {
      std::string current = to_visit.front().first;
      int current_count = to_visit.front().second;
      if (visited.find(current) != visited.end() ) {
        to_visit.pop();
        continue;
      }
      to_visit.pop();
      visited.emplace(current);
      result[current] = current_count;

      auto from_current = this->connections.equal_range(current);
      for (auto i = from_current.first; i != from_current.second; i++ ) {
        if (visited.count(i->second) == 0) to_visit.push(std::make_pair(i->second, current_count+1));
      }
    }
    return result;
  }

  static std::pair<std::set<std::string>, int> add_maps(const std::vector<std::map<std::string, int>> & data) {
    std::set<std::string> result;
    std::map<std::string, int> added(data[0]);
    for (size_t i = 1; i < data.size(); i++) {
      std::map<std::string, int> current = data[i];
      for (auto j: current) {
        if (added.count(j.first) != 0) added[j.first] += j.second;
      } 
    }
    int min_cost = -1;
    for (auto i : added) {
      if (i.second < min_cost || min_cost == -1) {
        result.clear();
        result.emplace(i.first);
        min_cost = i.second;
      } else if (i.second == min_cost) {
        result.emplace(i.first);
      }
    }
    return std::make_pair(result, min_cost);
  }
};

int main() {
  CTransport t;
  std::istringstream iss;
  iss.clear();
  iss.str(
      "Newton\n"
      "Black Hill\n"
      "Wood Side\n"
      "Green Hill\n"
      "Lakeside\n"
      "Newton\n");
  t.addLine(iss);
  iss.clear();
  iss.str(
      "Wood Side\n"
      "Green Hill\n"
      "Little Burnside\n"
      "Great Newton\n"
      "Wood Side\n");
  t.addLine(iss);
  iss.clear();
  iss.str(
      "Little Newton\n"
      "Little Burnside\n"
      "Castle Hill\n"
      "Newton Crossroad\n"
      "Lakeside Central\n"
      "Little Newton\n");
  t.addLine(iss);
  iss.clear();
  iss.str(
      "Lakeside Central\n"
      "Little Waterton\n");
  t.addLine(iss);
  iss.clear();
  iss.str(
      "Little Waterton\n"
      "Waterton West\n"
      "Waterton Central\n"
      "Waterton East\n"
      "Waterton Woods\n"
      "Waterton North\n"
      "Waterton East\n"
      "Little Waterton\n");
  t.addLine(iss);
  iss.clear();
  iss.str(
      "Tidmouth\n"
      "Gordon's Hill\n"
      "Suderry\n"
      "Knapford\n"
      "Great Waterton\n"
      "Brendam Docks\n"
      "Tidmouth\n");
  t.addLine(iss);
  iss.clear();
  iss.str(
      "Sodor Ironworks\n"
      "Sodor Steamwork\n"
      "Knapford\n"
      "Maron\n"
      "Gordon's Hill\n"
      "Sodor Ironworks\n");
  t.addLine(iss);
  t.print();
  
  int cost;
  assert(
    (t.meetingPoints({"Newton"}, cost) == std::set<std::string>{"Newton"}) &&
    (cost == 0));
    
    assert((t.meetingPoints({"Newton", "Newton"}, cost) ==
    std::set<std::string>{"Newton"}) &&
    (cost == 0));
    
    assert((t.meetingPoints({"Brno"}, cost) == std::set<std::string>{"Brno"}) &&
    (cost == 0));
    
    assert((t.meetingPoints({"Brno", "Brno"}, cost) ==
    std::set<std::string>{"Brno"}) &&
    (cost == 0));
    
    assert(
      (t.meetingPoints({"Newton", "Brno"}, cost) == std::set<std::string>{}));
      
      assert((t.meetingPoints({"Newton", "Black Hill"}, cost) ==
      std::set<std::string>{"Newton", "Black Hill"}) &&
      cost == 1);
      
      assert((t.meetingPoints({"Newton", "Newton", "Black Hill"}, cost) ==
      std::set<std::string>{"Newton"}) &&
      cost == 1);
      
      assert((t.meetingPoints({"Newton", "Black Hill", "Wood Side"}, cost) ==
      std::set<std::string>{"Black Hill"}) &&
      cost == 2);
      
      assert((t.meetingPoints({"Newton Crossroad", "Little Waterton"}, cost) ==
      std::set<std::string>{"Newton Crossroad", "Little Waterton",
        "Lakeside Central"}) &&
        cost == 2);
        
        assert((t.meetingPoints({"Suddery", "Little Waterton"}, cost) ==
        std::set<std::string>{}));
        
        assert((t.meetingPoints({"Tidmouth", "Newton Crossroad"}, cost) ==
        std::set<std::string>{}));
        
        iss.clear();
        iss.str(
          "Newton\n"
          "Newton Crossroad\n"
          "Tidmouth\n"
          "Newton\n");
          t.addLine(iss);
          
          assert((t.meetingPoints({"Tidmouth", "Newton Crossroad"}, cost) ==
          std::set<std::string>{"Tidmouth", "Newton Crossroad"}) &&
          cost == 1);
         return 0;
         }