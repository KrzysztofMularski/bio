#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#define n 10   // długość DNA
#define k 4     // długość oligonukleotydu

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
        //dna = "ACTGAACTGG"; // temp
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

Pair minWeight(vector<int>* graph, string oligo, int m, vector<int>& visited)
{
    Pair next;
    int temp, mini = k;

    for(int i = 0; i < m; i++)
    {
        auto it = find(visited.begin(), visited.end(), i);
        if(it == visited.end()){
            temp = *min_element(graph[i].begin(), graph[i].end());
            if(temp < mini){
                mini = temp;
                next.weight = temp;
                next.index = i;
            }  
        }      
    }

    return next;
}

vector<Pair> greedy(vector<string>& oligos, vector<int>** graph, int m, string first) {
    vector<int> visited;
    vector<Pair> result;
    Pair temp, toPush;
    int currentLength = k;

    int index = getIndex(oligos, first);
    int previousIndex;

    visited.push_back(index);
    temp = minWeight(graph[index], first, m, visited);
    previousIndex = temp.index;
    toPush.index = index;
    toPush.weight = temp.weight;
    result.push_back(toPush);

    while(currentLength < n)
    {
        temp = minWeight(graph[previousIndex], oligos[toPush.index], m, visited);

        if(currentLength + temp.weight <= n)
        {        
            toPush.index = temp.index;
            toPush.weight = temp.weight;
            visited.push_back(toPush.index);
            result.push_back(toPush);
            currentLength += temp.weight;
        }
    }

    return result;
}

string makeDNA(vector<Pair> result, vector<string>& oligos)
{
    string dna;
    dna += oligos[result[0].index];
    for(int i = 1; i < result.size(); i++)
        dna += oligos[result[i].index].substr(k-result[i-1].weight);

    return dna;
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

    cout<<"generate"<<endl;

    vector<int>** graph = new vector<int>* [oligos.size()];
    for (int i=0; i<m; i++) {
        graph[i] = new vector<int> [oligos.size()];
    }

    generateGraph(oligos, graph, m);

    cout<<"result";
    
    vector<Pair> result = greedy(oligos, graph, m, first);

    cout<<"greeedy";
    string resultDNA = makeDNA(result, oligos);

    cout<<resultDNA;
    cout<<endl;


    for (int i=0; i<m; i++)
        delete [] graph[i];
    delete [] graph;

    return 0;
}