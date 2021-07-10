#include "dna.h"
#include "dnaStructure.h"
#include "greedy.h"
#include "tabu.h"
#include "finals.h"
#include "printers.h"
#include "additives.h"

int main() {

    Dna dna;

    string dnaStr = dna.getDna();
    vector<string> oligos = dna.getOligos();
    string first = dna.getFirst();
    
    cout << "dnaStr: " << dnaStr << endl;
    // cout << "oligos:" << endl;
    // for (int i=0; i<oligos.size(); i++) {
    //     cout << oligos[i] << endl;
    // }

    toSet(oligos);
    // cout << "oligos:" << endl;
    // for (int i=0; i<oligos.size(); i++) {
    //     cout << oligos[i] << endl;
    // }

    int m = oligos.size();

    cout<<"generate"<<endl;

    vector<int>** graph = new vector<int>* [oligos.size()];
    for (int i=0; i<m; i++) {
        graph[i] = new vector<int> [oligos.size()];
    }

    generateGraph(oligos, graph, m);
    // printGraph(graph, m);

    cout<<"greedy"<<endl;
    
    vector<Pair> result = greedy(oligos, graph, m, first);

    cout<<"makeDNA"<<endl;
    string resultDNA = makeDNA(result, oligos);

    cout<<resultDNA<<endl;

    cout << "Distance: " << levenshteinDistance(dnaStr, resultDNA) << endl;

    for (int i=0; i<m; i++)
        delete [] graph[i];
    delete [] graph;

    return 0;
}