#pragma once

#include "additives.h"
#include "printer.h"
#include <set>

class DnaStructure {
private:
    vector<string>& oligos;
    vector<int>& oligosAsNumbers;
    vector<Location>& locations;
    int oligosSize;
    vector<int>** graph;
    Locator locator;
public:
    DnaStructure(
        vector<string>& oligos,
        vector<int>& oligosAsNumbers,
        vector<Location>& locations
    ) :
        oligos(oligos),
        oligosAsNumbers(oligosAsNumbers),
        locations(locations)
    {
        oligosSize = oligos.size();
    }

    ~DnaStructure() {
        for (int i=0; i<oligosSize; i++)
            delete [] graph[i];
        delete [] graph;
    }

    static void convertOligoCodeToString(string& oligo, int codeBase10) {
        for (int i=k-1; i>=0; --i) {
            int mod4 = codeBase10 % 4;
            if (mod4 < 2) {
                oligo[i] = mod4 == 0 ? 'A' : 'C';
            } else {
                oligo[i] = mod4 == 2 ? 'G' : 'T';
            }
            codeBase10 /= 4;
            if (codeBase10 == 0) {
                break;
            }
        }
    }

    static void generateRandomOligo(string& oligo) {
        for (int i=0; i<k; i++) {
            oligo[i] = 'A' + rand() % 4;
            if (oligo[i] == 'B')
                oligo[i] = 'G';
            else if (oligo[i] == 'D')
                oligo[i] = 'T';
        }
    }

    void generatePositiveErrors(vector<string>& posErrors, const int& posErrorsNumber) {
        if (posErrorsNumber == 0) {
            return;
        }
        int posErrorsCurrentIndex = 0;
        int posErrorsLeft = posErrorsNumber;
        if (k < 9) {
            int wrongOligoCounter = 0;
            while (posErrorsLeft > 0) {
                string randomOligo(k, 'A');
                generateRandomOligo(randomOligo);
                int indexInOligos = getIndex(oligos, randomOligo);
                int indexInPosErrors = getIndex(posErrors, randomOligo);
                if (indexInOligos == -1 && indexInPosErrors == -1) {
                    --posErrorsLeft;
                    posErrors[posErrorsCurrentIndex++] = randomOligo;
                    wrongOligoCounter = 0;
                    continue;
                } else {
                    ++wrongOligoCounter;
                    if (wrongOligoCounter > 10)
                        break;
                }
            }
            if (posErrorsLeft == 0)
                return;
        } else {
            while (posErrorsLeft > 0) {
                string randomOligo(k, 'A');
                generateRandomOligo(randomOligo);
                int randomIndex = getIndex(oligos, randomOligo);
                if (randomIndex == -1) {
                    --posErrorsLeft;
                    posErrors[posErrorsCurrentIndex++] = randomOligo;
                    continue;
                }
            }
            if (posErrorsLeft == 0)
                return;
        }
        // random oligo finding failed - starting brute force
        posErrorsCurrentIndex = 0;
        int allOligosSize = 1;
        int base = 4;
        int power = k;
        for (int i=0; i<power; ++i) {
            allOligosSize *= base;
        }
        vector<int> allOligosAsNumbers(allOligosSize);
        for (int i=0; i<allOligosSize; ++i) {
            allOligosAsNumbers[i] = i;
        }
        int lastNumber = -1;
        for (int i=oligosAsNumbers.size()-1; i>=0; --i) {
            int oligoNumber = oligosAsNumbers[i];
            if (oligoNumber == lastNumber) {
                continue;
            }
            allOligosAsNumbers.erase(allOligosAsNumbers.begin() + oligoNumber);
            lastNumber = oligoNumber;
        }
        // allOligosAsNumbers vector now contains all possible oligo codes candidates to be positive error
        if (posErrorsNumber >= allOligosAsNumbers.size()) {
            for (const int& posError : allOligosAsNumbers) {
                convertOligoCodeToString(posErrors[posErrorsCurrentIndex++], posError);
            }
            return;
        }
        posErrorsLeft = posErrorsNumber;
        int allOligosAsNumbersSize = allOligosAsNumbers.size();
        while (posErrorsLeft > 0) {
            int newIndex = rand() % allOligosAsNumbersSize--;
            convertOligoCodeToString(posErrors[posErrorsCurrentIndex++], allOligosAsNumbers[newIndex]);
            allOligosAsNumbers.erase(allOligosAsNumbers.begin() + newIndex);
            --posErrorsLeft;
        }
        return;
    }

