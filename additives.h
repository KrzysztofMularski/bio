#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

enum class Random_Type { LINEAR, GAUSSIAN };

int n;  // DNA length
int k;  // oligonucleotide length
int GREEDY_DEPTH;
int LOCATION_RANGE;
Random_Type LOCATION_RANDOM_TYPE;
int INSTANCE_COUNTER;
int TABU_LIST_LENGTH;
int MAX_TABU_ITERATIONS;
int MAX_TABU_ITERATIONS_WITH_NO_IMPROVEMENT;
int TO_PRINT;

struct Pair {
    int index;
    int weight;
};

struct OligosWithLocation {
    string oligo;
    int index;
};

struct Location {
    int left;
    int right;
};

std::ostream& operator<<(std::ostream& os, const Location& loc)
    {
        os << "~[" << loc.left << ", " << loc.right << "]";
        return os;
    }

bool locationNotFit(const int lastOligoIndex, const Location& loc) {
    return lastOligoIndex < loc.left || lastOligoIndex > loc.right;
}

void add(vector<int>& tabuList, const int& element) {
    tabuList.push_back(element);
    if (tabuList.size() > TABU_LIST_LENGTH)
        tabuList.erase(tabuList.begin());
}
