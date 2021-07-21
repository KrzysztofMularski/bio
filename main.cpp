#include "dna.h"
#include "dnaStructure.h"
#include "greedy.h"
#include "tabu.h"
#include "finals.h"
#include "printers.h"
#include "additives.h"

int main() {

    printInitials();

    Dna dna;

    string dnaStr = dna.getDna();
    vector<string> oligos = dna.getOligos();
    string firstOligo = dna.getFirst();
    
    printDNA(dnaStr);
    // printOligos(oligos);

    DnaStructure structure(oligos);
    structure.generateErrors();
    // printOligos(oligos);
    structure.generateGraph();
    structure.populateGraph();    

    // printGraph(structure);

    Greedy greedy(&structure, firstOligo);
    greedy.calculateResult();
    vector<Pair> result = greedy.getResult();

    string resultDNA = makeDNA(result, oligos);

    printResultDNA(resultDNA);
    printDistance(levenshteinDistance(dnaStr, resultDNA));

    return 0;
}