#include "compute.cpp"

using namespace std;
using namespace Eigen;

MatrixXi read_data(int rows, int cols, string filename);

// Learner class
class Learner {
public:
    Learner();
    ~Learner();

    void set_data(MatrixXi data);
    void set_qual(MatrixXi qual);

    void get_counts();
    void initialize();
    void svd();
    void temporal();


    void svd();
    void temporal();
    void train();
    void predict();

private:
    MatrixXi data;
    MatrixXi qual;
    MatrixXf resp;

    MatrixXf U;
    MatrixXf V;

    double avg_rating;

    double *avg_user_rating;
    double *user_rating_count;
    double *user_rating_norm;
    double *avg_user_date;

    double *avg_movie_rating;
    double *movie_rating_count;
};
