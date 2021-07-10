#include "additives.h"

#pragma once

using namespace std;

int minimum(int a, int b, int c) {
    return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

int levenshteinDistance(std::string& str1, std::string& str2) {
    int p = str1.size();
    int q = str2.size();
    int** dist = new int* [p+1];
    for (int i=0; i<p+1; i++)
        dist[i] = new int [q+1];

    for (int i=0; i<=p; i++)
        dist[i][0] = i;
    for (int j=1; j<=q; j++)
        dist[0][j] = j;
    
    for (int i=1; i<=p; i++)
        for (int j=1; j<=q; j++) {
            int cost = str1[i-1] == str2[j-1] ? 0 : 1;
            dist[i][j] = minimum(
                dist[i-1][j] + 1,       // deletion
                dist[i][j-1] + 1,       // insertion
                dist[i-1][j-1] + cost   // replacement
            );
        }

    int distance = dist[p][q];

    for (int i=0; i<p+1; i++)
        delete [] dist[i];
    delete [] dist;

    return distance;
}

string makeDNA(vector<Pair> result, vector<string>& oligos)
{
    string dna = oligos[result[0].index];
    for (int i=1; i<result.size(); i++)
        dna += oligos[result[i].index].substr(k-result[i].weight);
    
    return dna;
}