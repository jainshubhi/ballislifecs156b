#ifndef DATAREADER_HPP
#define DATAREADER_HPP
#include "DataReader.hpp"
#endif

#ifndef COMPUTE_CPP
#define COMPUTE_CPP
#include "compute.cpp"
#endif

#include <map>

class KnnLearner {
public:
    KnnLearner();
    ~KnnLearner();

    void set_dr(DataReader * reader);
    void compute_similarity_coef();
    vector <int> compute_U(int i, int j);
    void sort_neighbors(int user, int movie);
    void train();
    void pred();

private:
    // similarity coefficient matrix for all movies
    double ** s;

    // rating for each user/movie combination
    // int ** ratings;
    // int ** ratings_2;
    map<pair<int,int>,int> ratings_map;

    // neighbors for each item/user
    int * N;

    // similarity coefs for one user
    int * sim;

    // movie count
    // int * count_movie_rating;

    // data reader
    DataReader * reader;
};
