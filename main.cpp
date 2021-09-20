#include "manager.h"

/*
struct Instance {
    int repetition;
    int dnaLength;
    int oligoLength;
    string dna;
    int locationRange;
    int greedyDepth;
    Random_Type locationRandomType;
    int tabuListLength;
    int maxTabuIterations;
    int maxTabuIterationsWithNoImprovement;
    int toPrint;
};
*/

int main() {
    
    srand(time(NULL));

    const int toPrint = 0
    | Printer::COUNTER
    // | Printer::INITIALS
    // | Printer::STARTING_DNA
    // | Printer::ORIGINAL_OLIGOS
    // | Printer::ORIGINAL_LOCATIONS
    // | Printer::ORIGINAL_OLIGOS_WITH_LOCATIONS
    // | Printer::WITH_ERRORS_OLIGOS
    // | Printer::WITH_ERRORS_LOCATIONS
    // | Printer::WITH_ERRORS_OLIGOS_WITH_LOCATIONS
    // | Printer::GRAPH
    | Printer::RESULTS_WITHOUT_DNA_STRING
    | Printer::RESULTS_GREEDY
    | Printer::RESULTS_AFTER_COMPACTION
    | Printer::RESULTS_AFTER_LENGTHENING
    | Printer::RESULTS_FINAL;

    Instance instance1(
        1,      // repetition
        800,    // dna length
        8,      // oligonucleotide length
        "",     // dna string, put "" to generate random one
        1,      // oligonucleotides location range
        2,      // greedy calculation depth
        Random_Type::GAUSSIAN,  // random type for generating random location interval for oligonucleotides
        10,     // tabu list length
        5,      // tabu search max iterations
        2,      // tabu search max iterations with no improvement, put -1 to deactivate this parameter
        toPrint // integer for printing
    );

    Manager manager({
        instance1,
    });

    manager.runAll();

    return 0;
}