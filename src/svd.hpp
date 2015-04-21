#ifndef DATAREADER_HPP
#define DATAREADER_HPP
#include "DataReader.hpp"
#include "compute.cpp"
#endif

class SvdLearner {
public:
    SvdLearner();
    ~SvdLearner();

    void set_data(vector<DataPoint *> data);
    void svd(double lambda, int i, int j, double le_dot);
    void init(double limit);
    void train(double norm_val);
    void write(string filename);

private:
    double ** U; // 2 dimensional array NUM_USERS * NUM_FEATS
    double ** V; // 2 dimensional array NUM_MOVIES * NUM_FEATS

    double ** gradU;
    double ** gradV;

    vector<DataPoint *> data; // complete data set
};
