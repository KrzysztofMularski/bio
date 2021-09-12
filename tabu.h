#include "additives.h"
#include <iostream>
#include <algorithm>
#include <vector>

#pragma once

class Tabu{
    string dna, firstOligo;
    vector<string> oligosGreedy;
    vector<string> oligosAll;
    vector<int> tabuList;
    vector<Pair> greedyResult;
    int oligoLength;
    float evaluation;
    vector<int>** graph;

public:
    Tabu(string dna, vector<string>& oligosGreedy, vector<string>& oligosAll, vector<Pair>& greedyResult, vector<int> **graph): 
        dna(dna), firstOligo(oligosGreedy[0]), oligosGreedy(oligosGreedy), oligosAll(oligosAll), greedyResult(greedyResult), graph(graph) {
        oligoLength = k;
        evaluation = (float)greedyResult.size() / dna.size();
    }

    //zagęszczanie
    void compaction()
    {
        while(true)
        {
            vector<float> rating = {0.0};
            float size = (float)greedyResult.size();
            int current, next = 0, newNext = 0;

            for(int i = 1; i < size; i++)
            {
                auto it = find(tabuList.begin(), tabuList.end(), i);
                if (it != tabuList.end()) {
                    rating.push_back(-1.0);
                    continue;
                }
                
                current = greedyResult[i].weight;
                if(i != size - 1)
                {
                    next = greedyResult[i+1].weight;
                    newNext = graph[i-1][i+1][0];
                }

                rating.push_back((size-1) / (dna.length() - (current + next - newNext)));
            }

            int bestIndex = max_element(rating.begin(), rating.end()) - rating.begin();

            if(rating[bestIndex] >= evaluation)
            {
                evaluation = rating[bestIndex];

                tabuList.push_back(greedyResult[bestIndex].index);
                greedyResult.erase(greedyResult.begin() + bestIndex);
            }
            else
                break;

        }
    }

    

    ~Tabu() {}
};