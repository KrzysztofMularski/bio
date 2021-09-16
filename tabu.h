#include "additives.h"
#include "greedy.h"

#pragma once

class Tabu{
    string originalDna, firstOligo;
    int dnaLength;
    vector<string>& oligosGreedy;
    vector<string>& oligosAll;
    vector<Location>& locations;
    int oligosAllSize;
    vector<int>& tabuList;
    vector<Pair>& result;
    vector<int>& visited;
    int oligoLength;
    float evaluation;
    vector<int>** graph;
    vector<string>& greedyResultOligos;

public:
    Tabu(
        string originalDna,
        int dnaLength,
        vector<string>& oligosGreedy,
        vector<string>& oligosAll,
        vector<Location>& locations,
        vector<Pair>& result,
        vector<int>** graph,
        vector<int>& visited,
        vector<int>& tabuList,
        vector<string>& greedyResultOligos
        ) :
        originalDna(originalDna),
        dnaLength(dnaLength),
        firstOligo(oligosAll[0]),
        oligosGreedy(oligosGreedy),
        oligosAll(oligosAll),
        locations(locations),
        result(result),
        graph(graph),
        visited(visited),
        tabuList(tabuList),
        greedyResultOligos(greedyResultOligos) {
            
        oligoLength = k;
        evaluation = (float)result.size() / (float)dnaLength;
        oligosAllSize = oligosAll.size();
    }

    void startSearch() {
        int maxIterations = 2;
        // int maxIterationsWithNoImprovement;

        for (int i=0; i<maxIterations; i++) {
            compaction();
            TO_PRINT & Printer::RESULTS_AFTER_COMPACTION && Printer::printResults("After compaction", result, oligosAll, originalDna);

            lengthening();
            TO_PRINT & Printer::RESULTS_AFTER_LENGTHENING && Printer::printResults("After lengthening", result, oligosAll, originalDna);
        }
    }

    //zagęszczanie
    void compaction()
    {
        while(true)
        {
            vector<float> rating = {0.0};
            int size = result.size();

            for(int i = 1; i < size; i++)
            {
                int current, next = 0, newNext = 0;
                // auto it = find(tabuList.begin(), tabuList.end(), result[i].index);
                // if (it != tabuList.end()) {
                //     // found something
                //     rating.push_back(-1.0);
                //     continue;
                // }

                string temp = oligosAll[result[i].index];
                
                current = result[i].weight;
                if(i != size - 1)
                {
                    next = result[i+1].weight;
                    newNext = graph[result[i-1].index][result[i+1].index][0];
                }
                
                rating.push_back((float)(size-1) / (float)(dnaLength - (current + next - newNext)));
            }

            int bestIndex = max_element(rating.begin(), rating.end()) - rating.begin();

            if(rating[bestIndex] >= evaluation)
            {
                int current = result[bestIndex].weight; // weight of best element
                int next = 0, newNext = 0;

                if (bestIndex != size-1) {
                    next = result[bestIndex+1].weight;
                    newNext = graph[result[bestIndex-1].index][result[bestIndex+1].index][0];
                }

                dnaLength = dnaLength - current - next + newNext;
                
                evaluation = rating[bestIndex];

                add(tabuList, result[bestIndex].index);
                result.erase(result.begin() + bestIndex);
                visited.erase(visited.begin() + bestIndex);

                // bestIndex now points to the next element after the best which was removed
                if (bestIndex != size) {
                    result[bestIndex].weight = graph[bestIndex-1][bestIndex][0];
                }
            }
            else {
                break;
            }

        }
    }

    // wydłużanie
    void lengthening() {
        Greedy greedy(
            oligosAll, locations, graph, oligosAllSize,
            firstOligo, dnaLength, visited,
            result, greedyResultOligos, tabuList,
            Greedy::TYPE_TABU_LENGTHENING);
        
        greedy.calculateResult();
    }

    vector<Pair>& getResult() {
        return result;
    }

    ~Tabu() {}
};