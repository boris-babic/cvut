#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <memory>
#endif /* __PROGTEST__ */
 


class CTeleport
{ 
public:
 
    // ctor
    // dtor
 
    CTeleport & Add ( const std::string & from,
                      const std::string & to,
                      unsigned fromTime,
                      unsigned toTime )
    {
        Teleports x;
        x.time_from = fromTime;
        x.time_to = toTime;
        x.to = to;
        this->data[from].push_back(x);
        return *this;
    }

    CTeleport & Optimize ( void )
    {
        return *this;
    }
 
    unsigned FindWay ( const std::string & from,
                       const std::string & to,
                       unsigned time )
    {
        std::cout << "zacinam bfs " << std::endl << std::endl;
        unsigned result = INT32_MAX;
        std::priority_queue<std::pair<unsigned, std::string>,
                            std::vector<std::pair<unsigned, std::string>>,
                            std::greater<>> pq;
        std::unordered_map<std::string, unsigned> visited;
        pq.push(std::make_pair( time, from));
        while (pq.size() != 0) {
            unsigned current_time = pq.top().first;
            std::string current_place = pq.top().second;
            std::cout << "Prave som:" << current_place << " s casom " << current_time << std::endl;
            pq.pop();

            visited.emplace(current_place, current_time);
            if(visited[current_place] >= current_time) visited[current_place] = current_time;
            else continue;

            if (current_place == to) {
                if (current_time < result) result = current_time;
            }

            for (const auto & conn: this->data[current_place]) {
                if (conn.time_from >= current_time && (visited.count(conn.to) == 0 || visited[conn.to] > conn.time_to)) {
                    std::cout << "Pridavam do queue " << conn.time_to << ": " << conn.to << std::endl;
                    pq.push(std::make_pair(conn.time_to, conn.to));
                }
            }
        }
        if (result < INT32_MAX) return result;
        else {
            throw std::invalid_argument("No path found");
        }
    }
private:
    struct Teleports {
        std::string to;
        unsigned time_from;
        unsigned time_to;
    };
    std::unordered_map<std::string, std::vector<Teleports>> data;
};



#ifndef __PROGTEST__
int main ( void )
{
    std::cout << "zijem" << std::endl;
    CTeleport t;
    t . Add ( "Prague", "Vienna", 0, 7 )
    . Add ( "Vienna", "Berlin", 9, 260 )
    . Add ( "Vienna", "London", 8, 120 )
    . Add ( "Vienna", "Chicago", 4, 3 )
    . Add ( "Prague", "Vienna", 10, 10 ) . Optimize ( );
    
    assert ( t . FindWay ( "Prague", "Vienna", 0 ) == 7 );
    assert ( t . FindWay ( "Prague", "Vienna", 1 ) == 10 );
    assert ( t . FindWay ( "Prague", "London", 0 ) == 120 );
    assert ( t . FindWay ( "Vienna", "Chicago", 4 ) == 3 );
    /*
    using  namespace std;
    try { t . FindWay ( "Prague", "London", 2 ); assert ( "Missing exception" == nullptr ); }
    catch ( const invalid_argument & e ) { }
    catch ( ... ) { assert ( "Invalid exception" == nullptr ); }
    try { t . FindWay ( "Prague", "Chicago", 0 ); assert ( "Missing exception" == nullptr ); }
    catch ( const invalid_argument & e ) { }
    catch ( ... ) { assert ( "Invalid exception" == nullptr ); }
    
    */
    t . Add ( "Dallas", "Atlanta", 150, 30 )
    . Add ( "Berlin", "Helsinki", 1080, 2560 )
    . Add ( "Chicago", "Frankfurt", 50, 0 )
    . Add ( "Helsinki", "Vienna", 3200, 3 )
    . Add ( "Chicago", "London", 10, 12 )
    . Add ( "London", "Atlanta", 20, 40 )
    . Add ( "Vienna", "Atlanta", 10, 50 )
    . Add ( "Prague", "Vienna", 1, 6 )
    . Add ( "Berlin", "Helsinki", 265, 265 ) 
    . Add ( "Berlin", "London", 259, 0 ) . Optimize ( );
    
    assert ( t . FindWay ( "Prague", "Frankfurt", 0 ) == 0 );
    assert ( t . FindWay ( "Prague", "Atlanta", 0 ) == 40 );
    assert ( t . FindWay ( "Prague", "Atlanta", 10 ) == 50 );
    
   std::cout << "skoncim" << std::endl;
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */