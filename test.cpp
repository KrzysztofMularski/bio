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

    set<vector<int>> setOfVectors;

    vector<int> vec1 = {1, 2, 3};
    vector<int> vec2 = {1, 2, 3};
    vector<int> vec3 = {1, 2, 4};

    setOfVectors.insert(vec1);
    setOfVectors.insert(vec2);
    setOfVectors.insert(vec3);
    

    return 0;
}