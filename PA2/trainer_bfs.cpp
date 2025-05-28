#ifndef __TRAINER__
#include <iostream>
#include <cassert>
#include <set>
#include <map>
#include <vector>
#include <queue>
#include <stack>
#endif

class Graph {
    public:
        void add_connection(int a, int b, int cost = 1) {

            //std::cout << "nacitavam " << std::endl;
            if (a != b) data.emplace(a, std::make_pair(b, cost));
        }

        void djikstra(int from, int to) {
            //std::cout <<"pocitam" <<std::endl;
            std::map<int,int> visited; //destination, cost
            std::priority_queue<std::pair<int, int>, std::vector<std::pair<int,int>>, std::greater<>> pq; //cost, next_node
            pq.push(std::make_pair(0, from));
            while(pq.size() != 0) {
                int current_place = pq.top().second;
                int current_price = pq.top().first;
                pq.pop();
                if ((visited.count(current_place) == 0) || (visited[current_place] > current_price)) {
                    visited.emplace(current_place, current_price);
                }
                auto iterator = this->data.equal_range(current_place);
                for (auto i = iterator.first; i != iterator.second; i++) {
                    if ((visited.count(i->second.first) == 0) || (visited[i->second.first] >i->second.second+current_price)) pq.push(std::make_pair(i->second.second+current_price, i->second.first));
                }    
            }
            if(visited.count(to) == 0) std::cout << "0" <<std::endl;
            else std::cout<< "1" <<std::endl;
            /*
            if(visited.count(to) == 0) std::cout << "X" <<std::endl;
            else std::cout<< visited[to] <<std::endl;
            */
        }

        void print() {
            std::cout << "idem printit" << std::endl;
            for (auto i: data) {
                std::cout << i.first << " => " << i.second.first << " : " << i.second.second << std::endl;
            }
        }
    private:
        std::multimap<int, std::pair<int, int>> data;
};

std::vector<int> splitToInts(const std::string& s) {
    std::vector<int> nums;
    size_t start = 0, end;

    while ((end = s.find(' ', start)) != std::string::npos) {
        if (end > start) { // vynechaj prázdne medzery
            nums.push_back(std::stoi(s.substr(start, end - start)));
        }
        start = end + 1;
    }
    if (start < s.size()) {
        nums.push_back(std::stoi(s.substr(start)));
    }
    return nums;
}

int main() {
    //std::cout << "zacinam" << std::endl;
    Graph g;
    int v,e;
    std::cin >> v >> e;
    std::string line;
    int a,b;
    for (int i = 0; i < e; i++) {
        std::cin >> a >> b;
        g.add_connection(a, b);
    }
    int from, to;
    while(std::cin >> from >> to ) {
        g.djikstra(from, to);
    }


    
    //g.print();
    //std::cout << "koncim" << std::endl;
    return 0;
}
