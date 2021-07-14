#include "additives.h"
#include "dnaStructure.h"

#pragma once

void print(const char* arr) {
    cout << arr << endl;
}

void printDNA(const string dna) {
    cout << "DNA:\n" << dna << endl;
}

void printOligos(const vector<string>& oligos) {
    cout << "Oligonucleotides: " << endl;
    for (int i=0; i<oligos.size(); i++) {
        cout << oligos[i] << endl;
    }
}

void printGraph(DnaStructure& structure) {
    cout << "Graph: " << endl;
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

void printResultDNA(const string resultDNA) {
    cout << "Result DNA:\n" << resultDNA << endl;
}

void printDistance(const int distance) {
    cout << "Distance: " << distance << endl;
}

void printErrors(const vector<string>& oligos) {
    int err = 0;
    for (int i=0; i<oligos.size()-1; i++) {
        if (oligos[i].compare(oligos[i+1]) > 0) {
            cout << "Error: " << i << " - " << i+1 << endl;
            err++;
        }
    }
    cout << "Total errors: " << err << endl;
}