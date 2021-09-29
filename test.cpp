#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <functional>
#include <sstream>
#include <random>
#include <set>

using namespace std;

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v)
{
    for (T const &i: v) {
        os << i << " ";
    }
    return os;
}


int main() {


    

    return 0;
}