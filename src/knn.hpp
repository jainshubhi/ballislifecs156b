#ifndef DATAREADER_HPP
#define DATAREADER_HPP
#include "DataReader.hpp"
#endif

#ifndef COMPUTE_CPP
#define COMPUTE_CPP
#include "compute.cpp"
#endif

class KnnLearner {
public:
    KnnLearner();
    ~KnnLearner();

    void set_dr(DataReader * reader);
    void compute_similarity_coef();
    vector <int> compute_U(int i, int j);
    void fill_ratings();
    void sort_neighbors(int user, int movie);
    void train();
    void pred();

private:
    // similarity coefficient
    double ** s;

    // movies each user has watched
    int ** ratings;
    // int ** ratings_2;

    // neighbors for each item/user
    int * N;

    // similarity coefs for one user
    int * sim;

    // data reader
    DataReader * reader;
};