    void generateNegativeErrorsIndexes(vector<int>& negErrorsIndexes, const int& negErrorsNumber) {
        if (negErrorsNumber == 0) {
            return;
        }
        if (negErrorsNumber >= oligosSize) {
            negErrorsIndexes.resize(oligosSize);
            negErrorsIndexes.shrink_to_fit();
            for (int i=0; i<oligosSize; ++i) {
                negErrorsIndexes[i] = i;
            }
            return;
        }
        int negErrorsCurrentIndex = 0;
        int negErrorsLeft = negErrorsNumber;
        int wrongOligoCounter = 0;
        while (negErrorsLeft > 0) {
            int randomIndex = rand() % oligosSize;
            int indexInNegErrorsIndexes = getIndex(negErrorsIndexes, randomIndex);
            if (indexInNegErrorsIndexes == -1) {
                --negErrorsLeft;
                negErrorsIndexes[negErrorsCurrentIndex++] = randomIndex;
                wrongOligoCounter = 0;
                continue;
            } else {
                ++wrongOligoCounter;
                if (wrongOligoCounter > 10)
                    break;
            }
        }
        if (negErrorsLeft == 0)
            return;
        // random oligo finding failed - starting brute force
        negErrorsCurrentIndex = 0;
        negErrorsLeft = negErrorsNumber;
        int allOligosIndexesSize = oligosSize;
        vector<int> allOligosIndexes(oligosSize);
        for (int i=0; i<oligosSize; ++i) {
            allOligosIndexes[i] = i;
        }
        while (negErrorsLeft > 0) {
            int randomIndex = rand() % allOligosIndexesSize;
            negErrorsIndexes[negErrorsCurrentIndex++] = allOligosIndexes[randomIndex];
            allOligosIndexes.erase(allOligosIndexes.begin() + randomIndex);
            --negErrorsLeft;
        }
        return;
    }

    template<typename Type>
    inline static void vec_sort(vector<Type>& vec) {
        sort(vec.begin(), vec.end(), [](const Type& a, const Type& b) { return a < b; });
    }

    inline static void vecs_sort(vector<OligoWithLocation>& vecs) {
        sort(vecs.begin(), vecs.end(), [](const OligoWithLocation& a, const OligoWithLocation& b) { return a.oligo < b.oligo; });
    }

    inline static void vecs_sort_newOrder(vector<OligoWithLocationWithOrder>& vecs) {
        sort(vecs.begin(), vecs.end(), [](const OligoWithLocationWithOrder& a, const OligoWithLocationWithOrder& b) { return a.indexOrder < b.indexOrder; });
    }

    void applyNegativeErrors(vector<int>& negErrorsIndexes) {
        int negErrorsIndexesSize = negErrorsIndexes.size();
        while (negErrorsIndexesSize > 0) {
            oligos.erase(oligos.begin() + negErrorsIndexes.back());
            locations.erase(locations.begin() + negErrorsIndexes.back());
            negErrorsIndexes.pop_back();
            --negErrorsIndexesSize;
        }
        oligosSize = oligos.size();
    }

    void applyPositiveErrors(vector<string>& posErrors) {
        const int posErrorsSize = posErrors.size();
        vector<Location> randomLocations(posErrorsSize);
        for (int i=0; i<posErrorsSize; ++i) {
            randomLocations[i] = locator.getLocation(rand() % oligosSize, LOCATION_RANDOM_TYPE);
        }
        oligos.insert(oligos.end(), posErrors.begin(), posErrors.end());
        locations.insert(locations.end(), randomLocations.begin(), randomLocations.end());
        oligosSize = oligos.size();
    }

