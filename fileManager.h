#pragma once

#include <fstream>
#include <iomanip>
#include "additives.h"
#include "finals.h"

class Instance;

class FileManager {
private:
    string filename;
public:
    FileManager(string filename) : filename(filename) {}

    void write(const int& counter) {
        ofstream file;
        file.open(filename, ios::app);

        file << "=============== Instance " << counter << " ===============" << endl;

        file.close();
    }

    void write(const Instance& instance) {
        ofstream file;
        file.open(filename, ios::app);

        file << setw(3) << left << n << " - DNA length" << endl;
        file << setw(3) << left << k << " - Oligonucleotide length" << endl;
        file << setw(3) << left << POSITIVE_ERRORS_PERCENTAGE << " - Positive error percentage" << endl;
        file << setw(3) << left << NEGATIVE_ERRORS_PERCENTAGE << " - Negative error percentage" << endl;
        file << setw(3) << left << LOCATION_RANGE << " - Location range" << endl;
        string randomTypeString = (LOCATION_RANDOM_TYPE == Random_Type::LINEAR) ? "Linear" : "Gaussian";
        file << setw(3) << left << randomTypeString << " random type for generating location interval" << endl;
        file << setw(3) << left << GREEDY_DEPTH << " - Greedy depth" << endl;
        file << setw(3) << left << TABU_LIST_LENGTH << " - Tabu list length" << endl;
        file << setw(3) << left << TABU_LIST_CLUSTERS_LENGTH << " - Tabu list for clusters length" << endl;
        file << setw(3) << left << MAX_TABU_ITERATIONS << " - Tabu max iterations" << endl;
        file << setw(3) << left << MAX_TABU_ITERATIONS_WITH_NO_IMPROVEMENT << " - Tabu max iterations with no improvement" << endl;
        file << setw(3) << left << GLOBAL_MAX_ITERATIONS << " - Global max iterations" << endl;
        file << setw(3) << left << CLUSTER_OVERLAP_CRITERION << " - Cluster overlap criterion" << endl;

        file.close();
    }

    void write(const string& dna, const bool& isRandomized) {
        ofstream file;
        file.open(filename, ios::app);

        file << "==========================================" << endl;
        if (isRandomized) {
            file << "Dna string (randomly generated):" << endl;
        } else {
            file << "Dna string:" << endl;
        }
        file << dna << endl;

        file.close();
    }

    void write(
        const string& header,
        const vector<Pair>& result,
        const vector<string>& oligos,
        const string& dnaStr) {
        
        ofstream file;
        file.open(filename, ios::app);

        string resultDNA = makeDNA(result, oligos);
        file << "==========================================" << endl;
        file << header << ": " << endl;
        file << "Distance: " << levenshteinDistance(dnaStr, resultDNA) << endl;
        file << "Result DNA length: " << resultDNA.size() << "/" << dnaStr.size() << " =~ " << (double)resultDNA.size() / (double)dnaStr.size() << endl;
        file << "Used oligonucleotides: " << result.size() << "/" << oligos.size() << " =~ " << (double)result.size() / (double)oligos.size() << endl;
        file << "Global rating: " << result.size() << "/" << resultDNA.size() << " =~ " << (double)result.size() / (double)resultDNA.size() << endl;
        file << "Result hash: " << calcHash(result, oligos) << endl;

        file.close();
    }

    void write(const clock_t& finalTime) {
        ofstream file;
        file.open(filename, ios::app);

        file << "==========================================" << endl;
        file << "An instance took " << (float)finalTime / CLOCKS_PER_SEC << " seconds to complete" << endl;

        file.close();
    }

