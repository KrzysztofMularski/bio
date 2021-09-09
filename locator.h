#pragma once

#include <random>
#include <time.h>
#include "additives.h"

class Locator {
private:
    std::default_random_engine generator;

public:

    static const int LINEAR = 0;
    static const int GAUSSIAN = 1;

    Locator() {
        srand(time(NULL));
    }

    ~Locator() {}

    Location getLocation(int index, int isGaussian = 0) {
        int loc_range = LOCATION_RANGE < n-1 ? LOCATION_RANGE : n-1;
        int half_loc_range = (loc_range + 1) / 2;

        int number = 0;
        if (isGaussian) {
            number += getGaussianRandom(loc_range, index);
        } else {
            number += getLinearRandom(loc_range) + index;
        }
        int left = number - half_loc_range;
        int right = number + half_loc_range;
        
        if (right - left > loc_range)
            --right;
        if (left < 0) {     // [-2, 1] => [0, 3]
            int diff = 0 - left;
            left = 0;
            right += diff;
        }
        if (right > n-1) {  // n-1=3; [2, 5] => [0, 3]
            int diff = right - (n-1);
            left -= diff;
            right = n-1;
        }
        // [45, 55]
        // if (right < 52)  // 46
        // if (left > 48)   // 54
        // [47, 53]
        // if (right < 51)  // 47
        // if (left > 48)   // 52
        // if (number == 47)
            // std::cout << "'" << number << "' " << index << ": [" << left << ", " << right << "]" << std::endl;
        return { left, right };
    }

    // void test(int index, int isGaussian = 0) {
    //     Location result = getLocation(index, isGaussian);
    // }

    // int main()
    // {
    //     
        
    //     std::cout << "loc_range=" << LOCATION_RANGE << " n=" << n << std::endl;
        
    //     // std::cout << "Linear: " << std::endl;
    //     // for (int i=0; i<10; ++i) {
    //     //     test(2);
    //     // }

    //     std::cout << "Gaussian: " << std::endl;

    //     for (int i=0; i<1000; ++i) {
    //         test(50, 1);
    //     }
        
    // }

    int getGaussianRandom(int loc_range, int index) {
        double mean = index;
        double stddev = loc_range / 6;
        std::normal_distribution<double> distribution(mean, stddev);
        int left = index - loc_range / 2;
        int right = index + loc_range / 2;
        double number;
        while(true) {
            number = distribution(generator);
            if (number >= left && number <= right) {
                return int(number + 0.5);
            }
        }
    }

    static int getLinearRandom(int loc_range) {
        return rand() % loc_range;
    }

};
