#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <functional>
#include <sstream>
#include <random>

using namespace std;

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v)
{
    for (T const &i: v) {
        os << i << " ";
    }
    return os;
}

struct Location {
    int left;
    int right;
};

struct OligoWithLocationWithOrder {
    string oligo;
    Location location;
    int indexOrder;
};

inline static void vecs_sort_newOrder(vector<OligoWithLocationWithOrder>& vecs) {
    sort(vecs.begin(), vecs.end(), [](const OligoWithLocationWithOrder& a, const OligoWithLocationWithOrder& b) { return a.indexOrder < b.indexOrder; });
}

vector<string> oligos = {
    "AAAA1",
    "AAAA2",
    "AAAA3",
    "AAAA4",
    "AAAA5",
    "AAAA6",
    "AAAA7",
    "AAAA8",
    "AAAA9",
    "AAAA0"
};

vector<Location> locations = {
    {0, 1},
    {0, 2},
    {0, 3},
    {0, 4},
    {0, 5},
    {0, 6},
    {0, 7},
    {0, 8},
    {0, 9},
    {0, 0},
};

int main() {

    srand(1);

    int oligosSize = 10;

    vector<int> allIndexesSorted(oligosSize);
    for (int i=0; i<oligosSize; ++i) {
        allIndexesSorted[i] = i;
    }

    vector<int> mixedIndexes(oligosSize);
    for (int i=0; i<oligosSize; ++i) {
        const int randomIndex = rand() % allIndexesSorted.size();
        mixedIndexes[i] = allIndexesSorted[randomIndex];
        allIndexesSorted.erase(allIndexesSorted.begin() + randomIndex);
    }

    vector<OligoWithLocationWithOrder> oligosWithLocationsWithOrder(oligosSize);
    for (int i=0; i<oligosSize; ++i) {
        oligosWithLocationsWithOrder[i] = { oligos[i], locations[i], mixedIndexes[i] };
    }

    vecs_sort_newOrder(oligosWithLocationsWithOrder);

    for (int i=0; i<oligosSize; ++i) {
        oligos[i] = oligosWithLocationsWithOrder[i].oligo;
        locations[i] = oligosWithLocationsWithOrder[i].location;
    }

    return 0;
}