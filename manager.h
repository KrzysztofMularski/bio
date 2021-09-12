#include "dna.h"
#include "dnaStructure.h"
#include "greedy.h"
#include "tabu.h"
#include "finals.h"
#include "printers.h"
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
                printCounter(INSTANCE_COUNTER);
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

        // printInitials();

        Dna dna(i.dna);

        string dnaStr = dna.getDna();
        vector<string> oligos = dna.getOligos();
        string firstOligo = dna.getFirst();
        
        printDNA(dnaStr);
        // printOligos(oligos);

        DnaStructure structure(oligos);
        structure.generateErrors();
        oligos = structure.getOligos();
        // printOligos(oligos);
        structure.generateGraph();
        structure.populateGraph();

        // printGraph(structure);

        Greedy greedy(structure, firstOligo);
        greedy.calculateResult();
        vector<Pair> result = greedy.getResult();

        // Tabu tabu(dna, oligos);

        //string resultDNA = makeDNA(result, oligos);
        string resultDNA = "ACGTAACTGG";

        printResultDNA(resultDNA);
        printDistance(levenshteinDistance(dnaStr, resultDNA));

        Tabu tabu(resultDNA, greedy.getResultOligos(), oligos, result, structure.getGraph());
        tabu.compaction();

    }

};