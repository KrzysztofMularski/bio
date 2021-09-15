#include "additives.h"

#pragma once

class Greedy {
private:
    vector<string>& oligos;
    vector<int>** graph;
    int oligosSize;
    string firstOligo;
    int currentDNAlength;

    vector<int>& visited;
    vector<Pair>& greedyResult;
    vector<string>& greedyResultOligos;

    vector<int>& tabuList;
    int greedyType; // 0-greedy, 1-tabu(lengthening)
public:
    Greedy(
        DnaStructure& structure,
        string& firstOligo,
        vector<int>& visited,
        vector<Pair>& greedyResult,
        vector<string>& greedyResultOligos,
        vector<int>& tabuList,
        int greedyType = 0
        ) :
        oligos(structure.getOligos()),
        graph(structure.getGraph()),
        oligosSize(structure.getOligosSize()),
        firstOligo(firstOligo),
        currentDNAlength(0),
        visited(visited),
        greedyResult(greedyResult),
        greedyResultOligos(greedyResultOligos),
        tabuList(tabuList),
        greedyType(greedyType) {}
    
    Greedy(
        vector<string>& oligos,
        vector<int>** graph,
        int oligosSize,
        string firstOligo,
        int currentDNAlength,
        vector<int>& visited,
        vector<Pair>& greedyResult,
        vector<string>& greedyResultOligos,
        vector<int>& tabuList,
        int greedyType = 1
        ) :
        oligos(oligos),
        graph(graph),
        oligosSize(oligosSize),
        firstOligo(firstOligo),
        currentDNAlength(currentDNAlength),
        visited(visited),
        tabuList(tabuList),
        greedyResult(greedyResult),
        greedyResultOligos(greedyResultOligos),
        greedyType(greedyType) {}
    ~Greedy() {}

    static const int TYPE_GREEDY = 0;
    static const int TYPE_TABU_LENGTHENING = 1;

    void calculateResult() {
        Pair pair;
        int index;

        if (greedyResult.size() == 0) {
            int indexFirst = getIndex(oligos, firstOligo);
            pair = {indexFirst, -1};
            greedyResult.push_back(pair);
            visited.push_back(indexFirst);
            currentDNAlength = k;
            index = indexFirst;
        } else {
            index = greedyResult.back().index;
        }

        while(true) {
            if (greedyType == Greedy::TYPE_GREEDY) {
                int oligosNumber = 0;
                pair = findBest(index, 1, {}, 0, oligosNumber);
            } else {
                pair = findBestTabu(index);
            }
            if (pair.index == -1)
                break;
            greedyResult.push_back(pair);
            visited.push_back(pair.index);
            add(tabuList, pair.index);
            currentDNAlength += pair.weight;

            if (currentDNAlength >= n || visited.size() == oligosSize)
                break;
            
            index = pair.index;
        }
        // if (greedyType == Greedy::TYPE_GREEDY) {
        //     result = {
        //         {2, -1},
        //         {5, 1},
        //         {6, 1},
        //         {1, 2},
        //         {4, 2}
        //     };
        // }
        
        for(auto pair : greedyResult)
        {
            greedyResultOligos.push_back(oligos[pair.index]);
        }
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

    Pair findBest(int index, int depth, vector<int> tempVisited, int newLength, int& prevOligosNumber) {
    
        vector<int> newTempVisited (tempVisited);
        newTempVisited.push_back(index);
        Pair best = { -1, -1 };
        int maxOligosNumber = 0;
        int min = 99999;
        for (int i=0; i < oligosSize; i++) {
            auto it = find(visited.begin(), visited.end(), i);
            auto itTemp = find(newTempVisited.begin(), newTempVisited.end(), i);
            if (it == visited.end() && itTemp == newTempVisited.end()) {
                int bestOligoWeight = 0;
                int bestParentOligoWeight = -1;
                int newOligosNumber = 0;
                if (depth == GREEDY_DEPTH) {
                    bestOligoWeight = graph[index][i][0];
                    if (currentDNAlength + newLength + bestOligoWeight > n)
                        continue;
                    newOligosNumber += 1;
                } else {
                    bestOligoWeight = graph[index][i][0];
                    if (currentDNAlength + newLength + bestOligoWeight > n)
                        continue;
                    newOligosNumber += 1;
                    int bestWeight = findBest(i, depth+1, newTempVisited, newLength+bestOligoWeight, newOligosNumber).weight;
                    if (bestWeight != -1)
                        bestOligoWeight += bestWeight;
                    if (depth == 1 && GREEDY_DEPTH > 1) {
                        bestParentOligoWeight = graph[index][i][0];
                    }
                }
                if (newOligosNumber > maxOligosNumber) {
                    maxOligosNumber = newOligosNumber;
                    min = bestOligoWeight;
                    if (bestParentOligoWeight != -1) {
                        best = { i, bestParentOligoWeight };
                    } else {
                        best = { i, bestOligoWeight };
                    }
                } else if (newOligosNumber == maxOligosNumber && bestOligoWeight < min) {
                    min = bestOligoWeight;
                    if (bestParentOligoWeight != -1) {
                        best = { i, bestParentOligoWeight };
                    } else {
                        best = { i, bestOligoWeight };
                    }
                }
            }
        }
        prevOligosNumber += maxOligosNumber;
        return best;
    }
    
    Pair findBestTabu(int index) {
        Pair best = { -1, -1 };
        int bestOligoWeight;
        int min = k+1;
        for(int i = 0; i < oligosSize; i++)
        {
            auto it = find(visited.begin(), visited.end(), i);
            auto itTabu = find(tabuList.begin(), tabuList.end(), i);
            if(it == visited.end() && itTabu == tabuList.end()){
                bestOligoWeight = graph[index][i][0];
                if (currentDNAlength + bestOligoWeight > n)
                    continue;
                if (bestOligoWeight < min) {
                    min = bestOligoWeight;
                    best.weight = bestOligoWeight;
                    best.index = i;
                }
            }      
        }
        return best;
    }

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

    vector<Pair>& getResult() {
        return greedyResult;
    }

    int getResultDnaLength() const {
        return currentDNAlength;
    }

    vector<string>& getResultOligos() {
        return greedyResultOligos;
    }

    vector<int>& getVisited() {
        return visited;
    }
};