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
#include <unordered_set>
#include <unordered_map>
#include <vector>
 
class CTransport {
public:
  // konstruktor
  // destruktor
 
  CTransport& addLine(std::istream& is);
 
  std::set<std::string> meetingPoints(
    const std::vector<std::string>& fromList, int& sumCost
  ) const;
 
private:
  // todo
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
    "Newton\n"
  );
  t.addLine(iss);
  iss.clear();
  iss.str(
    "Wood Side\n"
    "Green Hill\n"
    "Little Burnside\n"
    "Great Newton\n"
    "Wood Side\n"
  );
  t.addLine(iss);
  iss.clear();
  iss.str(
    "Little Newton\n"
    "Little Burnside\n"
    "Castle Hill\n"
    "Newton Crossroad\n"
    "Lakeside Central\n"
    "Little Newton\n"
  );
  t.addLine(iss);
  iss.clear();
  iss.str(
    "Lakeside Central\n"
    "Little Waterton\n"
  );
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
    "Little Waterton\n"
  );
  t.addLine(iss);
  iss.clear();
  iss.str(
    "Tidmouth\n"
    "Gordon's Hill\n"
    "Suderry\n"
    "Knapford\n"
    "Great Waterton\n"
    "Brendam Docks\n"
    "Tidmouth\n"
  );
  t.addLine(iss);
  iss.clear();
  iss.str(
    "Sodor Ironworks\n"
    "Sodor Steamwork\n"
    "Knapford\n"
    "Maron\n"
    "Gordon's Hill\n"
    "Sodor Ironworks\n"
  );
  t.addLine(iss);
 
  int cost;
  assert(
    (t.meetingPoints({"Newton"}, cost) ==
     std::set<std::string> {"Newton"}) &&
    (cost == 0)
  );
 
  assert(
    (t.meetingPoints({"Newton", "Newton"}, cost) ==
     std::set<std::string> {"Newton"}) &&
    (cost == 0)
  );
 
  assert(
    (t.meetingPoints({"Brno"}, cost) == std::set<std::string> {"Brno"}
    ) &&
    (cost == 0)
  );
 
  assert(
    (t.meetingPoints({"Brno", "Brno"}, cost) ==
     std::set<std::string> {"Brno"}) &&
    (cost == 0)
  );
 
  assert(
    (t.meetingPoints({"Newton", "Brno"}, cost) ==
     std::set<std::string> {})
  );
 
  assert(
    (t.meetingPoints({"Newton", "Black Hill"}, cost) ==
     std::set<std::string> {"Newton", "Black Hill"}) &&
    cost == 1
  );
 
  assert(
    (t.meetingPoints({"Newton", "Newton", "Black Hill"}, cost) ==
     std::set<std::string> {"Newton"}) &&
    cost == 1
  );
 
  assert(
    (t.meetingPoints({"Newton", "Black Hill", "Wood Side"}, cost) ==
     std::set<std::string> {"Black Hill"}) &&
    cost == 2
  );
 
  assert(
    (t.meetingPoints({"Newton Crossroad", "Little Waterton"}, cost) ==
     std::set<std::string> {
       "Newton Crossroad", "Little Waterton", "Lakeside Central"
     }) &&
    cost == 2
  );
 
  assert(
    (t.meetingPoints({"Suddery", "Little Waterton"}, cost) ==
     std::set<std::string> {})
  );
 
  assert(
    (t.meetingPoints({"Tidmouth", "Newton Crossroad"}, cost) ==
     std::set<std::string> {})
  );
 
  iss.clear();
  iss.str(
    "Newton\n"
    "Newton Crossroad\n"
    "Tidmouth\n"
    "Newton\n"
  );
  t.addLine(iss);
 
  assert(
    (t.meetingPoints({"Tidmouth", "Newton Crossroad"}, cost) ==
     std::set<std::string> {"Tidmouth", "Newton Crossroad"}) &&
    cost == 1
  );
}