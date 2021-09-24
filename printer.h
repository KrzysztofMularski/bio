#pragma once

#include <iomanip>
#include "additives.h"
#include "finals.h"

class Printer {
private:
    Printer() {}
public:

    static const int COUNTER                            = 0b00000000000000001;
    static const int INITIALS                           = 0b00000000000000010;
    static const int STARTING_DNA                       = 0b00000000000000100;
    static const int ORIGINAL_OLIGOS                    = 0b00000000000001000;
    static const int ORIGINAL_LOCATIONS                 = 0b00000000000010000;
    static const int ORIGINAL_OLIGOS_WITH_LOCATIONS     = 0b00000000000100000;
    static const int POSITIVE_ERRORS                    = 0b00000000001000000;
    static const int NEGATIVE_ERRORS                    = 0b00000000010000000;
    static const int WITH_ERRORS_OLIGOS                 = 0b00000000100000000;
    static const int WITH_ERRORS_LOCATIONS              = 0b00000001000000000;
    static const int WITH_ERRORS_OLIGOS_WITH_LOCATIONS  = 0b00000010000000000;
    static const int GRAPH                              = 0b00000100000000000;
    static const int RESULTS_WITHOUT_DNA_STRING         = 0b00001000000000000;
    static const int RESULTS_GREEDY                     = 0b00010000000000000;
    static const int RESULTS_AFTER_COMPACTION           = 0b00100000000000000;
    static const int RESULTS_AFTER_LENGTHENING          = 0b01000000000000000;
    static const int RESULTS_FINAL                      = 0b10000000000000000;

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

    static int printOligos(const string& header, const vector<string>& oligos, const string& emptyHeader = nullptr) {
        if (oligos.size()) {
            cout << "\n" << header << ": " << endl;
            for (auto& oligo : oligos) {
                cout << oligo << endl;
            }
        } else {
            cout << "\n" << emptyHeader << endl;
        }
    }

    static int printNegativeErrors(const string& header, const vector<int>& indexes, const vector<string>& oligos, const string& emptyHeader = nullptr) {
        if (indexes.size()) {
            cout << "\n" << header << ": " << endl;
            for (const int& index : indexes) {
                cout << "oligos[" << index << "]: " << oligos[index] << endl;
            }
        } else {
            cout << "\n" << emptyHeader << endl;
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

    static int printGraph(vector<int>** graph, const int m) {
        cout << "\nGraph: " << endl;
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