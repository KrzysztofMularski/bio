#pragma once

#include "additives.h"
#include "greedy.h"

class Tabu {
private:
    string originalDna, firstOligo;
    int dnaLength;
    vector<string>& oligosAll;
    vector<Location>& locations;
    int oligosAllSize;
    vector<int>& tabuList;
    vector<Pair>& result;
    int oligoLength;
    float evaluation;
    vector<int>** graph;
    set<vector<int>> tabuClusters;

public:
    Tabu(
        string originalDna,
        int dnaLength,
        vector<string>& oligosAll,
        vector<Location>& locations,
        vector<Pair>& result,
        vector<int>** graph,
        vector<int>& tabuList
        ) :
        originalDna(originalDna),
        dnaLength(dnaLength),
        oligosAll(oligosAll),
        locations(locations),
        result(result),
        graph(graph),
        tabuList(tabuList) {
        
        firstOligo = oligosAll[result[0].index];
        oligoLength = k;
        evaluation = calculateEval(result.size(), dnaLength);
        oligosAllSize = oligosAll.size();
    }

    inline static float calculateEval(const int resultSize, const int dnaLength) {
        return (float)resultSize / (float)dnaLength;
    }

    inline static void removeAllMatchingElements(vector<int>& vec, int elem) {
        vec.erase(remove(vec.begin(), vec.end(), elem), vec.end());
    }

    void removeMisplacedOligosInResult() {
        int currentIndex = 1; // for i=0, currentIndex = 0
        for (int i=0; i<result.size(); ++i) {
            currentIndex += result[i].weight; 
            if (locationNotFit(currentIndex, locations[result[i].index])) {
                dnaLength = currentIndex - result[i].weight + k;
                result.erase(result.begin() + i, result.end());
            }
        }
    }

    int predictMisplacedOligos(int resultIndex, int diff) {
        int currentIndex = 1 - diff; // for i=0, currentIndex = 0
        for (int i=0; i<result.size(); ++i) {
            currentIndex += result[i].weight;
            if (i > resultIndex && locationNotFit(currentIndex, locations[result[i].index])) {
                return i;
            }
        }
        return -1;
    }

    float predictEval(int index) {
        int oligosNumberInResult = 0;
        int newDnaLength = k + 1; // after first (first.weight = -1), newDnaLength = k
        for (int i=0; i<result.size(); ++i) {
            if (i < index) {
                ++oligosNumberInResult;
                newDnaLength += result[i].weight;
            } else {
                return (float)oligosNumberInResult / float(newDnaLength);
            }
        }
    }

    void startSearch() {

        int iterationsWithNoImprovement = 0;
        for (int i=0; i<MAX_TABU_ITERATIONS; i++) {
            compaction();
            TO_PRINT & Printer::RESULTS_AFTER_COMPACTION && Printer::printResults("After compaction", result, oligosAll, originalDna);

            lengthening();
            TO_PRINT & Printer::RESULTS_AFTER_LENGTHENING && Printer::printResults("After lengthening", result, oligosAll, originalDna);
            
            vector<vector<int>> afterIterationClusters;
            findGlobalClusters(afterIterationClusters);
            addNewClusters(afterIterationClusters, tabuClusters);

            float currentEval = calculateEval(result.size(), dnaLength);
            if (currentEval > evaluation) {
                evaluation = currentEval;
            } else {
                iterationsWithNoImprovement++;
            }
            if (MAX_TABU_ITERATIONS_WITH_NO_IMPROVEMENT != -1 && iterationsWithNoImprovement >= MAX_TABU_ITERATIONS_WITH_NO_IMPROVEMENT) {
                break;
            }
        }
    }

    void findClusters(vector<vector<int>>& clusters) {
        result.push_back({100, 2}); // adding temp element with weight != 1
        vector<int> newCluster;
        for (int i=2; i<result.size(); ++i) {
            if (result[i].weight == 1) {
                newCluster.push_back(i-1);
            } else if (!newCluster.empty()) {
                newCluster.push_back(i-1);
                clusters.push_back(newCluster);
                newCluster.clear();
            }
        }
        result.pop_back();  // removing temp element
    }

    void findGlobalClusters(vector<vector<int>>& clusters) {
        result.push_back({100, 2}); // adding temp element with weight != 1
        vector<int> newCluster;
        for (int i=2; i<result.size(); ++i) {
            if (result[i].weight == 1) {
                newCluster.push_back(result[i-1].index);
            } else if (!newCluster.empty()) {
                newCluster.push_back(result[i-1].index);
                clusters.push_back(newCluster);
                newCluster.clear();
            }
        }
        result.pop_back();  // removing temp element

        // converting indexes to dictionary scope
        for (vector<int>& cluster : clusters) {
            for (int& localIndex : cluster) {
                string localString = oligosAll[localIndex];
                localIndex = getIndex(DICTIONARY.oligos, localString);
            }
        }
    }

