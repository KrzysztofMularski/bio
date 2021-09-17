#include <iomanip>
#include "additives.h"
#include "dnaStructure.h"
#include "finals.h"

#pragma once

class Printer {
private:
    Printer() {}
public:

    static const int COUNTER = 1;
    static const int INITIALS = 2;
    static const int STARTING_DNA = 4;
    static const int ORIGINAL_OLIGOS = 8;
    static const int ORIGINAL_LOCATIONS = 16;
    static const int ORIGINAL_OLIGOS_WITH_LOCATIONS = 32;
    static const int WITH_ERRORS_OLIGOS = 64;
    static const int WITH_ERRORS_LOCATIONS = 128;
    static const int WITH_ERRORS_OLIGOS_WITH_LOCATIONS = 256;
    static const int GRAPH = 512;
    static const int RESULTS_WITHOUT_DNA_STRING = 1024;
    static const int RESULTS_GREEDY = 2048;
    static const int RESULTS_AFTER_COMPACTION = 4096;
    static const int RESULTS_AFTER_LENGTHENING = 8192;
    static const int RESULTS_FINAL = 16384;

    static int printCounter(const int& counter) {
        if (counter == 1) {
            cout << endl;
        }
        cout << "=============== Instance " << counter << " ===============" << endl;
    }

    static int printInitials() {
        cout << "\n" << setw(2) << left << n << " - DNA length" << endl;
        cout << setw(2) << left << k << " - Oligonucleotide length" << endl;
    }

    static int print(const char* arr) {
        cout << "\n" << arr << endl;
    }

    static int printDNA(const string& dna) {
        cout << "\nDNA:\n  " << dna << endl;
    }

    static int printOligos(const string& header, const vector<string>& oligos) {
        cout << "\n" << header << ": " << endl;
        for (auto& oligo : oligos) {
            cout << oligo << endl;
        }
    }

    static int printLocations(const string& header, const vector<Location>& locations) {
        cout << "\n" << header << ": " << endl;
        for (auto& loc : locations) {
            cout << loc << endl;
        }
    }

    static int printOligosWithLocations(const string& header, const vector<string>& oligos, const vector<Location>& locations) {
        cout << "\n" << header << ": " << endl;
        for (int i=0; i<oligos.size(); i++) {
            cout << oligos[i] << "  " << locations[i] << endl;
        }
    }

    static int printGraph(DnaStructure& structure) {
        cout << "\nGraph: " << endl;
        vector<int>** graph = structure.getGraph();
        const int m = structure.getOligosSize();
        for (int i=0; i<m; i++) {
            for (int j=0; j<m; j++) {
                for (int l=0; l<graph[i][j].size(); l++) {
                    cout << graph[i][j][l] << ", ";
                }
                cout << "|";
            }
            cout << endl;
        }
    }

    static int printGraph2(vector<int>** graph, int m) {
        for (int i=0; i<m; i++) {
            for (int j=0; j<m; j++) {
                for (int l=0; l<graph[i][j].size(); l++) {
                    cout << graph[i][j][l] << ", ";
                }
                cout << "|";
            }
            cout << endl;
        }
    }

    static int printResultDNA(const string resultDNA) {
        cout << "Result DNA:\n  " << resultDNA << endl;
    }

    static int printDistance(const int distance) {
        cout << "Distance: " << distance << endl;
    }

    static int printErrors(const vector<string>& oligos) {
        int err = 0;
        for (int i=0; i<oligos.size()-1; i++) {
            if (oligos[i].compare(oligos[i+1]) > 0) {
                cout << "Error: " << i << " - " << i+1 << endl;
                err++;
            }
        }
        cout << "Total errors: " << err << endl;
    }

    static int printResults(
        const string header,
        const vector<Pair>& result,
        const vector<string>& oligos,
        const string& dnaStr) {
        
        string resultDNA = makeDNA(result, oligos);
        cout << "\n" << header << ": " << endl;
        if (!(TO_PRINT & Printer::RESULTS_WITHOUT_DNA_STRING))
            cout << "  " << resultDNA << endl;
        printDistance(levenshteinDistance(dnaStr, resultDNA));
        cout << "Used oligonucleotides: " << result.size() << "/" << dnaStr.size() - k + 1 << endl;
        if (header == "Final result") {
            cout << endl;
        }
    }

};