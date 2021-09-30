#pragma once

#include "additives.h"

class Greedy {
private:
    vector<string>& oligos;
    vector<Location>& locations;
    vector<int>** graph;
    int oligosSize;
    string firstOligo;
    int currentDNAlength;
    vector<Pair>& greedyResult;
    vector<int>& tabuList;
    vector<size_t> tabuListClusters;
    vector<vector<int>> prevClusters;
    vector<Location> prevClustersLocations;
    int greedyType; // 0-greedy, 1-tabu(lengthening)

public:
    Greedy(
        DnaStructure& structure,
        string& firstOligo,
        vector<Pair>& greedyResult,
        vector<int>& tabuList,
        vector<size_t> tabuListClusters,
        int greedyType = 0
        ) :
        oligos(structure.getOligos()),
        locations(structure.getLocations()),
        graph(structure.getGraph()),
        oligosSize(structure.getOligosSize()),
        firstOligo(firstOligo),
        currentDNAlength(0),
        greedyResult(greedyResult),
        tabuList(tabuList),
        tabuListClusters(tabuListClusters),
        greedyType(greedyType) {}
    
    Greedy(
        vector<string>& oligos,
        vector<Location>& locations,
        vector<int>** graph,
        int oligosSize,
        string firstOligo,
        int currentDNAlength,
        vector<Pair>& greedyResult,
        vector<int>& tabuList,
        int greedyType = 1
        ) :
        oligos(oligos),
        locations(locations),
        graph(graph),
        oligosSize(oligosSize),
        firstOligo(firstOligo),
        currentDNAlength(currentDNAlength),
        tabuList(tabuList),
        greedyResult(greedyResult),
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
            currentDNAlength = k;
            index = indexFirst;
        } else {
            index = greedyResult.back().index;
        }
        
        while(true) {
            if (greedyType == Greedy::TYPE_GREEDY) {
                int oligosNumber = 0;
                if (GREEDY_DEPTH > 1)
                    pair = findBest(index, 1, 0, oligosNumber);
                else {

                    int prevClustersSize = prevClusters.size();
                    if (prevClustersSize) {
                        int currentlyAvailableIndex = currentDNAlength - k + 1;
                        vector<int> correctClusterIndexes;
                        correctClusterIndexes.reserve(prevClustersSize);
                        string lastOligoString = oligos[greedyResult.back().index];
                        for (int i=0; i<prevClustersSize; ++i) {
                            if (locationFits(
                                    prevClustersLocations[i],
                                    currentlyAvailableIndex,
                                    lastOligoString,
                                    oligos[prevClusters[i][0]]
                                )) {
                                correctClusterIndexes.push_back(i);
                            }
                        }

                        int correctClusterIndexesSize = correctClusterIndexes.size();

                        if (correctClusterIndexesSize == 0) {
                            pair = findBestShallow(index);
                        } else if (correctClusterIndexesSize == 1) {
                            index = applyClusterToResult(prevClusters[correctClusterIndexes[0]], index);
                            if (currentDNAlength >= n || index == -1) {
                                break;
                            }
                            continue;
                        } else {
                            bool clusterApplied = false;
                            for (const int& correctClusterIndex : correctClusterIndexes) {
                                size_t clusterHash = calcHashCluster(prevClusters[correctClusterIndex], oligos);
                                auto itTabuClusters = find(tabuListClusters.begin(), tabuListClusters.end(), clusterHash);
                                if (itTabuClusters == tabuListClusters.end()) {
                                    index = applyClusterToResult(prevClusters[correctClusterIndex], index);
                                    clusterApplied = true;
                                    addCluster(tabuListClusters, clusterHash);
                                    break;
                                }
                            }
                            if (!clusterApplied) {
                                pair = findBestShallow(index);
                            } else {
                                if (currentDNAlength >= n || index == -1) {
                                    break;
                                }
                                continue;
                            }
                            
                        }
                    } else {
                        pair = findBestShallow(index);
                    }
                }
            } else {
                pair = findBestTabu(index);
            }
            if (pair.index == -1)
                break;

            greedyResult.push_back(pair);
            currentDNAlength += pair.weight;

            if (currentDNAlength >= n)
                break;
            
            index = pair.index;
        }
    }

    vector<Pair> filteredResults() {
        vector<Pair> elems;
        for(const Pair& elem : greedyResult) {
            if(elem.weight==-1) {
                elems.push_back(elem);
            }
        }
        return elems;
    }

    int applyClusterToResult(const vector<int>& cluster, const int lastIndex) {
        int weight = graph[lastIndex][cluster[0]][0];
        if (currentDNAlength + weight >= n)
            return -1;
        greedyResult.push_back( { cluster[0], weight } );
        currentDNAlength += weight;
        for (int i=1; i<cluster.size(); ++i) {
            if (currentDNAlength + 1 >= n)
                return -1;
            greedyResult.push_back( { cluster[i], 1 } );
            currentDNAlength += 1;
            
        }
        return cluster.back();
    }

    bool locationFits(const Location& loc, const int& currentIndex, const string& lastOligo, const string& clusterFirstOligo) {
        int leftIndex = currentIndex;
        int rightIndex = currentIndex + CLUSTER_OVERLAP_CRITERION;
        if (rightIndex < loc.left || loc.right < leftIndex)
            return false;
        vector<int> weights = DnaStructure::calcOligosWeights(lastOligo, clusterFirstOligo);
        if (weights[0] > CLUSTER_OVERLAP_CRITERION + 1)
            return false;
        return true;
    }

    bool positionsInClusterCorrect(const vector<int>& cluster, int currentIndex) {
        int predictedIndex = currentIndex + 1;
        for (const int& index : cluster) {
            if (locationNotFit(predictedIndex, locations[index])) {
                return false;
            }
            ++predictedIndex;
        }
        return true;
    }

    Pair findBest(int index, int depth, int newLength, int& prevOligosNumber) {
        Pair best = { -1, -1 };
        int maxOligosNumber = 0;
        int min = 99999;
        for (int i=0; i < oligosSize; i++) {
            int bestOligoWeight = graph[index][i][0];
            int currentTempLength = currentDNAlength + newLength + bestOligoWeight;
            if (currentTempLength > n)
                continue;
            if (locationNotFit(currentTempLength - k, locations[i])) {
                continue;
            }
            int bestParentOligoWeight = -1;
            int newOligosNumber = 1;
            if (depth != GREEDY_DEPTH) {
                int bestWeight = findBest(i, depth+1, newLength+bestOligoWeight, newOligosNumber).weight;
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
        prevOligosNumber += maxOligosNumber;
        return best;
    }

    Pair findBestShallow(int index) { // if GREEDY_DEPTH = 1
        Pair best = { -1, -1 };
        int bestOligoWeight;
        int min = k+1;
        for(int i = 0; i < oligosSize; i++)
        {
            bestOligoWeight = graph[index][i][0];
            int currentTempLength = currentDNAlength + bestOligoWeight;
            if (currentTempLength > n)
                continue;
            if (locationNotFit(currentTempLength - k, locations[i])) {
                continue;
            }
            if (bestOligoWeight < min) {
                min = bestOligoWeight;
                best.weight = bestOligoWeight;
                best.index = i;
            }
        }
        return best;
    }
    // lengthening
    Pair findBestTabu(int index) {
        Pair best = { -1, -1 };
        int bestOligoWeight;
        int min = k+1;
        for(int i = 0; i < oligosSize; i++)
        {
            auto itTabu = find(tabuList.begin(), tabuList.end(), i);
            if(itTabu == tabuList.end()) {
                bestOligoWeight = graph[index][i][0];
                int currentTempLength = currentDNAlength + bestOligoWeight;
                if (currentTempLength > n)
                    continue;
                if (locationNotFit(currentTempLength - k, locations[i])) {
                    continue;
                }
                if (bestOligoWeight < min) {
                    min = bestOligoWeight;
                    best.weight = bestOligoWeight;
                    best.index = i;
                }
            }      
        }

        return best;
    }

    vector<Pair>& getResult() {
        return greedyResult;
    }

    int getResultDnaLength() const {
        return currentDNAlength;
    }

    void setClusters(const set<vector<int>> globalClusters) {
        prevClusters.assign(globalClusters.begin(), globalClusters.end());
        // converting indexes to local scope
        for (vector<int>& cluster : prevClusters) {
            for (int& index : cluster) {
                string oligoString = DICTIONARY.oligos[index];
                index = getIndex(oligos, oligoString);
            }
        }
        sort(prevClusters.begin(), prevClusters.end(), [] (const vector<int>& v1, const vector<int>& v2) {
            return (v1.size() > v2.size());
        });

        // calculating cluster locations
        prevClustersLocations.clear();
        for (const vector<int>& cluster : prevClusters) {
            prevClustersLocations.push_back(getClusterPossibleLocation(cluster));
        }
    }

    Location getClusterPossibleLocation(const vector<int>& cluster) {
        int counter = 0;
        const int clusterSize = cluster.size();
        vector<int> lefts(clusterSize);
        vector<int> rights(clusterSize);
        for (const int& index : cluster) {
            lefts[counter] = locations[index].left - counter;
            rights[counter] = locations[index].right - counter;
            ++counter;
        }
        int maxLeft = *max_element(lefts.begin(), lefts.end());
        int minRight = *min_element(rights.begin(), rights.end());
        return { maxLeft, minRight };
    }

    vector<size_t> getTabuListClusters() {
        return tabuListClusters;
    }

};