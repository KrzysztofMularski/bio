#include "manager.h"
#include "printPatterns.h"

int main() {
    
    srand(time(NULL));

    Instance instance1(
        1,      // repetition of instance
        800,    // dna length
        8,      // oligonucleotide length
        "",     // dna string, put "" to generate random one
        0.0,    // [0-100%] part of spectrum that represents negative errors
        0.0,    // [0-100%] part of spectrum that represents positive errors
        100,      // oligonucleotides location range
        1,      // greedy calculation depth
        Random_Type::GAUSSIAN,  // random type for generating random location interval for oligonucleotides
        10,     // tabu list length
        1,      // tabu search max iterations
        -1,      // tabu search max iterations with no improvement, put -1 to deactivate this parameter
        toPrint // integer for printing (toPrint is set as global variable in printPatterns.h)
    );

    Manager manager({
        instance1,
    });

    manager.runAll();

    return 0;
}