#include "additives.h"
#include "printers.h"

#pragma once

class Dna {
private:
    string dna;
    vector<string> oligos;
public:

    Dna() {
        dna = string(n, 'A');
        srand(time(NULL));

        // generating a DNA of length n
        for (int i=0; i<n; i++) {
            dna[i] = 'A' + rand() % 4;
            if (dna[i] == 'B')
                dna[i] = 'T';
            else if (dna[i] == 'D')
                dna[i] = 'G';
        }

        // generating (m = n - k + 1) oligonucleotides of length k
        int m = n - k + 1;
        // dna = "CCCGA"; // temp
        oligos = vector<string>(m);
        for (int i=0; i<m; i++) {
            oligos[i] = dna.substr(i, k);
        }

        // oligonucleotides before sorting
        // print("Oligonucleotides before sorting:");
        // printOligos(oligos);
        // printDNA(dna);

        // oligonucleotides mixing - sorting in alphabetical order
        sort(oligos.begin(), oligos.end(), [](const string& a, const string& b) {return a < b;});

        // oligonucleotides after sorting
        // print("Oligonucleotides after sorting:");
        // printOligos(oligos);
        // printDNA(dna);

        // (unnecessary) checking if it is in order
        // printErrors(oligos);
    }

    ~Dna() {}

    string getDna() {
        return dna;
    }

    vector<string> getOligos() {
        return oligos;
    }

    string getFirst() {
        return dna.substr(0, k);
    }
};