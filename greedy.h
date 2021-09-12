#include "additives.h"

#pragma once

class Greedy {
private:
    vector<string> oligos;
    vector<int>** graph;
    int oligosSize;
    string firstOligo;

    vector<int> visited;
    vector<Pair> greedyResult;
    vector<string> greedyResultOligos;
public:
    Greedy(
        DnaStructure& structure,
        string& firstOligo
        ) :
        oligos(structure.getOligos()),
        graph(structure.getGraph()),
        oligosSize(structure.getOligosSize()),
        firstOligo(firstOligo) {}
    ~Greedy() {}

    void calculateResult() {
        vector<Pair> result;
        int currentDNAlength = k;
        Pair pair;

        int indexFirst = getIndex(oligos, firstOligo);
        pair = {indexFirst, -1};
        result.push_back(pair);
        visited.push_back(indexFirst);
        int index = indexFirst;

        while(true) {
            pair = findBest(index);
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
        
        for(auto pair: result)
        {
            greedyResultOligos.push_back(oligos[pair.index]);
        }
    }

    Pair findBest(int index) {
        vector<int> tempVisited = {index};
        int depth = 0;
        
    }
    
    // static Pair findBest(vector<int>* graphRow, int oligosSize, vector<int>& visited) {
    //     Pair best;
    //     best.index = -1;
    //     best.weight = -1;
    //     int bestCellWeight, min = k+2;
    //     for(int i = 0; i < oligosSize; i++)
    //     {
    //         auto it = find(visited.begin(), visited.end(), i);
    //         if(it == visited.end()){
    //             bestCellWeight = *min_element(graphRow[i].begin(), graphRow[i].end());
    //             if(bestCellWeight < min){
    //                 min = bestCellWeight;
    //                 best.weight = bestCellWeight;
    //                 best.index = i;
    //             }
    //         }      
    //     }
    //     return best;
    // }

    // static int findBestRecur(vector<int>* graphRow, int oligosSize, vector<int>& visited, vector<int>& tempVisited, int depth, int currentScore ) {

    // }

    // static Pair findBest(vector<int>* graphRow, int oligosSize, vector<int>& visited) {
    //     Pair best;
    //     best.index = -1;
    //     best.weight = -1;
    //     int bestCellWeight, score, min = k+2;
    //     for(int i = 0; i < oligosSize; i++)
    //     {
    //         auto it = find(visited.begin(), visited.end(), i);
    //         if(it == visited.end()){
    //             // current element not visited
    //             vector<int> tempVisited = {i};
    //             int bestCellWeight = *min_element(graphRow[i].begin(), graphRow[i].end());
    //             score = findBestRecur(graphRow, oligosSize, visited, tempVisited, 1, bestCellWeight);
    //             if(score < min){
    //                 min = score;
    //                 best.weight = bestCellWeight;
    //                 best.index = i;
    //             }
    //         }      
    //     }
    //     return best;
    // }

    static int getIndex(const vector<string>& v, const string& s)
    {
        auto it = find(v.begin(), v.end(), s);

        return it != v.end() ? it - v.begin() : -1; // if found then return index
    }

    vector<Pair> getResult() const {
        return greedyResult;
    }

    vector<string> getResultOligos() const {
        return greedyResultOligos;
    }
};