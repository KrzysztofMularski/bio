#include "additives.h"

#pragma once

class tabu{
    Dna dna;
    vector<string> oligos;
    DnaStructure structure(vector<string> oligos);
public:
    tabu(Dna dna, vector<string> oligos): dna(dna), oligos(oligos) {
        structure(oligos);
    }

    

    ~tabu() {}
};