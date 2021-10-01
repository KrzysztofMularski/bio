#pragma once

#include "manager.h"
#include "printer.h"
#include "printPatterns.h"
#include <vector>
#include <string>

using namespace std;

// Instance instance(
//     3,      // repetition of instance
//     800,    // dna length
//     8,      // oligonucleotide length
//     "TGCAGGCAAAGGGAACTAGGTTTTGCAAACTATGAGCCGCCCAACACGATCTCGCTACAAATAAAGGCTCGCGGCTGAGGTTAGGCGGGGGGACGATCCCGTACACGGCCCGCCTTGAACGGTGTGGCTTGATGCTAAGCCTAACTTTTTGAGAGAGCTGGAGAGAAGAGACAATCTCGCCCCTTTTAGCGCCTTATTGTAGGAGATATACGTAACTTCACATATCACGCGGTCGGTAGCGATAATCACAAAATGTGGGCTCTTGGAAGGCCGGCCAACCGCTGCAGTGCCGGGACCACAGACTGGGGGTTCGCATCGTATGGACGCGGAGCATGGACCCGCATAACTGGTGCAGGGACCATAGCCCTAGGCTGGTCTCTGGATGCACCCACGGTATCTTTAGCGCGTCATTGTCTGTGTGCCTTGTCTCGCCTATGCTTCCCTGGCTAACGCAAGCTTGGAATTATGCCAAATAGACAACTGATCGGGATTGTCATGGCTCCTGCGGGGGTACAAAGGCCAGTTTTTGGGATTAACGGTCCCTACTGAATCGAGATATAGAGAAGACCGGATTTACCGCTAAAAGATGAATAGTACACAGTAGTAGGCCTGCTGCACGGCGACCATAAGAGGGCAGTGTGAGTTCGTATACAGGTGATGGACCGGCCATGCTCGCGTGGCCACTGGAGGGCGTGCATAGTGTCAAGGTCCGACATTGTACATTAAGATGGCTAGTGCAAACCGGCAATAGCAAGAATGTACCTTTCAGCAGGCCCATCATTCTTTATCGTTTGCCAGCG",     // dna string, put "" to generate random one
//     10.0,    // [0-100%] part of spectrum that represents positive errors
//     10.0,    // [0-100%] part of spectrum that represents negative errors
//     300,      // oligonucleotides location range
//     1,      // greedy calculation depth
//     Random_Type::LINEAR,  // random type for generating random location interval for oligonucleotides
//     20,     // tabu list length
//     3,     // tabu list for clusters length
//     10,      // tabu search max iterations
//     5,      // tabu search max iterations with no improvement, put -1 to deactivate this parameter
//     12,      // global max iterations
//     3,      // cluster overlap criterion - possible values: [0, k-2];
//     toPrint, // integer for printing (toPrint is set as global variable in printPatterns.h)
//     "results.txt",   // saving results to file with specified filename, put "" to not
//     "resultsCompact.csv" // saving compact data to file with specified filename, put "" to not
// );

const string DEFAULT_FILENAME = "result.txt";
const string DEFAULT_COMPACT_FILENAME = "resultsCompact.csv";

vector<Instance> instances = {
    { 3, 800, 8, "", 10.0, 10.0, 300, 1, Random_Type::LINEAR, 20, 3, 10, 5, 12, 3, toPrint, DEFAULT_FILENAME, DEFAULT_COMPACT_FILENAME },
    { 3, 800, 8, "", 10.0, 10.0, 300, 1, Random_Type::LINEAR, 20, 3, 10, 5, 12, 3, toPrint, DEFAULT_FILENAME, DEFAULT_COMPACT_FILENAME },
};