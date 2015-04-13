#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include "/usr/local/Cellar/eigen/3.2.3/include/eigen3/Eigen/Dense"
#include "constants.hpp"

using namespace std;

double small_rand() {
    int num = ((int) rand()) % 100;
    return 2.0 * num / 10000.0 - 0.01;
}

int sign(double x) {
    if (x < 0) return -1;
    if (x > 0) return 1;
    return 0;
}

double dev_ut(int date, double avg_date) {
    return sign(date - avg_date) * pow(abs(date - avg_date), BETA);
}

double user_k(int rating_count) {
    return pow(rating_count, SPLINE_POWER);
}
