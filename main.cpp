#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>

#define n 20   // długość DNA
#define k 4    // długość oligonukleotydu

using namespace std;

struct Pair{
    int index;
    int weight;
};

class Dna {
private:
    string dna;
    vector<string> oligos;
public:
    static bool strCompare(string a, string b) {
        return (a < b);
    }

    Dna() {
        dna = string(n, 'A');
        srand(time(NULL));

        // generowanie dna o długości n
        for (int i=0; i<n; i++) {
            dna[i] = 'A' + rand() % 4;
            if (dna[i] == 'B')
                dna[i] = 'T';
            else if (dna[i] == 'D')
                dna[i] = 'G';
        }

        // generowanie (m = n - k + 1) oligonukleotydów o długości k
        int m = n - k + 1;
        // dna = "CCCGA"; // temp
        oligos = vector<string>(m, string(k, 'A'));
        for (int i=0; i<m; i++) {
            oligos[i] = dna.substr(i, k);
        }

        // oligonukleotydy przed posortowaniem
        // for (int i=0; i<m; i++)
        //     std::cout << "i=" << i << ": " << oligos[i] << std::endl;
        // std::cout << dna << std::endl;

        // mieszanie oligonukleotydów - sortowanie w kolejności alfabetycznej
        sort(oligos.begin(), oligos.end(), strCompare);

        // oligonukleotydy po posortowaniu
        // for (int i=0; i<m; i++)
        //     std::cout << "i=" << i << ": " << oligos[i] << std::endl;
        // std::cout << dna << std::endl;

        // (niepotrzebne) sprawdzenie czy jest po kolei
        // int err = 0;
        // for (int i=0; i<m-1; i++) {
        //     if (oligos[i].compare(oligos[i+1]) > 0) {
        //         std::cout << "error: " << i << " - " << i+1 << std::endl;
        //         err++;
        //     }
        // }
        // std::cout << "Total errors: " << err << std::endl;
    }

    ~Dna() {}

    string getDna() {
        return dna;
    }

    vector<string> getOligos() {
        return oligos;
    }

    string getFirst() {
        return dna.substr(0, k);
    }
};

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

void toSet(vector<string>& oligos) {
    auto end = oligos.end();
    for (auto it = oligos.begin(); it != end; ++it) {
        end = remove(it + 1, end, *it);
    }
    oligos.erase(end, oligos.end());
}

vector<int> calcOligosWeights(const string& s1, const string& s2) {

    vector<int> weights;
    int size = s1.size();

    if (s1 == s2) {
        weights.push_back(k+1);
        return weights;
    }

    for (int i=1; i<size; i++)
        if (s1.substr(i, size-i) == s2.substr(0, size-i))
            weights.push_back(i);

    if (weights.size() == 0)
        weights.push_back(k);

    return weights;
}

void generateGraph(vector<string>& oligos, vector<int>** graph, int m) {
    for (int i=0; i<m; i++)
        for (int j=0; j<m; j++)
            graph[i][j] = calcOligosWeights(oligos[i], oligos[j]);
}

int getIndex(vector<string>& v, string s)
{
    auto it = find(v.begin(), v.end(), s);
 
    if (it != v.end())
        return it - v.begin();
    else
        return -1;
}

Pair findBest(vector<int>* graphRow, int m, vector<int>& visited) {
    Pair best;
    best.index = -1;
    best.weight = -1;
    int bestCellWeight, min = k+2;
    for(int i = 0; i < m; i++)
    {
        auto it = find(visited.begin(), visited.end(), i);
        if(it == visited.end()){
            bestCellWeight = *min_element(graphRow[i].begin(), graphRow[i].end());
            if(bestCellWeight < min){
                min = bestCellWeight;
                best.weight = bestCellWeight;
                best.index = i;
            }
        }      
    }
    return best;
}

vector<Pair> greedy(vector<string>& oligos, vector<int>** graph, int m, string first) {
    vector<int> visited;
    vector<Pair> result;
    int currentDNAlength = k;
    Pair pair;
    
    int indexFirst = getIndex(oligos, first);
    pair.index = indexFirst;
    pair.weight = -1;
    result.push_back(pair);
    visited.push_back(indexFirst);
    int index = indexFirst;

    while(true) {
        pair = findBest(graph[index], m, visited);
        if (pair.index == -1)
            break;
        currentDNAlength += pair.weight;
        if (currentDNAlength > n)
            break;
        result.push_back(pair);
        visited.push_back(pair.index);
        index = pair.index;
    }
    return result;
}

string makeDNA(vector<Pair> result, vector<string>& oligos)
{
    string dna = oligos[result[0].index];
    for (int i=1; i<result.size(); i++)
        dna += oligos[result[i].index].substr(k-result[i].weight);
    
    return dna;
}

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