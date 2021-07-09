#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#define n 10   // długość DNA
#define k 4     // długość oligonukleotydu

using namespace std;

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
        dna = "ACTGAACTGG"; // temp
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
        return oligos[0];
    }
};

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
        weights.push_back(0);
        return weights;
    }

    for (int i=1; i<size; i++)
        if (s1.substr(i, size-i) == s2.substr(0, size-i))
            weights.push_back(i);

    if (weights.size() == 0)
        weights.push_back(-1);

    return weights;
}

void generateGraph(vector<string>& oligos, vector<int>** graph, int m) {
    for (int i=0; i<m; i++)
        for (int j=0; j<m; j++)
            graph[i][j] = calcOligosWeights(oligos[i], oligos[j]);
}

void greedy(vector<string>& oligos) {

}

int main() {

    Dna dna;

    string dnaStr = dna.getDna();
    vector<string> oligos = dna.getOligos();
    string first = dna.getFirst();
    
    cout << "dnaStr:" << dnaStr << endl;
    cout << "oligos:" << endl;
    for (int i=0; i<oligos.size(); i++) {
        cout << oligos[i] << endl;
    }

    toSet(oligos);
    cout << "oligos:" << endl;
    for (int i=0; i<oligos.size(); i++) {
        cout << oligos[i] << endl;
    }

    int m = oligos.size();

    vector<int>** graph = new vector<int>* [oligos.size()];
    for (int i=0; i<m; i++) {
        graph[i] = new vector<int> [oligos.size()];
    }

    generateGraph(oligos, graph, m);
    


    for (int i=0; i<m; i++)
        delete [] graph[i];
    delete [] graph;

    return 0;
}