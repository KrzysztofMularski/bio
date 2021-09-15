#include "dna.h"
#include "dnaStructure.h"
#include "greedy.h"
#include "tabu.h"
#include "finals.h"
#include "printer.h"
#include "additives.h"

#pragma once

struct Instance {
    int repetition;

    int dnaLength;
    int oligoLength;
    string dna;
    int locationRange;
    int greedyDepth;
    int locationRandomType; // Linear or Gaussian
    int tabuListLength;

    int toPrint;

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

        // setting up globals
        n = i.dnaLength;
        k = i.oligoLength;
        GREEDY_DEPTH = i.greedyDepth;
        LOCATION_RANGE = i.locationRange;
        LOCATION_RANDOM_TYPE = i.locationRandomType;
        TABU_LIST_LENGTH = i.tabuListLength;
        TO_PRINT = i.toPrint;

        TO_PRINT & Printer::INITIALS && Printer::printInitials();

        Dna dna(i.dna);

        string dnaStr = dna.getDna();
        vector<string> oligos = dna.getOligos();
        string firstOligo = dna.getFirst();
        
        TO_PRINT & Printer::STARTING_DNA && Printer::printDNA(dnaStr);
        TO_PRINT & Printer::ORIGINAL_OLIGOS && Printer::printOligos("Original oligonucleotides", oligos);

        DnaStructure structure(oligos);
        structure.generateErrors();
        oligos = structure.getOligos();
        TO_PRINT & Printer::OLIGOS_WITH_ERRORS && Printer::printOligos("Oligonucleotides with errors", oligos);
        structure.generateGraph();
        structure.populateGraph();

        TO_PRINT & Printer::GRAPH && Printer::printGraph(structure);

        vector<int> visited;
        vector<Pair> result;
        vector<string> greedyResultOligos;
        vector<int> tabuList;

        Greedy greedy(structure, firstOligo, visited, result, greedyResultOligos, tabuList, Greedy::TYPE_GREEDY);
        greedy.calculateResult();
        // vector<Pair> result = greedy.getResult();

        int resultDnaLength = greedy.getResultDnaLength();
        TO_PRINT & Printer::RESULTS_GREEDY && Printer::printResults("Greedy result", result, oligos, dnaStr);

        Tabu tabu(dnaStr, resultDnaLength, greedy.getResultOligos(), oligos, result, structure.getGraph(), greedy.getVisited(), tabuList, greedyResultOligos);
        tabu.startSearch();

        TO_PRINT & Printer::RESULTS_FINAL && Printer::printResults("Final result", tabu.getResult(), oligos, dnaStr);

    }

};