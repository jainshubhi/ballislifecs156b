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
    double sum_over_features(int movie, int rating, double* h);
    double** p_calc_v(int** V, double* h, int user);
    void update_V(int** V, double** v, int user);
    int** create_v(int user);
    double* p_calc_h(int** V, int user);
    void update_h(double* h, int user, bool last, double threshold);
    void create_minibatch();
    void update_W();
    void train();

private:
    // third order tensor
    double*** W;

    // hidden states for each user
    double** h_u;

    // 100 users
    int* minibatch;

    // number of movies each user has rated
    int* count_user_rating;

    // index at which user appears in training set
    int* user_offset;

    // data reader
    DataReader * reader;
};
