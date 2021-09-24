#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

void print(const vector<int>& vec) {
    cout << vec.size() << ": ";
    for (const int& a : vec) {
        cout << a << " ";
    }
    cout << endl;
}

void toSet(vector<int>& oligos) {
    auto end = oligos.end();
    cout << "begin:" << *(oligos.begin()) << endl;
    cout << "18. end:" << *end << endl;
    for (auto it = oligos.begin(); it != end; ++it) {
        end = remove(it + 1, end, *it);
        cout << "end:" << *end << endl;
    }
    cout << "--------------" << endl;
    print(oligos);
    cout << "23. end:" << *end << endl;
    oligos.erase(end, oligos.end());
}

int main() {
    
    vector<int> arr = {1, 2, 3, 3, 2, 1};

    print(arr);

    toSet(arr);

    cout << "---------" << endl;

    print(arr);

    return 0;
}