#include "additives.h"
#include <iostream>
#include <algorithm>
#include <vector>

#pragma once

class Tabu{
    string dna;
    vector<string> oligos;
    vector<string> tabu;
    int oligoLength;
    float evaluation;
    vector<int>** graph;
    
    // DnaStructure structure;

public:
    Tabu(string dna, vector<string>& oligos, vector<int> **graph): dna(dna), oligos(oligos), graph(graph) {
        oligoLength = k;
        evaluation = numberOfOligos(dna) / dna.length();
     }

    //policzyć ile jest oligo w "pierwotnym" dna
    int numberOfOligos(string checkDna)
    {
        int howManyOligos = 0;
        for(int i = 0; i < oligos.size(); i++)
        {
            howManyOligos += countSubstring(checkDna, oligos[i]);
        }

        return howManyOligos;
    }    

    // returns count of non-overlapping occurrences of 'sub' in 'str'
    int countSubstring(const string& str, const string& sub)
    {
        if (sub.length() == 0) 
            return 0;

        int count = 0;
        for (size_t offset = str.find(sub); offset != std::string::npos;
            offset = str.find(sub, offset + sub.length()))
        {
            ++count;
        }

        return count;
    }

    void minMax(const vector<int>& weights, int& mini, int& maxi)
    {
        for(int i = 0; i < weights.size(); i ++)
        {
            if(weights[i] < mini)
                mini = weights[i];
            else if(weights[i] > maxi)
                maxi = weights[i];
        }
    }


    string compaction()
    {
        //vector<Pair> whichOligo;        //funkcja oceny stanu
        // vector<Rank> whichOligo;
        // Rank oligoRank;
        string tempOligo, oligoToCompare, substringDna, shortenedDna;
        string tempDna = dna;
        int comparing, index, largestElement, smallestElement;
        float tempEvaluation;
        //int help = 0;

        while(tempDna.length() > oligoLength) //tempDna.length() > oligoLength
        {
            tempOligo = tempDna.substr(0, oligoLength);
            cout<<"temp oligo: "<<tempOligo<<endl;
            
            for(int i = 0; i < oligos.size(); i ++)
            {
                comparing = tempOligo.compare(oligos[i]);
                if(comparing == 0)
                {
                    index = i;
                    break;
                }
            }
            cout<<"index of temp oligo: "<< index <<endl;

            for(int i = 0; i < oligoLength; i++)
            {
                cout<<"-------------"<<i<<"------------"<<endl;
                oligoToCompare = tempDna.substr(i, oligoLength);
                cout<<"oligo to compare: "<<oligoToCompare<<endl;

                for(int j = 0; j < graph[index][i].size() ; j++)      //j < od długości wiersza powinno być
                {
                    //cout<<"j: "<<j<<endl;
                    comparing = oligoToCompare.compare(oligos[j]);
                    if(comparing == 0)
                    {
                        cout<<"jesteśmy w IF"<<endl;
                        //largestElement  = *max_element(graph[index][j].begin(),graph[index][j].end());
                        //whichOligo.push_back({j, largest_element});
                        //smallestElement = *min_element(graph[index][j].begin(),graph[index][j].end());
                        smallestElement = graph[index][j].size() + 1;
                        largestElement = 0;
                        minMax(graph[index][j], smallestElement, largestElement);

                        //od najmniejszej do największej liczby dopasaowań dla danego oligo sprawdzamy po usunięciu tego oligo jaka będzie f. stanu
                        for(int l = smallestElement; l <= largestElement; l++)
                        {
                            substringDna = tempDna.substr(l, tempDna.length()-l);
                            tempEvaluation = numberOfOligos(substringDna) / substringDna.length();
                            
                            if(tempEvaluation > evaluation)
                            {
                                // oligoRank = {index, substringDna, tempEvaluation};
                                // whichOligo.push_back(oligoRank);
                                shortenedDna = substringDna;
                                evaluation = tempEvaluation;
                            }
                        }

                        tempDna = shortenedDna;
                        break;
                    }
                }
            }
        }

        return tempDna;
    }
    

    ~Tabu() {}
};