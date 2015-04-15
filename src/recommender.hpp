#include "compute.cpp"
#include "DataPoint.hpp"

using namespace std;

// class for the learner
class Learner {
public:
    Learner();
    ~Learner();

    void read_data(string filename, bool is_data);

    void get_counts();
    void initialize();

    void svd();
    void temporal();

    void train();
    double predict(int user, int movie, int date);
    double predict(DataPoint * point);
    void predict_qual();


private:
    // data
    vector<DataPoint *> data;

    // qual
    vector<DataPoint *> qual;

    // U, V
    double **U;
    double **V;

    // overall average rating
    double avg_rating;

    // user based counts and averages
    double *avg_user_rating;
    double *user_rating_count;
    double *user_rating_norm;
    double *avg_user_date;

    // movie based counts and averages
    double *avg_movie_rating;
    double *movie_rating_count;
};
