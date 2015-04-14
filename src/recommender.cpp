#include "recommender.hpp"

// read data from file into integer matrix
MatrixXi read_data(int rows, int cols, string filename) {
    ifstream data(filename);
    MatrixXi vals(rows, cols);
    string line;
    int row = 0, col = 0;

    while (getline(data, line)) {
        stringstream lineStream(line);
        string cell;
        while (getline(lineStream, cell, ' ')) {
            vals(row, col) = atoi(cell.c_str());
            ++col;
        }
        ++row;
        col = 0;
    }
    return vals;
}

// init learner
Learner::Learner() {
    this->avg_user_rating    = new double[NUM_USERS];
    this->user_rating_count  = new double[NUM_USERS];
    this->user_rating_norm   = new double[NUM_USERS];
    this->avg_user_date      = new double[NUM_USERS];

    this->avg_movie_rating   = new double[NUM_MOVIES];
    this->movie_rating_count = new double[NUM_MOVIES];
}

// deinit learner
Learner::~Learner() {
    delete this->avg_user_rating;
    delete this->user_rating_count;
    delete this->user_rating_norm;
    delete this->avg_user_date;
    delete this->avg_movie_rating;
    delete this->movie_rating_count;
}

// set data matrix
void Learner::set_data(MatrixXi data) {
    this->data = data;
}

// set qual matrix
void Learner::set_qual(MatrixXi qual) {
    this->qual = qual;
}

// get all user and movie counts and averages
void Learner::get_counts() {
    unsigned int user, movie, rating, date;
    for (unsigned int i = 0; i < DATA_SIZE; ++i) {
        user   = this->data(i, USER_COL);
        movie  = this->data(i, MOVIE_COL);
        rating = this->data(i, RATING_COL);
        date   = this->data(i, TIME_COL);

        this->avg_rating += rating;

        this->avg_user_rating[user] += rating;
        this->avg_user_date[user] += date;
        this->user_rating_count[user]++;

        this->avg_movie_rating[movie] += rating;
        this->user_rating_count[movie]++;
    }

    this->avg_rating /= DATA_SIZE;

    for (unsigned int i = 0; i < NUM_USERS; ++i) {
        this->avg_user_rating[i] /= this->user_rating_count[i];
        this->avg_user_date[i] /= this->user_rating_count[i];
        this->user_rating_norm[i] = pow(this->user_rating_count[i], -0.5);
    }

    for (unsigned int i = 0; i < NUM_MOVIES; ++i) {
        this->avg_movie_rating[i] /= this->movie_rating_count[i];
    }
}

// init U, V
void Learner::initialize() {
    for (unsigned int i = 0; i < NUM_FEATS; ++i) {
        for (unsigned int j = 0; i < NUM_USERS; ++j) {
            this->U(i, j) = small_rand();
        }
        for (unsigned int k = 0; i < NUM_MOVIES; ++k) {
            this->V(i, k) = small_rand();
        }
    }
}

// train U, V using SVD
// TODO shubhi
void Learner::svd() {

}

// train the temporal model parameters using SGD
// TODO dylan
void Learner::temporal() {

}

// train the full model
void Learner::train() {
    this->svd();
    this->temporal();
}

// make predictions on the qual data set
void Learner::predict() {
    for (unsigned int i = 0; i < QUAL_SIZE; ++i) {
        // prediction function
    }
}

int main() {
    Learner * learner = new Learner();
    learner->set_data(read_data(DATA_SIZE, 4, DATA_FILE));
    learner->set_qual(read_data(QUAL_SIZE, 3, QUAL_FILE));
    learner->train();
    learner->predict();

    return 1;
}
