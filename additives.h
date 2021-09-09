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

#define n 20   // DNA length
#define k 4    // oligonucleotide length
#define GREEDY_DEPTH 3
#define LOCATION_RANGE 50
