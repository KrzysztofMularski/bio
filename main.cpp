#include "manager.h"

/*
struct Instance {
    int repetition;
    int dnaLength;
    int oligoLength;
    string dna;
    int locationRange;
    int greedyDepth;
    int locationRandomType; // 0: Linear or 1: Gaussian
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
    | Printer::ORIGINAL_OLIGOS
    | Printer::OLIGOS_WITH_ERRORS
    | Printer::GRAPH
    | Printer::RESULTS_GREEDY
    | Printer::RESULTS_AFTER_COMPACTION
    | Printer::RESULTS_AFTER_LENGTHENING
    | Printer::RESULTS_FINAL;

    Instance instance1(1, 10, 4, "ACGTAACCTG", 50, 2, Random_Type::GAUSSIAN, 10, toPrint);
    // Instance instance2(1, 10, 4, "ACGTAACCTG", 50, 2, Locator::GAUSSIAN, 10, toPrint);

    Manager manager({
        instance1,
        // instance2
    });

    manager.runAll();

    return 0;
}