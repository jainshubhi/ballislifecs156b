#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#ifndef DATAREADER_HPP
#define DATAREADER_HPP
#include "DataReader.hpp"
#endif

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP
#include "constants.hpp"
#endif

using namespace std;

// returns random double between -0.1 and 0.1
double small_rand() {
    int num = ((int) rand()) % 100;
    return 2.0 * num / 10000.0 - 0.01;
}

// returns +1 if positve, -1 if negative, 0 if zero
int sign(double x) {
    if (x < 0) return -1;
    if (x > 0) return 1;
    return 0;
}

// calculates time deviation cost
double dev_ut(int date, double avg_date) {
    return sign(date - avg_date) * pow(abs(date - avg_date), BETA);
}

// bounds the prediction to between 1 and 5
double bound(double x) {
    if (x > 5) return 5;
    if (x < 1) return 1;
    return x;
}

bool check_user(int user) {
    if ((user > 0) && (user <= NUM_USERS)) {
        return true;
    }
    return false;
}

bool check_movie(int movie) {
    if ((movie > 0) && (movie <= NUM_MOVIES)) {
        return true;
    }
    return false;
}
