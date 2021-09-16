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
    int toPrint;
};
*/

int main() {
    
    srand(time(NULL));

    const int toPrint = 0
    | Printer::COUNTER
    // | Printer::INITIALS
    | Printer::STARTING_DNA
    // | Printer::ORIGINAL_OLIGOS
    // | Printer::ORIGINAL_LOCATIONS
    // | Printer::ORIGINAL_OLIGOS_WITH_LOCATIONS
    // | Printer::WITH_ERRORS_OLIGOS
    // | Printer::WITH_ERRORS_LOCATIONS
    // | Printer::WITH_ERRORS_OLIGOS_WITH_LOCATIONS
    // | Printer::GRAPH
    | Printer::RESULTS_GREEDY
    | Printer::RESULTS_AFTER_COMPACTION
    | Printer::RESULTS_AFTER_LENGTHENING
    | Printer::RESULTS_FINAL;

    // Instance instance1(1, 10, 4, "ACGTAACCTG", 6, 2, Random_Type::GAUSSIAN, 10, toPrint);
    Instance instance1(1, 20, 3, "CGACGGACTCTGAAAACCTG", 20, 2, Random_Type::GAUSSIAN, 10, toPrint);

    Manager manager({
        instance1,
    });

    manager.runAll();

    return 0;
}