#include <random>
#include <time.h>
#include "additives.h"
#include "printer.h"
#include "locator.h"

#pragma once

class Dna {
private:
    string dna;
    vector<string> oligos;
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
                    dna[i] = 'T';
                else if (dna[i] == 'D')
                    dna[i] = 'G';
            }
        } else {
            dna = dnaString;
        }

        // generating (m = n - k + 1) oligonucleotides of length k
        int m = n - k + 1;
        // dna = "CCCGA"; // temp
        vector<OligosWithLocation> oligosWithLocation(m);
        for (int i=0; i<m; i++) {
            oligosWithLocation[i] = {dna.substr(i, k), i};
        }


        // oligos = vector<string>(m);
        // for (int i=0; i<m; i++) {
        //     oligos[i] = dna.substr(i, k);
        // }

        // oligonucleotides before sorting
        // print("Oligonucleotides before sorting:");
        // printOligos(oligos);
        // printDNA(dna);

        // oligonucleotides mixing - sorting in alphabetical order
        sort(
            oligosWithLocation.begin(),
            oligosWithLocation.end(),
            [](const OligosWithLocation& a, const OligosWithLocation& b) {
                return a.oligo < b.oligo;
            }
        );

        // sort(oligos.begin(), oligos.end(), [](const string& a, const string& b) {return a < b;});

        oligos = vector<string>(m);
        locations = vector<Location>(m);
        for (int i=0; i<m; i++) {
            oligos[i] = oligosWithLocation[i].oligo;
            
            int index = oligosWithLocation[i].index;
            Location loc = locator.getLocation(index, LOCATION_RANDOM_TYPE == 1 ? Locator::GAUSSIAN : Locator::LINEAR);

            locations[i] = loc;
        }

        oligos = {"AACC", "AACT", "ACGT", "CCTG", "CTGG", "CGTA","GTAA"};
        
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

    vector<Location> getLocations() {
        return locations;
    }

};