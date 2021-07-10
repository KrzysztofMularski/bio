#include "additives.h"

#pragma once

class dnaStructure {

};

void toSet(vector<string>& oligos) {
    auto end = oligos.end();
    for (auto it = oligos.begin(); it != end; ++it) {
        end = remove(it + 1, end, *it);
    }
    oligos.erase(end, oligos.end());
}

vector<int> calcOligosWeights(const string& s1, const string& s2) {

    vector<int> weights;
    int size = s1.size();

    if (s1 == s2) {
        weights.push_back(k+1);
        return weights;
    }

    for (int i=1; i<size; i++)
        if (s1.substr(i, size-i) == s2.substr(0, size-i))
            weights.push_back(i);

    if (weights.size() == 0)
        weights.push_back(k);

    return weights;
}

void generateGraph(vector<string>& oligos, vector<int>** graph, int m) {
    for (int i=0; i<m; i++)
        for (int j=0; j<m; j++)
            graph[i][j] = calcOligosWeights(oligos[i], oligos[j]);
}

