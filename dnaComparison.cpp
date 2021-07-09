/**
 * 
 *  Levenshtein distance for comparing two DNA sequences
 *  pseudocode source: Wikipedia
 * 
 **/

#include <string>
#include <iostream>

int minimum(int a, int b, int c) {
    return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

int levenshteinDistance(std::string& str1, std::string& str2) {
    int m = str1.size();
    int n = str2.size();
    int** dist = new int* [m+1];
    for (int i=0; i<m+1; i++)
        dist[i] = new int [n+1];

    for (int i=0; i<=m; i++)
        dist[i][0] = i;
    for (int j=1; j<=n; j++)
        dist[0][j] = j;
    
    for (int i=1; i<=m; i++)
        for (int j=1; j<=n; j++) {
            int cost = str1[i-1] == str2[j-1] ? 0 : 1;
            dist[i][j] = minimum(
                dist[i-1][j] + 1,       // deletion
                dist[i][j-1] + 1,       // insertion
                dist[i-1][j-1] + cost   // replacement
            );
        }

    int distance = dist[m][n];

    for (int i=0; i<m+1; i++)
        delete [] dist[i];
    delete [] dist;

    return distance;
}

int main() {

    std::string str1 = "ACGTAACTGG"; //oryginalny
    std::string str2 = "ACGTAACCTG"; //po zachłannym
    std::string str3 = "";


    int distance = levenshteinDistance(str1, str2);

    std::cout << "string 1: " << str1 << std::endl;
    std::cout << "string 2: " << str2 << std::endl;
    std::cout << "distance: " << distance << std::endl;
    
    return 0;
}