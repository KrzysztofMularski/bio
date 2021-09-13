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
};
*/

int main() {
    
    srand(time(NULL));

    const int toPrint = 0
    | Printer::COUNTER
    | Printer::INITIALS
    | Printer::STARTING_DNA
    | Printer::ORIGINAL_OLIGOS
    | Printer::OLIGOS_WITH_ERRORS
    | Printer::GRAPH
    | Printer::RESULTS_GREEDY
    | Printer::RESULTS_AFTER_COMPACTION
    | Printer::RESULTS_AFTER_LENGTHENING
    | Printer::RESULTS_FINAL;

    Manager manager({
        // { 1, 10, 3, "GTATTAGAAC", 50, 1, 1, 10, toPrint}
        { 2, 10, 4, "ACGTAACTGG", 50, 2, 1, 10, toPrint}
        //{ 1, 10, 3, "", 50, 1, 1, 10, toPrint }
    });

    manager.runAll();

    return 0;
}