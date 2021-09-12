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
        int oligosLeft = oligosSize - 1 + 50;
        int ii = 0;
        while(true) {
            pair = findBest(ii, index, 1, {}, oligosLeft);
            if (pair.index == -1)
                break;
            currentDNAlength += pair.weight;
            result.push_back(pair);
            visited.push_back(pair.index);

            if (currentDNAlength >= n || visited.size() == oligosSize)
                break;
            
            index = pair.index;
            oligosLeft--;
            ii++;
        }
        greedyResult = result;
    }

    // Pair findBest(int index) {
    //     Pair best = { -1, -1 };
    //     for (int i=0; i < oligosSize; i++) {
    //         auto it = find(visited.begin(), visited.end(), i);
    //         if (it == visited.end()) {
    //             // if not visited:
                
    //         }
    //         Pair candidate = findBestRec()
    //     }
    // }

    Pair findBest(int ii, int index, int depth, vector<int> tempVisited, int oligosLeft) {
        vector<int> newTempVisited (tempVisited);
        newTempVisited.push_back(index);
        Pair best = { -1, -1 };
        int min = 99999;
        for (int i=0; i < oligosSize; i++) {
            auto it = find(visited.begin(), visited.end(), i);
            auto itTemp = find(newTempVisited.begin(), newTempVisited.end(), i);
            if (it == visited.end() && itTemp == newTempVisited.end()) {
                int bestOligoWeight = 0;
                int bestParentOligoWeight = 0;
                int flag = 0;
                if (depth == GREEDY_DEPTH || oligosLeft == 0 || oligosLeft == 1) {
                    flag = 1;
                    bestOligoWeight = graph[index][i][0];
                } else if (depth == 1 && GREEDY_DEPTH > 1 && oligosLeft > 1) {
                    flag = 2;
                    bestOligoWeight = graph[index][i][0] + findBest(ii, i, depth+1, newTempVisited, oligosLeft-1).weight;
                    bestParentOligoWeight = graph[index][i][0];
                } else {
                    flag = 3;
                    bestOligoWeight = graph[index][i][0] + findBest(ii, i, depth+1, newTempVisited, oligosLeft-1).weight;
                }
                
                if (bestOligoWeight < min) {
                    min = bestOligoWeight;
                    if (flag == 2) {
                        best = { i, bestParentOligoWeight };
                    } else {
                        best = { i, bestOligoWeight };
                    }
                }
            }
        }
        return best;
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
};