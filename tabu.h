#include "additives.h"
#include <iostream>
#include <algorithm>
#include <vector>

#pragma once

class Tabu{
    string originalDna, firstOligo;
    int dnaLength;
    vector<string> oligosGreedy;
    vector<string> oligosAll;
    vector<int> tabuList;
    vector<Pair> result;
    int oligoLength;
    float evaluation;
    vector<int>** graph;

public:
    Tabu(
        string originalDna,
        int dnaLength,
        vector<string>& oligosGreedy,
        vector<string>& oligosAll,
        vector<Pair>& result,
        vector<int> **graph
        ) :
        originalDna(originalDna),
        dnaLength(dnaLength),
        firstOligo(oligosGreedy[0]),
        oligosGreedy(oligosGreedy),
        oligosAll(oligosAll),
        result(result),
        graph(graph) {
            
        oligoLength = k;
        evaluation = (float)result.size() / (float)dnaLength;
    }

    void startSearch() {
        int maxIterations = 1;
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

                tabuList.push_back(result[bestIndex].index);
                result.erase(result.begin() + bestIndex);

                // bestIndex now points to the next element after the best which was removed
                if (bestIndex != size) {
                    result[bestIndex].weight = graph[bestIndex-1][bestIndex][0];
                }
            }
            else
                break;

        }
    }

    void lengthening() {

    }

    vector<Pair>& getResult() {
        return result;
    }

    ~Tabu() {}
};