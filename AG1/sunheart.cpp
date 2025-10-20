#ifndef __PROGTEST__
#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <compare>
#include <cstdint>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <random>
#include <ranges>
#include <set>
#include <stack>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

struct Item {
  enum Type : uint8_t {
    Weapon = 0,
    Armor = 1,
    RubberDuck = 2,
    TYPE_COUNT = 3,
  };

  std::string name;
  Type type;
  int hp = 0, off = 0, def = 0;
  int stacking_off = 0, stacking_def = 0;
  bool first_attack = false;  // Hero attacks first.
  bool stealth = false;  // Hero can sneak past monsters (but cannot loot items
                         // while sneaking).

  friend auto operator<=>(const Item&, const Item&) = default;
};

struct Monster {
  int hp = 0, off = 0, def = 0;
  int stacking_off = 0, stacking_def = 0;
};

using RoomId = size_t;
using ItemId = size_t;

struct Room {
  std::vector<RoomId> neighbors;
  std::optional<Monster> monster;
  std::vector<Item> items;
};

struct Move {
  RoomId room;
};
struct Pickup {
  ItemId item;
};
struct Drop {
  Item::Type type;
};
using Action = std::variant<Move, Pickup, Drop>;

namespace student_namespace {
#endif

std::optional<int> turns_to_kill(int hp, int dmg, int stacking_dmg) {
  assert(hp > 0);

  if (stacking_dmg == 0) {
    if (dmg <= 0) return {};
    return (hp + dmg - 1) / dmg;
  }

  int i = 0;
  for (; hp > 0; i++) {
    if (dmg <= 0 && stacking_dmg < 0) return {};
    hp -= std::max(dmg, 0);
    dmg += stacking_dmg;
  }

  return i;
}

enum CombatResult { A_WINS, B_WINS, TIE };

// Monster `a` attacks first
CombatResult simulate_combat(Monster a, Monster b) {
  a.def += a.stacking_def;

  auto a_turns =
      turns_to_kill(b.hp, a.off - b.def, a.stacking_off - b.stacking_def);
  auto b_turns =
      turns_to_kill(a.hp, b.off - a.def, b.stacking_off - a.stacking_def);

  if (!a_turns && !b_turns) return TIE;
  if (!a_turns) return B_WINS;
  if (!b_turns) return A_WINS;
  return *a_turns <= *b_turns ? A_WINS : B_WINS;
}

struct my_item {
  my_item() = default;
  my_item(const Item& source) {
    this->first_attack = source.first_attack;
    this->hp = source.hp;
    this->off = source.off;
    this->def = source.def;
    this->stacking_def = source.stacking_def;
    this->stacking_off = source.stacking_off;
    this->stealth = source.stealth;
    this->type = source.type;
  }
  Item::Type type = Item::TYPE_COUNT;
  int index = 0;
  int hp = 0, off = 0, def = 0;
  int stacking_off = 0, stacking_def = 0;
  bool first_attack = false;  // Hero attacks first.
  bool stealth = false;  // Hero can sneak past monsters (but cannot loot items
                         // while sneaking).
  void print() const {
    std::cout << "hp: " << hp << " off: " << off << " def: " << def
              << " soff: " << stacking_off << " sdef: " << stacking_def;
    std::cout << " 1st_at " << first_attack << " stealth " << stealth
              << "type: ";
    switch (type) {
      case Item::Weapon:
        std::cout << "Weapon";
        break;
      case Item::Armor:
        std::cout << "Armor";
        break;
      case Item::RubberDuck:
        std::cout << "Duck";
        break;
      case Item::TYPE_COUNT:
        std::cout << "TYPE_COUNT";
        break;
    }
    std::cout << std::endl;
  }
  friend auto operator<=>(const my_item&, const my_item&) = default;
};

struct Player {
  int hp = 10000, off = 3, def = 2;
  int stacking_off = 0, stacking_def = 0;
  std::array<my_item, 3> inventory;
  Monster make_monster() const {
    Monster result;
    result.hp = hp;
    result.off = off;
    result.def = def;
    result.stacking_off = stacking_off;
    result.stacking_def = stacking_def;

    for (auto i : inventory) {
      result.hp += i.hp;
      result.off += i.off;
      result.def += i.def;
      result.stacking_off += i.stacking_off;
      result.stacking_def += i.stacking_def;
    }
    if (result.hp <= 0) result.hp = 1;
    return result;
  }
  bool has_stealth() const {
    for (const auto& i : inventory) {
      if (i.stealth == true) return true;
    }
    return false;
  }
};

bool check_if_monster_defeatable(const Player& human,
                                 const Monster& monsteros) {
  bool player_first = false;
  for (const auto& i : human.inventory) {
    if (i.first_attack == true) player_first = true;
  }
  if (player_first) {
    auto result = simulate_combat(human.make_monster(), monsteros);
    if (result == A_WINS) {
      return true;
    } else {
      return false;
    }
  } else {
    auto result = simulate_combat(monsteros, human.make_monster());
    if (result == B_WINS) {
      return true;
    } else {
      return false;
    }
  }
}

struct Node_State {
  Node_State() : node(0), has_treasure(false), inventory() {}
  RoomId node;
  bool has_treasure = false;
  int distance = 0;
  std::array<my_item, 3> inventory = {};
  struct Hash {
    size_t operator()(const Node_State& s) const {
      size_t h = std::hash<RoomId>{}(s.node);
      h = h * 31 + std::hash<bool>{}(s.has_treasure);
      for (const auto& item : s.inventory) {
        h = h * 31 + std::hash<Item::Type>{}(item.type);
        if (item.type != Item::TYPE_COUNT) {
          h = h * 31 + std::hash<int>{}(item.index);
        }
      }
      return h;
    }
  };
  void print() const {
    std::cout << "RoomId: " << this->node
              << " has treasure: " << (has_treasure ? "1" : "0") << "distance: " << distance << std::endl;
    for (int i = 0; i < 3; i++) {
      this->inventory[i].print();
    }
  }
  auto operator<=>(const Node_State& other) const noexcept = default;
  bool operator==(const Node_State& other) const {
    return node == other.node && has_treasure == other.has_treasure &&
           inventory == other.inventory;
  }
};

struct CompareNodeState {
    bool operator()(const Node_State& a, const Node_State& b) const {
        return a.distance > b.distance;
    }
};


void printAction(const Action& a) {
  std::visit(
      [](const auto& action) {
        using T = std::decay_t<decltype(action)>;

        if constexpr (std::is_same_v<T, Move>)
          std::cout << "Move to room " << action.room << '\n';
        else if constexpr (std::is_same_v<T, Pickup>)
          std::cout << "Pickup item " << action.item << '\n';
        else if constexpr (std::is_same_v<T, Drop>)
          std::cout << "Drop item of type " << action.type << '\n';
      },
      a);
}

void previous_action_printer(
    std::unordered_map<Node_State, std::pair<Node_State, Action>,
                       Node_State::Hash>
        previous_action) {
  std::cout << "\n\nzaciatok----------------------------------------" << std::endl;
  for (const auto& pair : previous_action) {
    std::cout << "-------------------------------from:" << std::endl;
    pair.second.first.print();
    std::cout << "action: ";
    printAction(pair.second.second);
    std::cout << "to:";
    pair.first.print();
  }
  std::cout << "koniec vypisu---------------------------------------" << std::endl;
}

std::vector<Action> create_action_vector(
    const std::unordered_map<Node_State, std::pair<Node_State, Action>,
                             Node_State::Hash>& map,
    const Node_State& last, const Node_State& first) {
  std::vector<Action> result;
  Node_State current = last;

  // std::cout << "zacinam vyrabat result" << std::endl;
  //previous_action_printer(map);
  while (current != first) {
    //current.print();
    const auto& action = map.at(current).second;
    if (!(std::holds_alternative<Drop>(action) && std::get<Drop>(action).type == Item::TYPE_COUNT)) {
      result.push_back(action);
      //printAction(action);
    } else {
    }
    current = map.at(current).first;
  }
  std::reverse(result.begin(), result.end());
  return result;
}

std::vector<std::vector<my_item>> get_weapons_available(
    const std::array<my_item, 3>& inventory, const std::vector<Item>& items) {
  std::vector<std::vector<my_item>> weapons_available(3);
  weapons_available[0].push_back(my_item());
  weapons_available[1].push_back(my_item());
  weapons_available[2].push_back(my_item());
  // 0 je weapon 1 je armor 2 je duck
  if (inventory[0] != my_item()) {
    weapons_available[0].push_back(inventory[0]);
  }
  if (inventory[1] != my_item()) {
    weapons_available[1].push_back(inventory[1]);
  }
  if (inventory[2] != my_item()) {
    weapons_available[2].push_back(inventory[2]);
  }

  for (size_t i = 0; i < items.size(); i++) {
    my_item current_item(items[i]);
    current_item.index = i;
    if (current_item.type == 0)
      weapons_available[0].push_back(current_item);
    else if (current_item.type == 1)
      weapons_available[1].push_back(current_item);
    else if (current_item.type == 2)
      weapons_available[2].push_back(current_item);
  }
  return weapons_available;
}

std::vector<Action> find_shortest_path(const std::vector<Room>& rooms,
                                       const std::vector<RoomId>& entrances,
                                       RoomId treasure) {
  std::vector<Action> result;
  Player human;
  std::priority_queue<Node_State, std::vector<Node_State>, CompareNodeState> to_visit;
  std::unordered_map<Node_State, std::pair<Node_State, Action>, Node_State::Hash> previous_action;
  bool found_path = false;
  Node_State first_room;
  first_room.node = rooms.size();
  // std::cout << "zacinam bfs" << std::endl;
  for (auto i : entrances) {
    Node_State current_inserting;
    current_inserting.node = i;
    previous_action[current_inserting] = std::make_pair(first_room, Move(i));
    to_visit.push(current_inserting);
  }
  Node_State current_node_state;
  /// std::cout << "zacinam while loop" << std::endl;
  while (to_visit.size() > 0) {
    // nacitam terajsi node
    current_node_state = to_visit.top();
    to_visit.pop();
    //current_node_state.print();
    // skontrolujem ci som tu uz nebol
    human.inventory = current_node_state.inventory;
    // testujem ci idem fightit
    if (!rooms[current_node_state.node].monster.has_value() || check_if_monster_defeatable( human, rooms[current_node_state.node].monster.value())) {  // ci ho mozem fightit
      // std::cout << "zabil som priseru" << std::endl;
      if (current_node_state.node == treasure) {
        // std::cout << "nasiel som poklad" << std::endl;
        Node_State previous = current_node_state;
        current_node_state.has_treasure = true;
        if (previous_action.find(current_node_state) == previous_action.end()) {
          previous_action[current_node_state] = std::make_pair(previous, Drop(Item::TYPE_COUNT));

        }
      }
      if (current_node_state.has_treasure == true &&
          std::find(entrances.begin(), entrances.end(),
                    current_node_state.node) != entrances.end()) {
        // std::cout << "nasiel som cestu" << std::endl;
        found_path = true;
        break;
      }
      // std::cout << "vyrabam vector itemov" << std::endl;
      std::vector<std::vector<my_item>> weapons_available = get_weapons_available(human.inventory, rooms[current_node_state.node].items);
      Node_State new_state;
      for (const auto & neighbour_id : rooms[current_node_state.node].neighbors) {
        new_state = current_node_state;
        new_state.node = neighbour_id;
        new_state.distance += 1;
        if (!previous_action.contains(new_state)) {
          previous_action[new_state] = std::make_pair(current_node_state, Move(neighbour_id));
          to_visit.push(new_state);
        }
        for (size_t weapon_index = 0; weapon_index < weapons_available[0].size(); weapon_index++) {  // weapon
          for (size_t armor_index = 0; armor_index < weapons_available[1].size(); armor_index++) {  // armor
            for (size_t duck_index = 0; duck_index < weapons_available[2].size(); duck_index++) {  // duck
              new_state = current_node_state;


              if ((weapon_index == 0) && (current_node_state.inventory[0] != my_item())) { // idem dropovat weapon
                new_state.inventory[0] = my_item();
                if (!previous_action.contains(new_state)) {
                  previous_action[new_state] = std::make_pair(current_node_state, Drop(Item::Weapon));

                }

              } else if ((weapon_index != 0) && (current_node_state.inventory[0] != weapons_available[0][weapon_index])) {// idem pickovat weapon
                new_state.inventory[0] = weapons_available[0][weapon_index];
                if (!previous_action.contains(new_state)) {
                  previous_action[new_state] = std::make_pair(current_node_state, Pickup(new_state.inventory[0].index));

                } 
              }

              if ((armor_index == 0) && (current_node_state.inventory[1] != my_item())) { // idem dropovat armor
                new_state.inventory[1] = my_item();
                if (!previous_action.contains(new_state)) {
                  previous_action[new_state] = std::make_pair(current_node_state, Drop(Item::Armor));
                  
                }
              } else if ((armor_index != 0) && (current_node_state.inventory[1] != weapons_available[1][armor_index])) { // idem pickovat armor
                new_state.inventory[1] = weapons_available[1][armor_index];

                if (!previous_action.contains(new_state)) {
                  previous_action[new_state] = std::make_pair(current_node_state, Pickup(new_state.inventory[1].index));
                  

                } 
              }

              if ((duck_index == 0) && (current_node_state.inventory[2] != my_item())) { // idem dropovat duck
                new_state.inventory[2] = my_item();
                if (!previous_action.contains(new_state)) {
                  previous_action[new_state] = std::make_pair(current_node_state, Drop(Item::RubberDuck));
                  


                }
              } else if ((duck_index != 0) && (current_node_state.inventory[2] != weapons_available[2][duck_index])) { // idem pickovat duck
                new_state.inventory[2] = weapons_available[2][duck_index];

                if (!previous_action.contains(new_state)) {
                  previous_action[new_state] = std::make_pair(current_node_state, Pickup(new_state.inventory[2].index));
                  

                } 
              }
              new_state.node = neighbour_id;
              new_state.distance += 1;
              if (!previous_action.contains(new_state)) {
                previous_action[new_state] = std::make_pair(current_node_state, Move(neighbour_id));
                to_visit.push(new_state);
              }
            }
          }
        }
      }
    } else if (human.has_stealth()) {  // mozem stealthnut okolo
      if (current_node_state.has_treasure == true &&
          std::find(entrances.begin(), entrances.end(),
                    current_node_state.node) != entrances.end()) {
        // std::cout << "nasiel som cestu" << std::endl;
        found_path = true;
        break;
      }
      // tu vlozim vsetkych neighbours s terajsim inventarom alebo dropnutym
      // inventarom
      // std::cout << "stealthujem okolo" << std::endl;
      for (const auto& i : rooms[current_node_state.node].neighbors) {
        // nedropnem nic
        Node_State working_state = current_node_state;
        working_state.node = i;
        if (!previous_action.contains(working_state)) {
          to_visit.push(working_state);
          previous_action[working_state] = std::make_pair(current_node_state, Move(i));
        }
        for (int w = 0; w < 2; w++) {
          for (int a = 0; a < 2; a++) {
            for (int d = 0; d < 2; d++) {
              working_state = current_node_state;
              if ((w == 0) && (working_state.inventory[0] != my_item())) {
                working_state.inventory[0] = my_item();
                if (!previous_action.contains(working_state)) {
                  previous_action[working_state] = std::make_pair(current_node_state, Drop(Item::Weapon));
                  to_visit.push(working_state);

                }
              }
              if ((a == 0) && (working_state.inventory[1] != my_item())) {
                working_state.inventory[1] = my_item();
                if (!previous_action.contains(working_state)) {
                  previous_action[working_state] = std::make_pair(current_node_state, Drop(Item::Armor));
                  to_visit.push(working_state);

                }
              }
              if ((d == 0) && (working_state.inventory[2] != my_item())) {
                working_state.inventory[2] = my_item();
                if (!previous_action.contains(working_state)) {
                  previous_action[working_state] = std::make_pair(current_node_state, Drop(Item::RubberDuck));
                  to_visit.push(working_state);

                }
              }
            }
          }
        }
      }
    } else {
      continue;  // nemam tu co robit lebo by som prehral
    }
  }

  // std::cout << "bfs dobehlo" << std::endl;
  // previous_action_printer(previous_action);
  if (found_path) {
    // std::cout << "mam cestu" << std::endl;
    result =
        create_action_vector(previous_action, current_node_state, first_room);
  }
  // std::cout << "koncim find shortest path" << std::endl;
  return result;
}

#ifndef __PROGTEST__
}

