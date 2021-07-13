#include "additives.h"

#pragma once

void printDNA(const string dna) {
    cout << "DNA:\n" << dna << endl;
}

void printOligos(const vector<string>& oligos) {
    cout << "Oligos: " << endl;
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