#ifndef DATAREADER_HPP
#define DATAREADER_HPP
#include "DataReader.hpp"
#endif

#ifndef COMPUTE_CPP
#define COMPUTE_CPP
#include "compute.cpp"
#endif

class RbmLearner2 {
public:

    RbmLearner2();
    ~RbmLearner2();

    void set_dr(DataReader * reader);
    double** create_v(int user);
    double* h_calc(double ** V, int user);
    double * copy_h(double * h);
    void threshold_h(double * h);
    double *** exp_calc(double ** V, double * h, int user);
    double expected_val();
    double** v_calc(double * h, int user);
    void create_minibatch();
    double predict(double * h, int movie);
    void train();
    void pred(string predictions, bool is_qual, bool write);

private:
    // third order tensor
    double *** W;
    // gradients
    double *** grad;
    // gradient for B
    double ** B_grad;
    // gradient for h_b
    double * b_grad;

    // global bias for visible states
    double ** B_bias;
    // global bias for hidden states
    double * b_bias;

    // number of movies each user has rated
    int* count_user_rating;

    // index at which user appears in training set
    int* user_offset;

    // minibatch
    int * minibatch;

    // data reader
    DataReader * reader;
};
