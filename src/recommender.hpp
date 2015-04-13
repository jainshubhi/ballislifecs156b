#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "/usr/local/Cellar/eigen/3.2.3/include/eigen3/Eigen/Dense"

// to change between um and mu these next four lines must be changed
#define DATA_FILE  "data/mu/all.dta"
#define QUAL_FILE  "data/mu/qual.dta"
#define MOVIE_COL  0
#define USER_COL   1
#define TIME_COL   2
#define RATING_COL 3

// constants regarding data
#define DATA_SIZE  102416306
#define QUAL_SIZE  2749898
#define NUM_USERS  458293
#define NUM_MOVIES 17770

// constants regarding learning
#define NUM_FEATS  50
#define NUM_ITERS  50

using namespace std;
using namespace Eigen;

MatrixXi read_data(int rows, int cols, string filename);

class Learner {
public:
    Learner();
    ~Learner();

    void set_data(MatrixXi data);
    void set_qual(MatrixXi qual);
    void train();
    void predict();

private:
    MatrixXi data;
    MatrixXi qual;
    MatrixXf resp;

    MatrixXf U;
    MatrixXf V;

    double avg_rating;

    double[NUM_USERS] *avg_user_rating;
    double[NUM_USERS] *user_rating_count;
    double[NUM_USERS] *avg_user_date;

    double[NUM_MOVIES] *avg_movie_rating;
    double[NUM_MOVIES] *movie_ratings_count;
};