    //zagęszczanie
    void compaction()
    {
        while(true)
        {
            vector<float> rating = {-1.0};
            int size = result.size();

            for(int i = 1; i < size; i++)
            {
                int current = result[i].weight;
                int next = 0, newNext = 0;

                if(i != size - 1)
                {
                    next = result[i+1].weight;
                    newNext = graph[result[i-1].index][result[i+1].index][0];
                }

                int diff = current + next - newNext;

                if (predictMisplacedOligos(i, diff) == -1) {
                    rating.push_back((float)(size-1) / (float)(dnaLength - diff));
                } else {
                    rating.push_back(predictEval(i));
                }
                
            }

            vector<vector<int>> clusters;
            findClusters(clusters);

            for (const vector<int>& cluster : clusters) {
                int current = 0;
                for (const int& index : cluster) {
                    current += result[index].weight;
                }
                int next = 0, newNext = 0;

                const int lastIndexInCluster = cluster.back();

                if(lastIndexInCluster != size - 1)
                {
                    next = result[lastIndexInCluster+1].weight;
                    newNext = graph[result[cluster[0]-1].index][result[lastIndexInCluster+1].index][0];
                }

                int diff = current + next - newNext;

                if (predictMisplacedOligos(lastIndexInCluster, diff) == -1) {
                    rating.push_back((float)(size-cluster.size()) / (float)(dnaLength - diff));
                } else {
                    rating.push_back(predictEval(cluster[0]));
                }
            }

            int bestIndex = max_element(rating.begin(), rating.end()) - rating.begin();
            // cout << "bestIndex: " << bestIndex << endl;
            // cout << ">> (" << (rating[bestIndex] >= evaluation ? "+" : "-") << ")" << endl;
            // cout << ">> (" << (bestIndex < size ? "one" : "cluster") << ")" << endl;
            // cout << ">> [";
            // if (bestIndex >= size) {
            //     for (const auto& ids : clusters[bestIndex-size]) {
            //         cout << ids << ", ";
            //     }
            //     cout << "]" << endl;
            // }

            if(rating[bestIndex] >= evaluation)
            {
                evaluation = rating[bestIndex];
                if (bestIndex < size) {
                    int current = result[bestIndex].weight; // weight of best element
                    int next = 0, newNext = 0;

                    if (bestIndex != size-1) {
                        next = result[bestIndex+1].weight;
                        newNext = graph[result[bestIndex-1].index][result[bestIndex+1].index][0];
                    }

                    dnaLength = dnaLength - current - next + newNext;

                    add(tabuList, result[bestIndex].index);
                    result.erase(result.begin() + bestIndex);

                    // bestIndex now points to the next element after the best which was removed
                    if (bestIndex != size) {
                        result[bestIndex].weight = graph[result[bestIndex-1].index][result[bestIndex].index][0];
                    }

                    removeMisplacedOligosInResult();

                } else {
                    bestIndex -= size;
                    int current = 0;
                    vector<int>& cluster = clusters[bestIndex];
                    for (const int& index : cluster) {
                        current += result[index].weight;
                    }
                    int next = 0, newNext = 0;

                    const int lastIndexInCluster = cluster.back();

                    if(lastIndexInCluster != size - 1)
                    {
                        next = result[lastIndexInCluster+1].weight;
                        newNext = graph[result[cluster[0]-1].index][result[lastIndexInCluster+1].index][0];
                    }

                    int diff = current + next - newNext;

                    dnaLength -= diff;

                    add(tabuList, result[cluster[0]].index);
                    result.erase(result.begin() + cluster.front(), result.begin() + cluster.back() + 1);

                    bestIndex = cluster.front();
                    // bestIndex now points to the next element after the best which was removed
                    if (bestIndex != size) {
                        result[bestIndex].weight = graph[result[bestIndex-1].index][result[bestIndex].index][0];
                    }

                    removeMisplacedOligosInResult();
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
            firstOligo, dnaLength, result, tabuList,
            Greedy::TYPE_TABU_LENGTHENING);
        
        greedy.calculateResult();
        dnaLength = greedy.getResultDnaLength();
    }

    vector<Pair>& getResult() {
        return result;
    }

    vector<vector<int>> getClusters() {
        vector<vector<int>> tabuClustersVector(tabuClusters.begin(), tabuClusters.end());
        return tabuClustersVector;
    }

    ~Tabu() {}
};