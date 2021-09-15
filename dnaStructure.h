#include "additives.h"

#pragma once

class DnaStructure {
private:
    vector<string> oligos;
    int oligosSize;
    vector<int>** graph;
public:
    DnaStructure(vector<string> oligos) : oligos(oligos) {}

    ~DnaStructure() {
        for (int i=0; i<oligosSize; i++)
            delete [] graph[i];
        delete [] graph;
    }

    void generateErrors() {
        toSet();
        oligos = {
            "AACT",
            "AACC",
            "ACGT",
            "CCTG",
            "CGTA",
            "CTGG",
            "GTAA"
        };


        // uwzględnić tablice z pozycjami

        // todo: dodać generowanie błędów negatywnych i pozytywnych
    }

    void generateGraph() {
        oligosSize = oligos.size();
        graph = new vector<int>* [oligosSize];
        for (int i=0; i<oligosSize; i++) {
            graph[i] = new vector<int> [oligosSize];
        }
    }

    void populateGraph() {
        for (int i=0; i<oligosSize; i++)
            for (int j=0; j<oligosSize; j++)
                graph[i][j] = calcOligosWeights(oligos[i], oligos[j]);
    }

    vector<string>& getOligos() {
        return oligos;
    }

    int getOligosSize() {
        return oligosSize;
    }

    vector<int>** getGraph() {
        return graph;
    }

    void toSet() {
        auto end = oligos.end();
        for (auto it = oligos.begin(); it != end; ++it) {
            end = remove(it + 1, end, *it);
        }
        oligos.erase(end, oligos.end());
    }

    static vector<int> calcOligosWeights(const string& s1, const string& s2) {

        vector<int> weights;
        int size = s1.size();

        for (int i=1; i<size; i++)
            if (s1.substr(i, size-i) == s2.substr(0, size-i))
                weights.push_back(i);

        weights.push_back(k);

        return weights;
    }
};