    void writeHeaders() {
        ofstream file;
        file.open(filename, ios::app);

        file << "Instance No" << ";";
        file << "n" << ";";
        file << "k" << ";";
        file << "Positive errors [%]" << ";";
        file << "Negative errors [%]" << ";";
        file << "Location range" << ";";
        file << "Location random type" << ";";
        file << "Greedy depth" << ";";
        file << "Tabu list length" << ";";
        file << "Tabu list for clusters length" << ";";
        file << "Max tabu iterations" << ";";
        file << "Max tabu iterations with no improvement" << ";";
        file << "Global max iterations" << ";";
        file << "Cluster overlap criterion" << ";";
        file << "DNA randomness" << ";";
        file << "DNA string" << ";";

        file << "(First solution) Levenshtein distance" << ";";
        file << "(First solution) Original DNA string length" << ";";
        file << "(First solution) Oligonucleotides vector length" << ";";
        file << "(First solution) Result vector length" << ";";
        file << "(First solution) Result DNA length" << ";";
        file << "(First solution) Result DNA length in comparison with Original DNA length" << ";";
        file << "(First solution) Used oligonucleotides number" << ";";
        file << "(First solution) Global rating (Result vector length / Result DNA length)" << ";";
        file << "(First solution) Result hash" << ";";

        file << "(Best solution) Levenshtein distance" << ";";
        file << "(Best solution) Original DNA string length" << ";";
        file << "(Best solution) Oligonucleotides vector length" << ";";
        file << "(Best solution) Result vector length" << ";";
        file << "(Best solution) Result DNA length" << ";";
        file << "(Best solution) Result DNA length in comparison with Original DNA length" << ";";
        file << "(Best solution) Used oligonucleotides number" << ";";
        file << "(Best solution) Global rating (Result vector length / Result DNA length)" << ";";
        file << "(Best solution) Result hash" << ";";

        file << "Duration of the instance [s]" << endl;

        file.close();
    }

    void writeCompact(const int& counter) {
        ofstream file;
        file.open(filename, ios::app);

        file << counter << ";";

        file.close();
    }

    void writeCompact(const Instance& instance) {
        ofstream file;
        file.open(filename, ios::app);

        file << n << ";";
        file << k << ";";
        file << POSITIVE_ERRORS_PERCENTAGE << ";";
        file << NEGATIVE_ERRORS_PERCENTAGE << ";";
        file << LOCATION_RANGE << ";";
        string randomTypeString = (LOCATION_RANDOM_TYPE == Random_Type::LINEAR) ? "Linear" : "Gaussian";
        file << randomTypeString << ";";
        file << GREEDY_DEPTH << ";";
        file << TABU_LIST_LENGTH << ";";
        file << TABU_LIST_CLUSTERS_LENGTH << ";";
        file << MAX_TABU_ITERATIONS << ";";
        file << MAX_TABU_ITERATIONS_WITH_NO_IMPROVEMENT << ";";
        file << GLOBAL_MAX_ITERATIONS << ";";
        file << CLUSTER_OVERLAP_CRITERION << ";";

        file.close();
    }

    void writeCompact(const string& dna, const bool& isRandomized) {
        ofstream file;
        file.open(filename, ios::app);

        if (isRandomized) {
            file << "Random" << ";";
        } else {
            file << "Not random" << ";";
        }
        file << dna << ";";

        file.close();
    }

    void writeCompact(
        const vector<Pair>& result,
        const vector<string>& oligos,
        const string& dnaStr) {
        
        ofstream file;
        file.open(filename, ios::app);

        string resultDNA = makeDNA(result, oligos);
        file << levenshteinDistance(dnaStr, resultDNA) << ";";
        file << dnaStr.size() << ";";
        file << oligos.size() << ";";
        file << result.size() << ";";
        file << resultDNA.size() << ";";
        file << (double)resultDNA.size() / (double)dnaStr.size() << ";";
        file << (double)result.size() / (double)oligos.size() << ";";
        file << (double)result.size() / (double)resultDNA.size() << ";";
        file << calcHash(result, oligos) << ";";

        file.close();
    }

    void writeCompact(const clock_t& finalTime) {
        ofstream file;
        file.open(filename, ios::app);

        file << (float)finalTime / CLOCKS_PER_SEC << endl;

        file.close();
    }
};