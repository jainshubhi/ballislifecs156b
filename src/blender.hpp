#ifndef COMPUTE_CPP
#define COMPUTE_CPP
#include "compute.cpp"
#endif

/*
 * TODO:
 * write data readers for all these types to get their constants
 * write prediction functions for all to make predictions to use later
 */

class Svd {
public:
    Svd() {

    }

    ~Svd() {

    }

    double predict(int user, int movie, int date) {
        return 1.0;
    }

private:
    // nothing
};

class Temporal {
public:
    Temporal() {

    }

    ~Temporal() {

    }

    double predict(int user, int movie, int date) {
        return 1.0;
    }

private:
    // nothing
};

class Knn {
public:
    Knn() {

    }

    ~Knn() {

    }

    double predict(int user, int movie, int date) {
        return 1.0;
    }

private:
    // nothing
};
