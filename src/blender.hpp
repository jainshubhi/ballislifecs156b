#ifndef COMPUTE_CPP
#define COMPUTE_CPP
#include "compute.cpp"
#endif

using namespace std;

/*
 * TODO:
 * write data readers for all these types to get their constants
 * write prediction functions for all to make predictions to use later
 */

class Svd {
public:
    // init the svd model
    Svd(string filename) {
        // read parameters into file
    }

    // deinit the svd model
    ~Svd() {

    }

    // predict on a vector of points TODO no longer DataPoint
    vector<double> predict(int ** points) {
        vector<double> resp;
        for (unsigned int i = 0; i < QUAL_SIZE; ++i) {
            // TODO make prediction
        }
        return resp;
    }

private:
    // nothing
};

class Temporal {
public:
    // init the temporal model
    Temporal(string filename) {

    }

    // deinit the temporal model
    ~Temporal() {

    }

    // predict on a vector of points TODO no longer DataPoint
    vector<double> predict(int ** points) {
        vector<double> resp;
        for (unsigned int i = 0; i < QUAL_SIZE; ++i) {
            // TODO make prediction
        }
        return resp;
    }

private:
    // nothing
};

class Knn {
public:
    // init the knn model
    Knn(string filename) {

    }

    // deinit the knn model
    ~Knn() {

    }

    // predict on a vector of points TODO no longer DataPoint
    vector<double> predict(int ** points) {
        vector<double> resp;
        for (unsigned int i = 0; i < QUAL_SIZE; ++i) {
            // TODO make prediction
        }
        return resp;
    }

private:
    // nothing
};
