#ifndef DATAREADER_HPP
#define DATAREADER_HPP
#include "DataReader.hpp"
#endif

#ifndef COMPUTE_CPP
#define COMPUTE_CPP
#include "compute.cpp"
#endif

class RbmLearner {
public:

    RbmLearner();
    ~RbmLearner();

    void set_dr(DataReader * reader);
    double** create_V(int user);
    double* h_calc(double ** V, int user);
    double * copy_h(double * h);
    void threshold_h(double * h);
    double *** exp_calc(double ** V, double * h, int user);
    double expected_val();
    double** v_calc(double * h, int user);
    double * predict(int user);
    double rmse();
    void train();

private:
    // third order tensor
    double*** W;
    double*** grad;

    // 100 users
    int* minibatch;

    // number of movies each user has rated
    int* count_user_rating;

    // index at which user appears in training set
    int* user_offset;

    // data reader
    DataReader * reader;
};
