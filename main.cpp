#include "manager.h"
#include "printPatterns.h"

int main() {
    
    srand(1);

    Instance instance1(
        1,      // repetition of instance
        800,    // dna length
        8,      // oligonucleotide length
        "TGCAGGCAAAGGGAACTAGGTTTTGCAAACTATGAGCCGCCCAACACGATCTCGCTACAAATAAAGGCTCGCGGCTGAGGTTAGGCGGGGGGACGATCCCGTACACGGCCCGCCTTGAACGGTGTGGCTTGATGCTAAGCCTAACTTTTTGAGAGAGCTGGAGAGAAGAGACAATCTCGCCCCTTTTAGCGCCTTATTGTAGGAGATATACGTAACTTCACATATCACGCGGTCGGTAGCGATAATCACAAAATGTGGGCTCTTGGAAGGCCGGCCAACCGCTGCAGTGCCGGGACCACAGACTGGGGGTTCGCATCGTATGGACGCGGAGCATGGACCCGCATAACTGGTGCAGGGACCATAGCCCTAGGCTGGTCTCTGGATGCACCCACGGTATCTTTAGCGCGTCATTGTCTGTGTGCCTTGTCTCGCCTATGCTTCCCTGGCTAACGCAAGCTTGGAATTATGCCAAATAGACAACTGATCGGGATTGTCATGGCTCCTGCGGGGGTACAAAGGCCAGTTTTTGGGATTAACGGTCCCTACTGAATCGAGATATAGAGAAGACCGGATTTACCGCTAAAAGATGAATAGTACACAGTAGTAGGCCTGCTGCACGGCGACCATAAGAGGGCAGTGTGAGTTCGTATACAGGTGATGGACCGGCCATGCTCGCGTGGCCACTGGAGGGCGTGCATAGTGTCAAGGTCCGACATTGTACATTAAGATGGCTAGTGCAAACCGGCAATAGCAAGAATGTACCTTTCAGCAGGCCCATCATTCTTTATCGTTTGCCAGCG",     // dna string, put "" to generate random one
        10.0,    // [0-100%] part of spectrum that represents positive errors
        10.0,    // [0-100%] part of spectrum that represents negative errors
        400,      // oligonucleotides location range
        1,      // greedy calculation depth
        Random_Type::LINEAR,  // random type for generating random location interval for oligonucleotides
        20,     // tabu list length
        100,      // tabu search max iterations
        -1,      // tabu search max iterations with no improvement, put -1 to deactivate this parameter
        20,      // global max iterations
        toPrint // integer for printing (toPrint is set as global variable in printPatterns.h)
    );

    Manager manager({
        instance1,
    });

    manager.runAll();

    return 0;
}