    void generateErrors() {
        // calculating exact number of positive and negative errors
        int spectrumSize = n - k + 1;
        int posErrorsNumber = (int)((float)POSITIVE_ERRORS_PERCENTAGE / 100.0 * (float)spectrumSize + 0.5);
        int negErrorsNumber = (int)((float)NEGATIVE_ERRORS_PERCENTAGE / 100.0 * (float)spectrumSize + 0.5);

        // generating positive errors and store them in vector for now
        vector<string> posErrors(posErrorsNumber, string(k, 'A'));
        generatePositiveErrors(posErrors, posErrorsNumber);
        if (TO_PRINT & Printer::POSITIVE_ERRORS) vec_sort(posErrors);   // for printing purposes
        TO_PRINT & Printer::POSITIVE_ERRORS && Printer::printOligos("Generated oligonucleotides as positive errors (" + to_string(posErrors.size()) + ")", posErrors, "No positive errors");

        vector<int> negErrorsIndexes(negErrorsNumber);
        generateNegativeErrorsIndexes(negErrorsIndexes, negErrorsNumber);
        vec_sort(negErrorsIndexes);
        TO_PRINT & Printer::NEGATIVE_ERRORS && Printer::printNegativeErrors("Chosen oligonucleotides as negative errors (" + to_string(negErrorsIndexes.size()) + ")", negErrorsIndexes, oligos, "No negative errors");

        applyNegativeErrors(negErrorsIndexes);
        toSet();
        applyPositiveErrors(posErrors);

        vector<OligoWithLocation> oligosWithLocations(oligosSize);
        for (int i=0; i<oligosSize; ++i) {
            oligosWithLocations[i] = { oligos[i], locations[i] };
        }

        vecs_sort(oligosWithLocations);

        for (int i=0; i<oligosSize; ++i) {
            oligos[i] = oligosWithLocations[i].oligo;
            locations[i] = oligosWithLocations[i].location;
        }
        
    }

    void reversePairs() {
        reverse(oligos.begin(), oligos.end());
        reverse(locations.begin(), locations.end());
    }

    void mixPairs() {
        vector<int> allIndexesSorted(oligosSize);
        for (int i=0; i<oligosSize; ++i) {
            allIndexesSorted[i] = i;
        }

        vector<int> mixedIndexes(oligosSize);
        for (int i=0; i<oligosSize; ++i) {
            const int randomIndex = rand() % allIndexesSorted.size();
            mixedIndexes[i] = allIndexesSorted[randomIndex];
            allIndexesSorted.erase(allIndexesSorted.begin() + randomIndex);
        }

        vector<OligoWithLocationWithOrder> oligosWithLocationsWithOrder(oligosSize);
        for (int i=0; i<oligosSize; ++i) {
            oligosWithLocationsWithOrder[i] = { oligos[i], locations[i], mixedIndexes[i] };
        }

        vecs_sort_newOrder(oligosWithLocationsWithOrder);

        for (int i=0; i<oligosSize; ++i) {
            oligos[i] = oligosWithLocationsWithOrder[i].oligo;
            locations[i] = oligosWithLocationsWithOrder[i].location;
        }

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

    vector<Location>& getLocations() {
        return locations;
    }

    int getOligosSize() {
        return oligosSize;
    }

    vector<int>** getGraph() {
        return graph;
    }

    void toSet() {
        set<string> uniqueOligos;
        vector<Location> reducedLocations;
        int prevSize = 0;
        for (int i=0; i<oligos.size(); ++i) {
            uniqueOligos.insert(oligos[i]);
            if (uniqueOligos.size() > prevSize) {
                reducedLocations.push_back(locations[i]);
                ++prevSize;
            }
        }
        oligos.assign(uniqueOligos.begin(), uniqueOligos.end());
        locations.assign(reducedLocations.begin(), reducedLocations.end());

        oligosSize = oligos.size();
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