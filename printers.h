#include "additives.h"

#pragma once

void printGraph(vector<int>** graph, int m) {
    cout << "GRAPH: " << endl;
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