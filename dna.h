#pragma once

#include <random>
#include <time.h>
#include "additives.h"
#include "printer.h"
#include "locator.h"

class Dna {
private:
    string dna;
    vector<string> oligos;
    vector<int> oligosAsNumbers;
    vector<Location> locations;
    Locator locator;

public:

    Dna(string dnaString) {
        if (dnaString.size() == 0) {
            dna = string(n, 'A');

            // generating a DNA of length n
            for (int i=0; i<n; i++) {
                dna[i] = 'A' + rand() % 4;
                if (dna[i] == 'B')
                    dna[i] = 'G';
                else if (dna[i] == 'D')
                    dna[i] = 'T';
            }
        } else {
            dna = dnaString;
        }

        // generating (m = n - k + 1) oligonucleotides of length k
        int m = n - k + 1;
        // dna = "CCCGA"; // temp
        vector<OligoWithPosition> oligoWithPosition(m);
        for (int i=0; i<m; i++) {
            string oligo = dna.substr(i, k);
            oligoWithPosition[i] = {dna.substr(i, k), i};
        }

        // oligonucleotides mixing - sorting in alphabetical order
        sort(
            oligoWithPosition.begin(),
            oligoWithPosition.end(),
            [](const OligoWithPosition& a, const OligoWithPosition& b) {
                return a.oligo < b.oligo;
            }
        );

        oligos = vector<string>(m);
        oligosAsNumbers = vector<int>(m);
        locations = vector<Location>(m);
        for (int i=0; i<m; i++) {
            oligos[i] = oligoWithPosition[i].oligo;
            oligosAsNumbers[i] = convertOligoToInt(oligos[i]);
            int index = oligoWithPosition[i].index;
            Location loc = locator.getLocation(index, LOCATION_RANDOM_TYPE);

            locations[i] = loc;
        }
    }

    ~Dna() {}

    inline static int convertOligoToInt(const string& oligo) {
        int accum = 0;
        const int base = 4;
        int power = 1;
        for (int i=k-1; i>=0; --i) {
            int digit = 3;
            switch(oligo[i]) {
            case 'A':
                digit = 0;
                break;
            case 'C':
                digit = 1;
                break;
            case 'G':
                digit = 2;
                break;
            }
            accum += power * digit;
            power *= base;
        }
        return accum;
    }

    string getDna() {
        return dna;
    }

    vector<string>& getOligos() {
        return oligos;
    }

    vector<int>& getOligosAsNumbers() {
        return oligosAsNumbers;
    }

    string getFirst() {
        return dna.substr(0, k);
    }

    vector<Location>& getLocations() {
        return locations;
    }

};