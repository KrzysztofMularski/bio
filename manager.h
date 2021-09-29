#pragma once

#include "dna.h"
#include "dnaStructure.h"
#include "greedy.h"
#include "tabu.h"
#include "finals.h"
#include "printer.h"
#include "additives.h"

struct Instance {
    int repetition;
    int dnaLength;
    int oligoLength;
    string dna;
    float positiveErrorPercentage;
    float negativeErrorPercentage;
    int locationRange;
    int greedyDepth;
    Random_Type locationRandomType; // Linear or Gaussian
    int tabuListLength;
    int maxTabuIterations;
    int maxTabuIterationsWithNoImprovement;
    int globalMaxIterations;
    int clusterOverlapCriterion;
    int toPrint;

    Instance(
        int repetition,
        int dnaLength,
        int oligoLength,
        string dna,
        float positiveErrorPercentage,
        float negativeErrorPercentage,
        int locationRange,
        int greedyDepth,
        Random_Type locationRandomType,
        int tabuListLength,
        int maxTabuIterations,
        int maxTabuIterationsWithNoImprovement,
        int globalMaxIterations,
        int clusterOverlapCriterion,
        int toPrint
        ) :
        repetition(repetition),
        dnaLength(dnaLength),
        oligoLength(oligoLength),
        dna(dna),
        positiveErrorPercentage(positiveErrorPercentage),
        negativeErrorPercentage(negativeErrorPercentage),
        locationRange(locationRange),
        greedyDepth(greedyDepth),
        locationRandomType(locationRandomType),
        tabuListLength(tabuListLength),
        maxTabuIterations(maxTabuIterations),
        maxTabuIterationsWithNoImprovement(maxTabuIterationsWithNoImprovement),
        globalMaxIterations(globalMaxIterations),
        clusterOverlapCriterion(clusterOverlapCriterion),
        toPrint(toPrint) {
            int& coc = clusterOverlapCriterion;
            if (coc < 0) {
                coc = 0;
            } else if (coc > k-2) {
                coc = k-2;
            }
        }
    
};

class Manager {
private:
    vector<Instance> instances;
public:
    Manager(vector<Instance> instances) : instances(instances) {}
    ~Manager() {}

    void runAll() {
        for (auto instance : instances) {
            for (int i=0; i<instance.repetition; i++) {
                instance.toPrint & Printer::COUNTER && Printer::printCounter(INSTANCE_COUNTER);
                run(instance);
                INSTANCE_COUNTER++;
            }
        }
    }

    void run(Instance i) {

        n = i.dnaLength;
        k = i.oligoLength;
        POSITIVE_ERRORS_PERCENTAGE = i.positiveErrorPercentage;
        NEGATIVE_ERRORS_PERCENTAGE = i.negativeErrorPercentage;
        GREEDY_DEPTH = i.greedyDepth;
        LOCATION_RANGE = i.locationRange;
        LOCATION_RANDOM_TYPE = i.locationRandomType;
        TABU_LIST_LENGTH = i.tabuListLength;
        MAX_TABU_ITERATIONS = i.maxTabuIterations;
        MAX_TABU_ITERATIONS_WITH_NO_IMPROVEMENT = i.maxTabuIterationsWithNoImprovement;
        GLOBAL_MAX_ITERATIONS = i.globalMaxIterations;
        CLUSTER_OVERLAP_CRITERION = i.clusterOverlapCriterion;
        TO_PRINT = i.toPrint;

        TO_PRINT & Printer::INITIALS && Printer::printInitials();

        Dna dna(i.dna);

        string dnaStr = dna.getDna();
        vector<string>& oligos = dna.getOligos();
        vector<int>& oligosAsNumbers = dna.getOligosAsNumbers();
        string firstOligo = dna.getFirst();
        vector<Location>& locations = dna.getLocations();
        
        TO_PRINT & Printer::STARTING_DNA && Printer::printDNA(dnaStr);
        TO_PRINT & Printer::ORIGINAL_OLIGOS && Printer::printOligos("Original oligonucleotides", oligos);
        TO_PRINT & Printer::ORIGINAL_LOCATIONS && Printer::printLocations("Original oligonucleotides locations", locations);
        TO_PRINT & Printer::ORIGINAL_OLIGOS_WITH_LOCATIONS && Printer::printOligosWithLocations("Original oligonucleotides with locations", oligos, locations);

        DnaStructure structure(oligos, oligosAsNumbers, locations);
        structure.generateErrors();

        TO_PRINT & Printer::WITH_ERRORS_OLIGOS && Printer::printOligos("Oligonucleotides with errors", oligos);
        TO_PRINT & Printer::WITH_ERRORS_LOCATIONS && Printer::printLocations("Oligonucleotides with errors locations", locations);
        TO_PRINT & Printer::WITH_ERRORS_OLIGOS_WITH_LOCATIONS && Printer::printOligosWithLocations("Oligonucleotides with errors with locations", oligos, locations);
        
        structure.generateGraph();

        structure.setDictionary();

        set<vector<int>> globalClusters;

        vector<Pair> bestResult;
        int bestResultSize = 0;
        vector<string> bestOligosVersion;

        for (int i=0; i<GLOBAL_MAX_ITERATIONS; ++i) {
            // oligos and locations pairs permutation
            if (i == 0) {
                // oligos and locations pairs sorted alphabetically (default)
            } else if (i == 1) {
                // oligos and locations pairs sorted alphabetically in reverse order
                structure.reversePairs();
            } else {
                // oligos and locations pairs randomly mixed up
                structure.mixPairs();
            }

            structure.populateGraph();

            TO_PRINT & Printer::GRAPH && Printer::printGraph(structure.getGraph(), structure.getOligosSize());

            vector<Pair> result;
            vector<int> tabuList;

            Greedy greedy(structure, firstOligo, result, tabuList, Greedy::TYPE_GREEDY);
            if (i > 1)
                greedy.setClusters(globalClusters);
            greedy.calculateResult();
            int resultDnaLength = greedy.getResultDnaLength();

            if (result.size() > bestResultSize) {
                bestResult = result;
                bestResultSize = result.size();
                bestOligosVersion = oligos;
            }
            if (i == 0) // temp
                TO_PRINT & Printer::RESULTS_GREEDY && Printer::printResults("Greedy result", result, oligos, dnaStr);

            Tabu tabu(dnaStr, resultDnaLength, oligos, locations, result, structure.getGraph(), tabuList);
            tabu.startSearch();
            vector<vector<int>> tabuClusters;
            tabu.getClusters(tabuClusters);
            addNewClusters(tabuClusters, globalClusters);

            if (result.size() > bestResultSize) {
                bestResult = result;
                bestResultSize = result.size();
                bestOligosVersion = oligos;
            }
            
            TO_PRINT & Printer::RESULTS_FINAL && Printer::printResults("Final result", tabu.getResult(), oligos, dnaStr);
        }

        TO_PRINT & Printer::RESULTS_GLOBAL_FINAL && Printer::printResults("Final global result", bestResult, bestOligosVersion, dnaStr);

        Printer::printEnd();
    }
};