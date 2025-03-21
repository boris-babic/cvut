#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cmath>
#include <cfloat>
#include <cassert>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <span>
#include <algorithm>
#include <memory>
#include <compare>
#include <complex>

int main() {
    std::vector<int> a;
    a.resize(10);
    a.insert(a.begin() + 5, 5);
    a.shrink_to_fit();
    return 0;

}