bool contains(const auto& vec, const auto& x) {
  return std::ranges::find(vec, x) != vec.end();
};

#define CHECK(cond, ...)            \
  do {                              \
    if (!(cond)) {                  \
      fprintf(stderr, __VA_ARGS__); \
      assert(0);                    \
    }                               \
  } while (0)
void check_solution(const std::vector<Room>& rooms,
                    const std::vector<RoomId>& entrances, RoomId treasure,
                    size_t expected_rooms, bool print = true) {
  // TODO check if hero survives combat
  // TODO check if treasure was collected

  using student_namespace::find_shortest_path;
  const std::vector<Action> solution =
      find_shortest_path(rooms, entrances, treasure);

  if (expected_rooms == 0) {
    CHECK(solution.size() == 0, "No solution should exist but got some.\n");
    return;
  }

  CHECK(solution.size() != 0, "Expected solution but got none.\n");

  try {
    CHECK(contains(entrances, std::get<Move>(solution.front()).room),
          "Path must start at entrance.\n");
    CHECK(contains(entrances, std::get<Move>(solution.back()).room),
          "Path must end at entrance.\n");
  } catch (const std::bad_variant_access&) {
    CHECK(false, "Path must start and end with Move.\n");
  }

  std::vector<Item> equip;
  RoomId cur = std::get<Move>(solution.front()).room;
  CHECK(cur < rooms.size(), "Room index out of range.\n");
  size_t room_count = 1;
  if (print) printf("Move(%zu)", cur);

  auto drop_items = [&](Item::Type type) {
    std::erase_if(equip, [&](const Item& i) { return i.type == type; });
  };

  for (size_t i = 1; i < solution.size(); i++) {
    if (auto m = std::get_if<Move>(&solution[i])) {
      CHECK(m->room < rooms.size(), "Next room index out of range.\n");
      CHECK(contains(rooms[cur].neighbors, m->room),
            "Next room is not a neighbor of the current one.\n");
      cur = m->room;
      room_count++;

      if (print) printf(", Move(%zu)", cur);
    } else if (auto p = std::get_if<Pickup>(&solution[i])) {
      CHECK(p->item < rooms[cur].items.size(),
            "Picked up item out of range.\n");
      const Item& item = rooms[cur].items[p->item];
      drop_items(item.type);
      equip.push_back(item);

      if (print) printf(", Pickup(%zu, %s)", p->item, item.name.c_str());
    } else {
      auto t = std::get<Drop>(solution[i]).type;
      drop_items(t);

      if (print)
        printf(", Drop(%s)", t == Item::Armor        ? "Armor"
                             : t == Item::Weapon     ? "Weapon"
                             : t == Item::RubberDuck ? "Duck"
                                                     : "ERROR");
    }
  }

  if (print) printf("\n");

  CHECK(room_count == expected_rooms, "Expected %zu rooms but got %zu.\n",
        expected_rooms, room_count);
}
#undef CHECK

