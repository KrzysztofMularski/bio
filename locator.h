#pragma once

#include <random>
#include <time.h>
#include "additives.h"

class Locator {
private:
    std::default_random_engine generator;

public:

    Locator() {}

    ~Locator() {}

    Location getLocation(int index, Random_Type r_type = Random_Type::LINEAR) {
        int loc_range = LOCATION_RANGE-1 < n-1 ? LOCATION_RANGE-1 : n-1;
        int half_loc_range = (loc_range + 1) / 2;

        if (loc_range == 0)
            return { index, index };

        int number = 0;
        if (r_type == Random_Type::GAUSSIAN) {
            number += getGaussianRandom(loc_range, index);
        } else {
            number += getLinearRandom(loc_range+1) + index - half_loc_range + 1;
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

        if (left > index) {
            int diff = left - index;
            left -= diff;
            right -= diff;
        }

        if (right < index) {
            int diff = index - right;
            left += diff;
            right += diff;
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
