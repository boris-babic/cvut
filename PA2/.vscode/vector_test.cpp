#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <compare>
#include <functional>
#include <stdexcept>
#include <algorithm>


int main() {
    std::string x1 = "Adam Bielik";
    std::string x2 = "Boris Babic";
    std::string x3 = "Filip Cesticky";
    std::string x4 = "Filip Papso";
    std::string x5 = "Tomas Lamlech";
    std::string x6 = "Tomas Bajana";

    std::vector<std::string> v = {x1, x2, x3, x4, x6, x5};
    std::string nove1 = "Boris Bajana";
    std::string nove2 = "Tomas Bajana";
    // Insert element 8 at index 2


    auto it1 = std::lower_bound(v.begin(), v.end(), nove1);
    if (*it1 != nove1) v.insert(it1, nove1);
    else std::cout << "uz tam je picus  " << nove1 << std::endl;

    auto it2 = std::lower_bound(v.begin(), v.end(), nove2);
    if (*it2 != nove2) v.insert(it2, nove2);
    else std::cout << "uz tam je picus  " << nove2 << std::endl;
    for (auto i : v)
        std::cout << i << std::endl;

    std::cout << std::endl;
    return 0;
}