#ifndef DATAREADER_HPP
#define DATAREADER_HPP
#include "DataReader.hpp"
#include "compute.cpp"
#endif

class SvdLearner {
public:
    SvdLearner();
    ~SvdLearner();

    void svd(double lambda, double limit);

private:
    double** U; // 2 dimensional array NUM_USERS * NUM_FEATS
    double** V; // 2 dimensional array NUM_MOVIES * NUM_FEATS
    double** Y; // 2 dimensional array NUM_USERS * NUM_MOVIES
    vector<DataPoint *> data; // complete data set
};
