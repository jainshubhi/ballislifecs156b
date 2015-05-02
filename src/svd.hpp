#ifndef DATAREADER_HPP
#define DATAREADER_HPP
#include "DataReader.hpp"
#endif

#ifndef COMPUTE_CPP
#define COMPUTE_CPP
#include "compute.cpp"
#endif

class SvdLearner {
public:
    SvdLearner();
    ~SvdLearner();

    void set_dr(DataReader * reader);

    void get_implicit_c(unsigned int user, double n);
    void update_implicit_features(unsigned int user, double n);
    void update_features(unsigned int user, unsigned int movie, double err);
    void update_baselines(unsigned int user, unsigned int movie, double err,
        unsigned int item_bin, int user_date_count, double dev);

    void train();
    void write();
    void pred_blend();
    void pred();

private:
    // learning rates
    double gamma1 = GAMMA_1;
    double gamma2 = GAMMA_2;

    // user and movie latent factors
    double ** U;
    double ** V;

    // movies and dates that each user has watched
    int ** user_movies;
    int ** user_dates;

    // user averages and counts
    double * user_bias;
    int * count_user_rating;
    double * avg_user_date;

    // movie averages and counts
    double * movie_bias;
    int * count_movie_rating;

    // user alphas for time deviatoin
    double * user_alpha;

    // user time biases
    double ** user_time_bias;

    // user c for time bins
    double * user_c;

    // c_u(t) for each user
    //TODO figure out where this fits in (also need to allocate it)
    double ** user_time_c;

    // TODO figure out what these are
    double ** item_bin_bias;
    double ** implicit_features;

    // user to decrease computation time
    double * implicit_c;
    double * temp_implicit_c;

    // data reader
    DataReader * reader;
};
