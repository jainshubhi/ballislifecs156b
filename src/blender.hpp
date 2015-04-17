#ifndef COMPUTE_CPP
#define COMPUTE_CPP
#include "compute.cpp"
#endif

#ifndef DATAPOINT_HPP
#define DATAPOINT_HPP
#include "DataPoint.hpp"
#endif

using namespace std;

/*
 * TODO:
 * write data readers for all these types to get their constants
 * write prediction functions for all to make predictions to use later
 */

class Svd {
public:
    Svd(string filename) {

    }

    ~Svd() {

    }

    vector<double> predict(vector<DataPoint *> points) {
        vector<double> resp;
        for (unsigned int i = 0; i < points.size(); ++i) {
            // TODO make prediction
        }
        return resp;
    }

private:
    // nothing
};

class Temporal {
public:
    Temporal(string filename) {

    }

    ~Temporal() {

    }

    vector<double> predict(vector<DataPoint *> points) {
        vector<double> resp;
        for (unsigned int i = 0; i < points.size(); ++i) {
            // TODO make prediction
        }
        return resp;
    }

private:
    // nothing
};

class Knn {
public:
    Knn(string filename) {

    }

    ~Knn() {

    }

    vector<double> predict(vector<DataPoint *> points) {
        vector<double> resp;
        for (unsigned int i = 0; i < points.size(); ++i) {
            // TODO make prediction
        }
        return resp;
    }

private:
    // nothing
};
