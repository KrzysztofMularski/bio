#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

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

int n;  // DNA length
int k;  // oligonucleotide length
int GREEDY_DEPTH;
int LOCATION_RANGE;
int LOCATION_RANDOM_TYPE;
int INSTANCE_COUNTER = 1;
int TABU_LIST_LENGTH =  10;