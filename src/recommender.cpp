#include "recommender.hpp"

// init learner
Learner::Learner() {
    this->avg_user_rating    = new double[NUM_USERS];
    this->user_rating_count  = new double[NUM_USERS];
    this->user_rating_norm   = new double[NUM_USERS];
    this->avg_user_date      = new double[NUM_USERS];

    this->avg_movie_rating   = new double[NUM_MOVIES];
    this->movie_rating_count = new double[NUM_MOVIES];

    this->U = new double*[NUM_USERS];
    this->V = new double*[NUM_MOVIES];

    for (unsigned int i = 0; i < NUM_USERS; ++i) {
        this->U[i] = new double[NUM_FEATS];
    }

    for (unsigned int i = 0; i < NUM_MOVIES; ++i) {
        this->V[i] = new double[NUM_FEATS];
    }
}

// deinit learner
Learner::~Learner() {
    delete this->avg_user_rating;
    delete this->user_rating_count;
    delete this->user_rating_norm;
    delete this->avg_user_date;
    delete this->avg_movie_rating;
    delete this->movie_rating_count;

    for (unsigned int i = 0; i < data.size(); ++i) {
        delete data[i];
    }

    for (unsigned int i = 0; i < NUM_USERS; ++i) {
        delete[] this->U[i];

    }

    for (unsigned int i = 0; i < NUM_MOVIES; ++i) {
        delete[] this->V[i];
    }

    delete[] this->U;
    delete[] this->V;
}

// read data from file into integer matrix and put in object
void Learner::read_data(string filename, bool is_data) {
    ifstream data(filename);
    string line;
    vector<int> current;

    while (getline(data, line)) {
        stringstream lineStream(line);
        string cell;
        current.clear();
        while (getline(lineStream, cell, ' ')) {
            current.push_back(atoi(cell.c_str()));
        }
        if (is_data) {
            DataPoint * new_point = new DataPoint(current);
            this->data.push_back(new_point);
        }
        else {
            DataPoint * new_point = new DataPoint(current);
            this->qual.push_back(new_point);
        }
    }
}

// get all user and movie counts and averages
void Learner::get_counts() {
    unsigned int user, movie, rating, date;
    for (unsigned int i = 0; i < DATA_SIZE; ++i) {
        user   = this->data[i]->user;
        movie  = this->data[i]->movie;
        rating = this->data[i]->rating;
        date   = this->data[i]->date;

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

// init U, V to small random numbers
void Learner::initialize() {
    for (unsigned int i = 0; i < NUM_FEATS; ++i) {
        for (unsigned int j = 0; i < NUM_USERS; ++j) {
            this->U[j][i] = small_rand();
        }
        for (unsigned int k = 0; i < NUM_MOVIES; ++k) {
            this->V[k][i] = small_rand();
        }
    }
}

// make predictions on a given point
double Learner::predict(int user, int movie, int date) {
    if (!(check_user(user) && check_movie(movie))) {
        printf("Invalid user or movie. User: %d, Movie: %d\n", user, movie);
    }
    return 1.0;
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

void Learner::predict_qual() {

}

int main() {
    Learner * learner = new Learner();
    learner->read_data(DATA_FILE, true);
    learner->read_data(QUAL_FILE, false);
    learner->train();
    learner->predict_qual();

    return 1;
}
