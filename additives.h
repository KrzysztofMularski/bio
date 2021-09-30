#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <set>

using namespace std;

enum class Random_Type { LINEAR, GAUSSIAN };

int n;
int k;
float POSITIVE_ERRORS_PERCENTAGE;
float NEGATIVE_ERRORS_PERCENTAGE;
int LOCATION_RANGE;
int GREEDY_DEPTH;
Random_Type LOCATION_RANDOM_TYPE;
int INSTANCE_COUNTER = 1;
int TABU_LIST_LENGTH;
int TABU_LIST_CLUSTERS_LENGTH;
int MAX_TABU_ITERATIONS;
int MAX_TABU_ITERATIONS_WITH_NO_IMPROVEMENT;
int GLOBAL_MAX_ITERATIONS;
int CLUSTER_OVERLAP_CRITERION;
int TO_PRINT;

struct Pair {
    int index;
    int weight;
};

struct OligoWithPosition {
    string oligo;
    int index;
};

struct Location {
    int left;
    int right;
};

struct OligoWithLocation {
    string oligo;
    Location location;
};

struct OligoWithLocationWithOrder {
    string oligo;
    Location location;
    int indexOrder;
};

struct Dictionary {
    vector<string> oligos;
    vector<Location> locations;
} DICTIONARY;

std::ostream& operator<<(std::ostream& os, const Location& loc) {
    os << "[" << loc.left << ", " << loc.right << "]";
    return os;
}

inline bool locationNotFit(const int lastOligoIndex, const Location& loc) {
    return lastOligoIndex < loc.left || lastOligoIndex > loc.right;
}

inline void add(vector<int>& tabuList, int element) {
    tabuList.push_back(element);
    if (tabuList.size() > TABU_LIST_LENGTH) {
        tabuList.erase(tabuList.begin());
    }
}

inline void addCluster(vector<size_t>& tabuListClusters, size_t clusterHash) {
    tabuListClusters.push_back(clusterHash);
    if (tabuListClusters.size() > TABU_LIST_CLUSTERS_LENGTH) {
        tabuListClusters.erase(tabuListClusters.begin());
    }
}

template <typename Type>
int getIndex(const vector<Type>& v, const Type& t) {
    auto it = find(v.begin(), v.end(), t);
    return it != v.end() ? it - v.begin() : -1; // if found then return index
}

inline size_t calcHash(const vector<Pair>& res, const vector<string>& oligos) {
    stringstream ss;
    for (const Pair& pair : res)
        ss << "_" << oligos[pair.index] << "_" << pair.weight;
    hash<string> str_hash;
    return str_hash(ss.str());
}

inline size_t calcHashCluster(const vector<int>& cluster, const vector<string>& oligos) {
    stringstream ss;
    for (const int& index : cluster) {
        // finding global index first
        string oligo = oligos[index];
        const int globalIndex = getIndex(DICTIONARY.oligos, oligo);
        ss << globalIndex << "_";
    }
    hash<string> str_hash;
    return str_hash(ss.str());
}

inline void addNewClusters(const vector<vector<int>>& newClusters, set<vector<int>>& clustersContainer) {
    for (const vector<int>& cluster : newClusters) {
        clustersContainer.insert(cluster);
    }
}
