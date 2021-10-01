#pragma once

#include <iomanip>
#include "additives.h"
#include "finals.h"

class Printer {
private:
    Printer() {}
public:

    static const int COUNTER                            = 0b00000000000000000001;
    static const int INITIALS                           = 0b00000000000000000010;
    static const int STARTING_DNA                       = 0b00000000000000000100;
    static const int ORIGINAL_OLIGOS                    = 0b00000000000000001000;
    static const int ORIGINAL_LOCATIONS                 = 0b00000000000000010000;
    static const int ORIGINAL_OLIGOS_WITH_LOCATIONS     = 0b00000000000000100000;
    static const int POSITIVE_ERRORS                    = 0b00000000000001000000;
    static const int NEGATIVE_ERRORS                    = 0b00000000000010000000;
    static const int WITH_ERRORS_OLIGOS                 = 0b00000000000100000000;
    static const int WITH_ERRORS_LOCATIONS              = 0b00000000001000000000;
    static const int WITH_ERRORS_OLIGOS_WITH_LOCATIONS  = 0b00000000010000000000;
    static const int GRAPH                              = 0b00000000100000000000;
    static const int RESULTS_WITHOUT_DNA_STRING         = 0b00000001000000000000;
    static const int RESULTS_GREEDY                     = 0b00000010000000000000;
    static const int RESULTS_AFTER_COMPACTION           = 0b00000100000000000000;
    static const int RESULTS_AFTER_LENGTHENING          = 0b00001000000000000000;
    static const int RESULTS_FINAL                      = 0b00010000000000000000;
    static const int RESULTS_GLOBAL_FINAL               = 0b00100000000000000000;
    static const int LOADING                            = 0b01000000000000000000;
    static const int END_TIME                           = 0b10000000000000000000;

    static int printCounter(const int& counter) {
        if (counter == 1) {
            cout << endl;
        }
        cout << "=============== Instance " << counter << " ===============" << endl;
    }

    static int printInitials() {
        cout << endl;
        cout << setw(3) << left << n << " - DNA length" << endl;
        cout << setw(3) << left << k << " - Oligonucleotide length" << endl;
        cout << setw(3) << left << POSITIVE_ERRORS_PERCENTAGE << " - Positive error percentage" << endl;
        cout << setw(3) << left << NEGATIVE_ERRORS_PERCENTAGE << " - Negative error percentage" << endl;
        cout << setw(3) << left << LOCATION_RANGE << " - Location range" << endl;
        string randomTypeString = (LOCATION_RANDOM_TYPE == Random_Type::LINEAR) ? "Linear" : "Gaussian";
        cout << setw(3) << left << randomTypeString << " random type for generating location interval" << endl;
        cout << setw(3) << left << GREEDY_DEPTH << " - Greedy depth" << endl;
        cout << setw(3) << left << TABU_LIST_LENGTH << " - Tabu list length" << endl;
        cout << setw(3) << left << TABU_LIST_CLUSTERS_LENGTH << " - Tabu list for clusters length" << endl;
        cout << setw(3) << left << MAX_TABU_ITERATIONS << " - Tabu max iterations" << endl;
        cout << setw(3) << left << MAX_TABU_ITERATIONS_WITH_NO_IMPROVEMENT << " - Tabu max iterations with no improvement" << endl;
        cout << setw(3) << left << GLOBAL_MAX_ITERATIONS << " - Global max iterations" << endl;
        cout << setw(3) << left << CLUSTER_OVERLAP_CRITERION << " - Cluster overlap criterion" << endl;
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

    static int printResultDNA(const string& resultDNA) {
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
        cout << "Result DNA length: " << resultDNA.size() << "/" << dnaStr.size() << " =~ " << (double)resultDNA.size() / (double)dnaStr.size() << endl;
        cout << "Used oligonucleotides: " << result.size() << "/" << oligos.size() << " =~ " << (double)result.size() / (double)oligos.size() << endl;
        cout << "Global rating: " << result.size() << "/" << resultDNA.size() << " =~ " << (double)result.size() / (double)resultDNA.size() << endl;
        cout << "Result hash: " << calcHash(result, oligos) << endl;
    }

    static int printLoading(const int instancesSize, const int maxCyclesNumber, const int isEnd = false) {
        cout << "                              \r";
        cout << " Instance: " << INSTANCE_COUNTER << "/" << instancesSize << " ( " << (float)CURRENT_CYCLES_NUMBER / (float)maxCyclesNumber * 100 << "% )\r";
        fflush(stdout);
        if (isEnd) {
            cout << endl;
        }
    }

    static int printEnd(clock_t time) {
        cout << "\nInstance took " << (float)time / CLOCKS_PER_SEC << " seconds to complete" << endl;
        cout << endl;
    }
};