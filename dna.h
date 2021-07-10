#include "additives.h"

#pragma once

class Dna {
private:
    string dna;
    vector<string> oligos;
public:

    Dna() {
        init();
    }

    ~Dna() {}

    static bool strCompare(string a, string b) {
        return (a < b);
    }

    void init() {
        dna = string(n, 'A');
        srand(time(NULL));

        // generowanie dna o długości n
        for (int i=0; i<n; i++) {
            dna[i] = 'A' + rand() % 4;
            if (dna[i] == 'B')
                dna[i] = 'T';
            else if (dna[i] == 'D')
                dna[i] = 'G';
        }

        // generowanie (m = n - k + 1) oligonukleotydów o długości k
        int m = n - k + 1;
        // dna = "CCCGA"; // temp
        oligos = vector<string>(m, string(k, 'A'));
        for (int i=0; i<m; i++) {
            oligos[i] = dna.substr(i, k);
        }

        // oligonukleotydy przed posortowaniem
        // for (int i=0; i<m; i++)
        //     std::cout << "i=" << i << ": " << oligos[i] << std::endl;
        // std::cout << dna << std::endl;

        // mieszanie oligonukleotydów - sortowanie w kolejności alfabetycznej
        sort(oligos.begin(), oligos.end(), strCompare);

        // oligonukleotydy po posortowaniu
        // for (int i=0; i<m; i++)
        //     std::cout << "i=" << i << ": " << oligos[i] << std::endl;
        // std::cout << dna << std::endl;

        // (niepotrzebne) sprawdzenie czy jest po kolei
        // int err = 0;
        // for (int i=0; i<m-1; i++) {
        //     if (oligos[i].compare(oligos[i+1]) > 0) {
        //         std::cout << "error: " << i << " - " << i+1 << std::endl;
        //         err++;
        //     }
        // }
        // std::cout << "Total errors: " << err << std::endl;
    }

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