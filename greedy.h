#include "additives.h"

#pragma once

class Greedy {
private:
    DnaStructure* structure;
    string firstOligo;
    vector<Pair> greedyResult;
public:
    Greedy(DnaStructure* structure, string& firstOligo) : structure(structure), firstOligo(firstOligo) {}
    ~Greedy() {}

    void calculateResult() {
        vector<int> visited;
        vector<Pair> result;
        int currentDNAlength = k;
        Pair pair;
        vector<string> oligos = structure->getOligos();
        vector<int>** graph = structure->getGraph();
        int m = structure->getOligosSize();

        int indexFirst = getIndex(oligos, firstOligo);
        pair.index = indexFirst;
        pair.weight = -1;
        result.push_back(pair);
        visited.push_back(indexFirst);
        int index = indexFirst;

        while(true) {
            pair = findBest(graph[index], m, visited);
            if (pair.index == -1)
                break;
            currentDNAlength += pair.weight;
            if (currentDNAlength > n)
                break;
            result.push_back(pair);
            visited.push_back(pair.index);
            index = pair.index;
        }
        greedyResult = result;
    }

    static int getIndex(const vector<string>& v, const string& s)
    {
        auto it = find(v.begin(), v.end(), s);
    
        if (it != v.end())
            return it - v.begin();
        else
            return -1;
    }

    static Pair findBest(vector<int>* graphRow, int m, vector<int>& visited) {
        Pair best;
        best.index = -1;
        best.weight = -1;
        int bestCellWeight, min = k+2;
        for(int i = 0; i < m; i++)
        {
            auto it = find(visited.begin(), visited.end(), i);
            if(it == visited.end()){
                bestCellWeight = *min_element(graphRow[i].begin(), graphRow[i].end());
                if(bestCellWeight < min){
                    min = bestCellWeight;
                    best.weight = bestCellWeight;
                    best.index = i;
                }
            }      
        }
        return best;
    }

    vector<Pair> getResult() {
        return greedyResult;
    }
};