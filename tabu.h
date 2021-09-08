#include "additives.h"

#pragma once

class Tabu{
    Dna dna;
    vector<string> oligos;
    // DnaStructure structure;
public:
    Tabu(Dna dna, vector<string> oligos): dna(dna), oligos(oligos) { }

    void test() {
        // printOligos(structure.getOligos());
    }
    

    ~Tabu() {}
};