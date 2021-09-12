#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

struct Pair{
    int index;
    int weight;
};

// struct Rank{
//     int oligoIndex;
//     string oligoShortened;
//     float rank;
// };

#define n 20   // DNA length
#define k 4    // oligonucleotide length
#define GREEDY_DEPTH 3

using namespace std;