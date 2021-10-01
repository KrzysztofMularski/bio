#pragma once

#include "dna.h"
#include "dnaStructure.h"
#include "greedy.h"
#include "tabu.h"
#include "finals.h"
#include "printer.h"
#include "additives.h"
#include "fileManager.h"

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
    int tabuListClustersLength;
    int maxTabuIterations;
    int maxTabuIterationsWithNoImprovement;
    int globalMaxIterations;
    int clusterOverlapCriterion;
    int toPrint;
    string filename;
    string compactFilename;

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
        int tabuListClustersLength,
        int maxTabuIterations,
        int maxTabuIterationsWithNoImprovement,
        int globalMaxIterations,
        int clusterOverlapCriterion,
        int toPrint,
        string filename,
        string compactFilename
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
        tabuListClustersLength(tabuListClustersLength),
        maxTabuIterations(maxTabuIterations),
        maxTabuIterationsWithNoImprovement(maxTabuIterationsWithNoImprovement),
        globalMaxIterations(globalMaxIterations),
        clusterOverlapCriterion(clusterOverlapCriterion),
        toPrint(toPrint),
        filename(filename),
        compactFilename(compactFilename) {
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
    int instancesSize;
    int maxCyclesNumber;
public:
    Manager(vector<Instance> instances) : instances(instances) {
        maxCyclesNumber = 0;
        instancesSize = 0;
        for (auto& instance : instances) {
            maxCyclesNumber += instance.repetition * instance.globalMaxIterations;
            instancesSize += instance.repetition;
        }
    }
    ~Manager() {}

    void runAll() {
        
        for (auto& instance : instances) {
            for (int i=0; i<instance.repetition; i++) {
                instance.toPrint & Printer::COUNTER && Printer::printCounter(INSTANCE_COUNTER);
                run(instance);
                INSTANCE_COUNTER++;
            }
        }
    }

    void run(Instance ins) {

        clock_t start = clock();

        n = ins.dnaLength;
        k = ins.oligoLength;
        POSITIVE_ERRORS_PERCENTAGE = ins.positiveErrorPercentage;
        NEGATIVE_ERRORS_PERCENTAGE = ins.negativeErrorPercentage;
        LOCATION_RANGE = ins.locationRange;
        GREEDY_DEPTH = ins.greedyDepth;
        LOCATION_RANDOM_TYPE = ins.locationRandomType;
        TABU_LIST_LENGTH = ins.tabuListLength;
        TABU_LIST_CLUSTERS_LENGTH = ins.tabuListClustersLength;
        MAX_TABU_ITERATIONS = ins.maxTabuIterations;
        MAX_TABU_ITERATIONS_WITH_NO_IMPROVEMENT = ins.maxTabuIterationsWithNoImprovement;
        GLOBAL_MAX_ITERATIONS = ins.globalMaxIterations;
        CLUSTER_OVERLAP_CRITERION = ins.clusterOverlapCriterion;
        TO_PRINT = ins.toPrint;

        clock_t startFile = clock();

        FileManager file(ins.filename);
        file.write(INSTANCE_COUNTER);
        file.write(ins);
        FileManager fileCompact(ins.compactFilename);
        if (INSTANCE_COUNTER == 1)
            fileCompact.writeHeaders();
        fileCompact.writeCompact(INSTANCE_COUNTER);
        fileCompact.writeCompact(ins);

        clock_t endFile = clock();
        clock_t fileWritingDelay = endFile - startFile;

        TO_PRINT & Printer::INITIALS && Printer::printInitials();

        Dna dna(ins.dna);

        string dnaStr = dna.getDna();

        startFile = clock();
        file.write(dnaStr, !ins.dna.size());
        fileCompact.writeCompact(dnaStr, !ins.dna.size());
        endFile = clock();
        fileWritingDelay += endFile - startFile;

        vector<string>& oligos = dna.getOligos();
        vector<int>& oligosAsNumbers = dna.getOligosAsNumbers();
        string firstOligo = dna.getFirst();
        vector<Location>& locations = dna.getLocations();
        
        TO_PRINT & Printer::STARTING_DNA && Printer::printDNA(dnaStr);
        TO_PRINT & Printer::ORIGINAL_OLIGOS && Printer::printOligos("Original oligonucleotides", oligos);
        TO_PRINT & Printer::ORIGINAL_LOCATIONS && Printer::printLocations("Original oligonucleotides locations", locations);
        TO_PRINT & Printer::ORIGINAL_OLIGOS_WITH_LOCATIONS && Printer::printOligosWithLocations("Original oligonucleotides with locations", oligos, locations);

        DnaStructure structure(oligos, oligosAsNumbers, locations, firstOligo);
        structure.generateErrors();

        TO_PRINT & Printer::WITH_ERRORS_OLIGOS && Printer::printOligos("Oligonucleotides with errors", oligos);
        TO_PRINT & Printer::WITH_ERRORS_LOCATIONS && Printer::printLocations("Oligonucleotides with errors locations", locations);
        TO_PRINT & Printer::WITH_ERRORS_OLIGOS_WITH_LOCATIONS && Printer::printOligosWithLocations("Oligonucleotides with errors with locations", oligos, locations);
        
        structure.generateGraph();

        structure.setDictionary();

        set<vector<int>> globalClusters;

        vector<Pair> bestResult;
        float bestEvaluation = 0;
        vector<string> bestOligosVersion;

        vector<size_t> tabuListClusters;

        for (int i=0; i<GLOBAL_MAX_ITERATIONS; ++i) {

            TO_PRINT & Printer::LOADING && Printer::printLoading(instancesSize, maxCyclesNumber);

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

            Greedy greedy(structure, firstOligo, result, tabuList, tabuListClusters, Greedy::TYPE_GREEDY);
            if (i > 1)
                greedy.setClusters(globalClusters);
            greedy.calculateResult();

            int resultDnaLength = greedy.getResultDnaLength();

            float currentEvaluation = (float)result.size() / (float)makeDNA(result, oligos).size();

            if (currentEvaluation > bestEvaluation) {
                bestResult = result;
                bestEvaluation = currentEvaluation;
                bestOligosVersion = oligos;
            }
            tabuListClusters = greedy.getTabuListClusters();
            
            TO_PRINT & Printer::RESULTS_GREEDY && Printer::printResults("Greedy result", result, oligos, dnaStr);

            if (i == 0) {
                startFile = clock();
                file.write("First result (first greedy)", result, oligos, dnaStr);
                fileCompact.writeCompact(result, oligos, dnaStr);
                endFile = clock();
                fileWritingDelay += endFile - startFile;
            }

            Tabu tabu(dnaStr, resultDnaLength, oligos, locations, result, structure.getGraph(), tabuList);
            tabu.startSearch(bestEvaluation, bestResult, bestOligosVersion);
            vector<vector<int>> tabuClusters;
            tabu.getClusters(tabuClusters);
            addNewClusters(tabuClusters, globalClusters);

            currentEvaluation = (float)result.size() / (float)makeDNA(result, oligos).size();

            if (currentEvaluation > bestEvaluation) {
                bestResult = result;
                bestEvaluation = currentEvaluation;
                bestOligosVersion = oligos;
            }
            
            TO_PRINT & Printer::RESULTS_FINAL && Printer::printResults("Final result", tabu.getResult(), oligos, dnaStr);
            
            ++CURRENT_CYCLES_NUMBER;
        }

        TO_PRINT & Printer::LOADING && Printer::printLoading(instancesSize, maxCyclesNumber, true);

        TO_PRINT & Printer::RESULTS_GLOBAL_FINAL && Printer::printResults("Final global result", bestResult, bestOligosVersion, dnaStr);

        clock_t end = clock();

        startFile = clock();
        file.write("Final result (best)", bestResult, bestOligosVersion, dnaStr);
        fileCompact.writeCompact(bestResult, bestOligosVersion, dnaStr);
        endFile = clock();
        fileWritingDelay += endFile - startFile;

        clock_t finalTime = end - start - fileWritingDelay;

        file.write(finalTime);
        fileCompact.writeCompact(finalTime);

        TO_PRINT & Printer::END_TIME && Printer::printEnd(finalTime);
    }
};