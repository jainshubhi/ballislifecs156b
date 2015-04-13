#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "/usr/local/Cellar/eigen/3.2.3/include/eigen3/Eigen/Dense"

#define DATA_FILE  "data/mu/all.dta"
#define QUAL_FILE  "data/mu/qual.dta"
#define DATA_SIZE  102416306
#define QUAL_SIZE  2749898
#define NUM_FEATS  50
#define NUM_ITERS  50
#define NUM_USERS  458293 + 1
#define NUM_MOVIES 17770 + 1

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
};
