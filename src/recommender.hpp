#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "/usr/local/Cellar/eigen/3.2.3/include/eigen3/Eigen/Dense"

#define DATA_FILE    "data/mu/all.dta"
#define QUAL_FILE    "data/mu/qual.dta"
#define DATA_SIZE    102416306
#define QUAL_SIZE    2749898
#define NUM_FEATS    50
#define NUM_ITERS    50
#define NUM_USERS    1
#define NUM_MOVIES   1

using namespace std;
using namespace Eigen;

typedef Matrix<int,   DATA_SIZE, 4>          DataMatrix;
typedef Matrix<int,   QUAL_SIZE, 3>          QualMatrix;
typedef Matrix<float, QUAL_SIZE, 1>          RespVector;
typedef Matrix<float, NUM_USERS, NUM_FEATS>  UVector;
typedef Matrix<float, NUM_FEATS, NUM_MOVIES> VVector;

MatrixXd read_data(int rows, int cols);

class Learner {
public:
    Learner();
    ~Learner();

    void train();
    void test();

private:
    DataMatrix data;
    QualMatrix qual;
    RespVector resp;
    UVector U;
    VVector V;
};
