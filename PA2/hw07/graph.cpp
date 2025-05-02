#ifndef __PROGTEST__
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <array>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <compare>
#include <algorithm>
#include <memory>
#include <iterator>
#include <functional>
#include <stdexcept>
#include <optional>
using namespace std::literals;

class CDumbString
{
  public:
    CDumbString             ( std::string v )
      : m_Data ( std::move ( v ) )
    {
    }
    bool        operator == ( const CDumbString & rhs ) const = default;
  private:
    std::string m_Data;
};
#endif /* __PROGTEST__ */

template <typename T_>
class CNet
{
  public:
    // default constructor
    // add ( x, y, cost )
    // optimize ()
    // totalCost ( x, y )
  private:
    // todo
};

#ifndef __PROGTEST__
int main ()
{

  CNet<std::string> a;
  a . add ( "Adam", "Bob", 100 )
    . add ( "Bob", "Carol", 200 )
    . add ( "Dave", "Adam", 300 )
    . add ( "Eve", "Fiona", 120 )
    . add ( "Kate", "Larry", 270 )
    . add ( "Ivan", "John", 70 )
    . add ( "Kate", "Ivan", 300 )
    . add ( "George", "Henry", 10 )
    . add ( "Eve", "George", 42 )
    . add ( "Adam", "Eve", 75 )
    . add ( "Ivan", "George", 38 )
    . optimize ();
  assert ( a . totalCost ( "Adam", "Bob" ) == 100 );
  assert ( a . totalCost ( "John", "Eve" ) == 150 );
  assert ( a . totalCost ( "Dave", "Henry" ) == 427 );
  assert ( a . totalCost ( "Carol", "Larry" ) == 1025 );
  assert ( a . totalCost ( "George", "George" ) == 0 );
  assert ( a . totalCost ( "Alice", "Bob" ) == -1 );
  assert ( a . totalCost ( "Thomas", "Thomas" ) == -1 );

  CNet<int> b;
  b . add ( 0, 1, 100 )
    . add ( 1, 2, 200 )
    . add ( 3, 0, 300 )
    . add ( 4, 5, 120 )
    . add ( 10, 11, 270 )
    . add ( 8, 9, 70 )
    . add ( 10, 8, 300 )
    . add ( 6, 7, 10 )
    . add ( 4, 6, 42 )
    . add ( 0, 4, 75 )
    . add ( 8, 6, 38 )
    . optimize ();
  assert ( b . totalCost ( 0, 1 ) == 100 );
  assert ( b . totalCost ( 9, 4 ) == 150 );
  assert ( b . totalCost ( 3, 7 ) == 427 );
  assert ( b . totalCost ( 2, 11 ) == 1025 );
  assert ( b . totalCost ( 6, 6 ) == 0 );
  assert ( b . totalCost ( 0, 1 ) == 100 );
  assert ( b . totalCost ( 19, 19 ) == -1 );

  CNet<CDumbString> c;
  c . add ( "Adam"s, "Bob"s, 100 )
    . add ( "Bob"s, "Carol"s, 200 )
    . add ( "Dave"s, "Adam"s, 300 )
    . add ( "Eve"s, "Fiona"s, 120 )
    . add ( "Kate"s, "Larry"s, 270 )
    . add ( "Ivan"s, "John"s, 70 )
    . add ( "Kate"s, "Ivan"s, 300 )
    . add ( "George"s, "Henry"s, 10 )
    . add ( "Eve"s, "George"s, 42 )
    . add ( "Adam"s, "Eve"s, 75 )
    . add ( "Ivan"s, "George"s, 38 )
    . optimize ();
  assert ( c . totalCost ( "Adam"s, "Bob"s ) == 100 );
  assert ( c . totalCost ( "John"s, "Eve"s ) == 150 );
  assert ( c . totalCost ( "Dave"s, "Henry"s ) == 427 );
  assert ( c . totalCost ( "Carol"s, "Larry"s ) == 1025 );
  assert ( c . totalCost ( "George"s, "George"s ) == 0 );
  assert ( c . totalCost ( "Alice"s, "Bob"s ) == -1 );
  assert ( c . totalCost ( "Thomas"s, "Thomas"s ) == -1 );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
