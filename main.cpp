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

    Manager manager({
        // { 1, 10, 3, "GTATTAGAAC", 50, 1, 1, 10}
        { 1, 10, 4, "ACGTAACTGG", 50, 2, 1, 10}
        //{ 1, 10, 3, "", 50, 1, 1 }
    });

    manager.runAll();

    return 0;
}