void combat_examples() {
  const Item defensive_duck = {
    .name = "Defensive Duck", .type = Item::RubberDuck,
    .off = -2, .def = 8,
  };

  const Item invincible_duck = {
    .name = "Invincible Duck", .type = Item::RubberDuck,
    .hp = -20'000, .def = 1'000,
  };

  const Item fast_duck = {
    .name = "Fast Duck", .type = Item::RubberDuck,
    .first_attack = true,
  };

  const Item offensive_duck = {
    .name = "Offensive Duck", .type = Item::RubberDuck,
    .stacking_off = 100,
  };

  std::vector<Room> rooms(2);
  rooms[0].neighbors.push_back(1);
  rooms[1].neighbors.push_back(0);

  check_solution(rooms, { 0 }, 1, 3);

  rooms[1].monster = Monster{ .hp = 9'999, .off = 3, .def = 2 };
  check_solution(rooms, { 0 }, 1, 3);
  
  rooms[1].monster->hp += 1;
  check_solution(rooms, { 0 }, 1, 0);

  rooms[1].monster = Monster{ .hp = 100'000, .off = 10 };
  check_solution(rooms, { 0 }, 1, 0);

  rooms[0].items = { defensive_duck };
  check_solution(rooms, { 0 }, 1, 3);

  rooms[0].items = { invincible_duck };
  check_solution(rooms, { 0 }, 1, 3);

  rooms[0].items = {};
  rooms[1].monster = Monster{ .hp=1, .off=3, .def=0, .stacking_def=100 };
  check_solution(rooms, { 0 }, 1, 0);

  rooms[0].items.push_back(offensive_duck);
  check_solution(rooms, { 0 }, 1, 0);

  rooms[0].items.push_back(fast_duck);
  check_solution(rooms, { 0 }, 1, 3);
}

void stealth_examples() {
  const Item stealth_duck = {
    .name = "Stealth Duck", .type = Item::RubberDuck,
    .stealth = true,
  };

  const Item sword = {
    .name = "Sword", .type = Item::Weapon,
    .off = 10,
  };

  const Monster m = { .hp = 10'000, .off=10, .def=2 };

  std::vector<Room> rooms(4);

  for (size_t i = 1; i < rooms.size(); i++) {
    rooms[i].neighbors.push_back(i - 1);
    rooms[i - 1].neighbors.push_back(i);
  }

  rooms[0].items = { stealth_duck };
  rooms[2].monster = m;

  check_solution(rooms, { 0 }, 2, 0); // Cannot stealth steal treasure

  rooms[3].items = { sword };
  // Stealth to 3, grab sword & kill monster
  check_solution(rooms, { 0 }, 2, 7);

  rooms[3].items = {};
  rooms[1].items = { sword };
  check_solution(rooms, { 0 }, 2, 5);

  rooms[1].monster = m;
  check_solution(rooms, { 0 }, 2, 0); // Cannot pickup while stealthing
}

void example_tests() {
  const Item sword = {
    .name = "Sword", .type = Item::Weapon,
    .off = 10, .def = -1,
  };

  const Item berserker_sword = {
    .name = "Berserker's Sword", .type = Item::Weapon,
    .hp = -1'000, .off = 10'000, .def = 0,
    .stacking_off = 1'000, .stacking_def = -500,
    .first_attack = true
  };

  const Item heavy_armor = {
    .name = "Heavy Armor", .type = Item::Armor,
    .hp = 5'000, .off = -10, .def = 300,
  };

  const Item debugging_duck = {
    .name = "Debugging Duck", .type = Item::RubberDuck,
    .stacking_off = 1,
    .stealth = true
  };

  std::vector<Room> rooms(14);
  enum : RoomId {
    no_monster = 10,
    weak,
    strong,
    durable
  };

  rooms[no_monster] = { {}, {}, { heavy_armor } };
  rooms[weak] = { {}, Monster{ .hp = 1000, .off = 10 }, { debugging_duck, sword } };
  rooms[strong] = { {}, Monster{ .hp = 10, .off = 10'000, .def = 1'000'000 },
    { berserker_sword } };
  rooms[durable] =  { {}, Monster{ .hp = 100'000, .off = 10, .stacking_def = 1 },
    { berserker_sword } };

  auto link = [&](RoomId a, RoomId b) {
    rooms[a].neighbors.push_back(b);
    rooms[b].neighbors.push_back(a);
  };

  link(0, no_monster);
  link(0, weak);
  link(weak, 7);
  link(0, strong);
  link(strong, 8);
  link(0, 1);
  link(1, 2);
  link(2, durable);
  link(durable, 6);

  check_solution(rooms, { 0 }, 0, 1); // Treasure at entrance
  check_solution(rooms, { 9 }, 0, 0); // No path to treasure
  check_solution(rooms, { 8 }, 0, 0); // Blocked by monster
  check_solution(rooms, { durable }, durable, 0); // Killed on spot
  check_solution(rooms, { 7 }, 0, 5); // Kills weak monster
  check_solution(rooms, { 6, 7 }, 2, 7); // Sneaks around durable
  check_solution(rooms, { 6, 7 }, durable, 9); // Kills durable
}

void example_tests2() {
  const Item duck_of_power = {
    .name = "Duck of Power", .type = Item::RubberDuck,
    .hp = 10'000'000, .off = 10'000'000, .def = 10'000'000,
  };

  const Item dull_sword = {
    .name = "Dull Sword", .type = Item::Weapon,
    .off = -10, .def = -5,
  };

  const Item sword = {
    .name = "Sword", .type = Item::Weapon,
    .off = 5, .def = -1,
  };

  const Item leather_pants = {
    .name = "Leather pants", .type = Item::Armor,
    .off = -3, .def = 1,
    .first_attack = true
  };

  const Item defensive_duck = {
    .name = "Defensive Duck", .type = Item::RubberDuck,
    .off = -2, .def = 8,
  };

  const Item stealth_duck = {
    .name = "Stealth Duck", .type = Item::RubberDuck,
    .off = -100, .def = -100,
    .stealth = true,
  };

  const Item slow_sword = {
    .name = "Slow Sword", .type = Item::Weapon,
    .off = -10'000,
    .stacking_off = 1,
  };

  constexpr int CYCLE_LEN = 100;
  enum : RoomId {
    impossible = CYCLE_LEN,
    r1, r2, r3, r4, r4a, r4b, ROOM_COUNT
  }; 
  std::vector<Room> rooms(ROOM_COUNT);

  auto link = [&](RoomId a, RoomId b) {
    rooms[a].neighbors.push_back(b);
    rooms[b].neighbors.push_back(a);
  };

  for (int i = 1; i < CYCLE_LEN; i++) link(i - 1, i);
  rooms[CYCLE_LEN-1].neighbors.push_back(0);

  rooms[impossible] = { {}, {{ .hp = 1'000'000, .off = 1'000'000 }}, { duck_of_power } };
  link(impossible, 0);

  rooms[r1] = { {}, {{ .hp = 9'999, .off = 3, .def = 2 }}, { defensive_duck, dull_sword } };
  link(r1, 1);

  rooms[r2] = { {}, {{ .hp = 100'000, .off = 10 }}, { sword, leather_pants } };
  link(r2, CYCLE_LEN - 3);

  rooms[r3] = { {}, {{ .hp = 100'000, .off = 10, .def = 1 }}, { stealth_duck, slow_sword } };
  link(r3, 2);

  rooms[r4] = { { r4a }, {{ .hp = 10'000, .off = 10'000 }}, {} };
  rooms[r4a] = { { r4b } };
  rooms[r4b] = { {}, {{ .hp = 10'000, .off = 1 }}, {} };
  link(r4, CYCLE_LEN - 4);
  link(r4b, CYCLE_LEN - 4);

  // r1 (loots duck) -> r2 (loots pants & sword) -> r3
  check_solution(rooms, { 0 }, r3, CYCLE_LEN + 11);
  // r1 (loots duck) -> r2 (loots pants & sword) -> r3 (loots stealth duck) -> r4a
  check_solution(rooms, { 0 }, r4a, 2*CYCLE_LEN + 11);
}

void example_tests3() {
  const Item sword = {
    .name = "Sword", .type = Item::Weapon,
    .off = 10,
  };

  const Item stacking_duck = {
    .name = "Stacking Duck", .type = Item::RubberDuck,
    .hp = -9'999, .stacking_off = 100,
  };

  const Item heavy_armor = {
    .name = "Heavy Armor", .type = Item::Armor,
    .off = -1'000, .def = 1'000,
  };

  enum : RoomId {
    start, treasure, short_path, long_path = short_path + 3,
    COUNT = long_path + 4
  };

  std::vector<Room> rooms(COUNT);

  auto link = [&](RoomId a, RoomId b) {
    rooms[a].neighbors.push_back(b);
    rooms[b].neighbors.push_back(a);
  };

  rooms[treasure].neighbors.push_back(start); // one-way back to start

  link(start, long_path + 0);
  link(long_path + 0, long_path + 1);
  link(long_path + 1, long_path + 2);
  link(long_path + 2, long_path + 3);
  link(long_path + 3, treasure);

  link(start, short_path + 0);
  link(short_path + 0, short_path + 1);
  link(short_path + 1, short_path + 2);
  link(short_path + 2, treasure);

  rooms[short_path + 0].items = { sword };
  rooms[short_path + 1].monster = Monster{ .hp=10'000, .off=5, .def=3 };
  rooms[short_path + 1].items = { stacking_duck, heavy_armor };
  rooms[short_path + 2].monster = Monster{ .hp=100'000, .off=5, .def=3 };

  check_solution(rooms, { start }, treasure, 6);
}

void example_tests4() {
  const Item sword = {
    .name = "Sword", .type = Item::Weapon,
    .off = 3, .def = -1
  };

  enum : RoomId {
    start, treasure, short_path, long_path = short_path + 3,
    COUNT = long_path + 4
  };

  std::vector<Room> rooms(COUNT);

  auto link = [&](RoomId a, RoomId b) {
    rooms[a].neighbors.push_back(b);
    rooms[b].neighbors.push_back(a);
  };

  rooms[treasure].neighbors.push_back(start); // one-way back to start

  link(start, long_path + 0);
  link(long_path + 0, long_path + 1);
  link(long_path + 1, long_path + 2);
  link(long_path + 2, long_path + 3);
  link(long_path + 3, treasure);

  link(start, short_path + 0);
  link(short_path + 0, short_path + 1);
  link(short_path + 1, short_path + 2);
  link(short_path + 2, treasure);

  Monster needs_sword = Monster{ .hp=10'000, .off=6, .def=3 };
  Monster no_sword = Monster{ .hp=100'000, .off=3 };
  rooms[short_path + 0].monster = needs_sword;
  rooms[short_path + 1].monster = no_sword;
  rooms[short_path + 0].monster = needs_sword;

  check_solution(rooms, { start }, treasure, 7);
}

void example_tests5() {
  const Item sword = {
    .name = "Sword", .type = Item::Weapon,
    .off = 5, .def = -1,
  };

  constexpr int LEN = 300;
  std::vector<Room> rooms(LEN);

  auto link = [&](RoomId a, RoomId b) {
    rooms[a].neighbors.push_back(b);
    rooms[b].neighbors.push_back(a);
  };

  for (int i = 1; i < LEN; i++) {
    rooms[i].items = { sword, sword, sword };
    link(i - 1, i);
  }

  rooms[LEN - 1].monster = Monster{ .hp = 1'000'000, .off = 1'000'000 };

  check_solution(rooms, { 0 }, LEN - 1, 0);
}

void example_tests6() {
  const Item defensive_duck = {
    .name = "Defensive Duck", .type = Item::RubberDuck,
    .off = -100, .def = 100,
  };

  constexpr int LEN = 31;
  std::vector<Room> rooms(LEN + LEN + 10);

  auto link = [&](RoomId a, RoomId b) {
    rooms[a].neighbors.push_back(b);
    rooms[b].neighbors.push_back(a);
  };

  rooms[0].items = { defensive_duck };

  assert(LEN % 2 == 1);
  for (int i = 1; i + 1 < LEN; i += 2) {
    link(i - 1, i);
    link(i, i + 1);

    rooms[i+1].items = { defensive_duck };

    rooms[i].monster = Monster{ .hp = 10'000'000, .off = 50, .def = -120 };
    rooms[i+1].monster = Monster{ .hp = 10'000, .off = 1, .def = 1 };
  }

  for (int i = 1; i < LEN + 10; i++)
    link(LEN + i - 1, LEN + i);

  link(0, LEN);
  link(LEN - 1, 2*LEN + 10 - 1);

  check_solution(rooms, { 0 }, LEN - 1, 2*LEN - 1);
}

void example_tests7() {
  const Item stealth_underpants = {
    .name = "Stealth Underpants", .type = Item::Armor,
    .off = 0, .def = 0,
    .stealth = true,
  };

  const Item offensive_underpants = {
    .name = "Offensive Underpants", .type = Item::Armor,
    .hp = -10'000, .off = 100,
  };

  const int LEN = 4;
  std::vector<Room> rooms(4);

  auto link = [&](RoomId a, RoomId b) {
    rooms[a].neighbors.push_back(b);
    rooms[b].neighbors.push_back(a);
  };

  for (int i = 1; i < LEN; i++) link(i - 1, i);

  rooms[2].items = { offensive_underpants };

  rooms[1].monster = Monster{ .hp = 1, .off = 1, .def = 50 };
  rooms[1].items = { stealth_underpants };

  rooms[0].monster = Monster{ .hp=100'000, .off=1000, .def=1000 };
  check_solution(rooms, { 0, LEN - 1 }, 1, LEN);
}

int main() {
  std::cout << "zacinam" << std::endl;
  combat_examples();
  std::cout << "combat examples done" << std::endl;
  stealth_examples();
  std::cout << "stealth examples done" << std::endl;
  example_tests();
  std::cout << "example test 1 done" << std::endl;
  example_tests2();
  std::cout << "example test 2 done" << std::endl;
  example_tests3();
  std::cout << "example test 3 done" << std::endl;
  example_tests4();
  std::cout << "example test 4 done" << std::endl;
  example_tests5();
  std::cout << "example test 5 done" << std::endl;
  example_tests6();
  std::cout << "example test 6 done" << std::endl;
  example_tests7();
  std::cout << "example test 7 done" << std::endl;
  std::cout << "koncim" << std::endl;
}

#endif
