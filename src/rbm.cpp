#include "rbm.hpp"

int main() {
    // seed the random generator
    srand(time(NULL));

    // read data
    DataReader * reader = new DataReader;

    // read qual
    reader->read_qual();

    // init learner
    RbmLearner rbm;

    // set reader
    rbm.set_dr(reader);

    // learn parameters
    rbm.train();
    return 1;
}

RbmLearner::RbmLearner() {

    time_t start, end;
    time(&start);
    printf("Initializing RbmLearner...\n");

    // Initialize W
    this->W = new double**[NUM_MOVIES];
    this->h_u = new double*[NUM_USERS];

    for (unsigned int i = 0; i < NUM_MOVIES; ++i) {
        this->W[i] = new double*[NUM_FACTORS];
        for (unsigned int j = 0; j < NUM_FACTORS; ++j) {
            this->W[i][j] = new double[NUM_RATINGS];
            for (unsigned int k = 0; k < NUM_RATINGS; ++k) {
                this->W[i][j][k] = one_rand();
            }
        }
    }

    for (unsigned int i = 0; i < NUM_USERS; ++i) {
        this->h_u[i] = new double[NUM_FACTORS];
        for (unsigned int j = 0; j < NUM_FACTORS; ++j) {
            this->h_u[i][j] = one_rand();
        }
    }

    this->minibatch = new int[MINIBATCH_SIZE];
    this->count_user_rating = new int[NUM_USERS];
    vector_read_int(CNT_USER_RATING, this->count_user_rating, NUM_USERS);
    this->user_offset = new int[NUM_USERS];
    vector_read_int(USER_OFFSETS, this->user_offset, NUM_USERS);

    time(&end);
    printf("RbmLearner initialized. Took %.f seconds.\n", difftime(end, start));

}

RbmLearner::~RbmLearner() {
    time_t start, end;
    time(&start);
    printf("Deinitializing RbmLearner...\n");

    delete[] this->minibatch;
    delete[] this->count_user_rating;
    delete[] this->user_offset;

    for(unsigned int i = 0; i < NUM_MOVIES; ++i) {
        for (unsigned int j = 0; j < NUM_FACTORS; ++j) {
            delete[] this->W[i][j];
        }
        delete[] this->W[i];
    }

    for (unsigned int i = 0; i < NUM_USERS; ++i) {
        delete[] this->h_u[i];
    }

    delete[] this->W;
    delete[] this->h_u;

    time(&end);
    printf("RbmLearner deinitialized. Took %.f seconds.\n", difftime(end, start));
}

// sets the data to the passed value
void RbmLearner::set_dr(DataReader * reader) {
    this->reader = reader;
}

// sum over features
double RbmLearner::sum_over_features(int movie, int rating, double* h) {
    double total = 0;
    for (unsigned int i = 0; i < NUM_FACTORS; ++i) {
        // 0-indexed for rating
        total += h[i] * this->W[movie][i][rating - 1];
    }
    return total;
}

// expected value for each user
double** RbmLearner::p_calc_v(double** V, double* h, int user) {
    int index = this->user_offset[user],
        count = this->count_user_rating[user],
        movie;
    double numer, denom, e_value = 0;
    // determine most likely
    for (unsigned int i = 0; i < count; ++i) {
        denom = 0;
        movie = this->reader->train_set[index + i][MOVIE_COL];
        for (unsigned int j = 1; j <= NUM_RATINGS; ++j) {
            numer = exp(sum_over_features(movie, j, h));
            for (unsigned int k = 1; k <= NUM_RATINGS; ++k) {
                denom += exp(sum_over_features(movie, k, h));
            }
            V[i][j] = numer / denom;
        }
    }
    double sum = 0;
    for (unsigned int i = 0; i < count; ++i) {
        for (unsigned int j = 1; j <= NUM_RATINGS; ++j) {
            sum += V[i][j];
        }
        sum = 0;
    }
    // they all sum 0.45/0.46. None of them add to 1 like they should.

    // stored as movie count x 2 array
    double**v = new double*[count];
    for (unsigned int i = 0; i < count; ++i) {
        v[i] = new double[2];
        v[i][0] = V[i][0];
        e_value = V[i][1] + (2*V[i][2]) + (3*V[i][3])+ (4*V[i][4])+ (5*V[i][5]);
        v[i][1] = e_value;
    }
    return v;
}

// update v
void RbmLearner::update_V(double** V, double** v, int user) {
    int count = this->count_user_rating[user];
    for(unsigned int i = 0; i < count; ++i) {
        V[i][1] = bound(v[i][1]);
    }
}

// create V
double** RbmLearner::create_v(int user) {
    int index = this->user_offset[user],
        count = this->count_user_rating[user],
        movie, rating;
    double** V = new double*[count];
    // fill up V with movies/ratings
    for (unsigned int i = 0; i < count; ++i) {
        // initialize array
        V[i] = new double[NUM_RATINGS + 1];
        // fill it up
        movie = this->reader->train_set[index + i][MOVIE_COL];
        rating = this->reader->train_set[index + i][RATING_COL];
        V[i][0] = movie;
        V[i][rating] = 1;
    }
    return V;
}

// fill up h with appropriate weight probabilities for each user
// create is an integer which is 0 if this method is called after create_v and 1
// otherwise
double* RbmLearner::p_calc_h(double** V, int user, int create) {
    double* h = new double[NUM_FACTORS];
    int term, movie, rating, count = this->count_user_rating[user];
    for (unsigned int i = 0; i < NUM_FACTORS; ++i) {
        term = 0;
        for (unsigned int j = 0; j < count; ++j) {
            movie = V[j][0];
            if (create == 0) {
                for (unsigned int k = 1; k <= NUM_RATINGS; ++k) {
                    if (V[j][k] == 1) {
                        rating = V[j][k];
                        break;
                    }
                }
            }
            else {
                rating = V[j][1];
            }
            term += this->W[movie][i][rating - 1];
        }
        h[i] = 1/(1 + exp(-1 * term));
    }
    return h;
}

// update h for each user
void RbmLearner::update_h(double* h, int user, bool last, double threshold) {
    // update h
    if (!last) {
        for (unsigned int i = 0; i < NUM_FACTORS; ++i) {
            if (h[i] > threshold)
                h[i] = 1;
            else
                h[i] = 0;
        }
    }
}

void RbmLearner::create_minibatch() {
    for (unsigned int i = 0; i < MINIBATCH_SIZE; ++i) {
        minibatch[i] = i;
    }
}

void RbmLearner::update_W() {
    double** V;
    double **v;
    int user, size, rating;
    // create third order tensors
    double*** exp_data = new double**[NUM_MOVIES];
    double*** exp_recon = new double**[NUM_MOVIES];
    for (unsigned int i = 0; i < NUM_MOVIES; ++i) {
        exp_data[i] = new double*[NUM_FACTORS];
        exp_recon[i] = new double*[NUM_FACTORS];
        for (unsigned int j = 0; j < NUM_FACTORS; ++j) {
            exp_data[i][j] = new double[NUM_RATINGS];
            exp_recon[i][j] = new double[NUM_RATINGS];
        }
    }
    // do the stuff
    for (unsigned int i = 0; i < MINIBATCH_SIZE; ++i) {
        user = this->minibatch[i];
        V = create_v(user);
        this->h_u[user] = p_calc_h(V, user, 0);
        update_h(this->h_u[user], user, false, one_rand());
        size = this->count_user_rating[user];
        for (unsigned int j = 0; j < size; ++j) {
            for (unsigned int k = 0; k < NUM_FACTORS; ++k) {
                for (unsigned int l = 1; l <= NUM_RATINGS; ++l) {
                    if (V[j][l] == 1) {
                        rating = l;
                        break;
                    }
                }
                exp_data[(int)V[j][0]][k][rating - 1] += this->h_u[user][k];
            }
        }
        // do it again for exp_recon
        v = p_calc_v(V, this->h_u[user], user);
        update_V(V, v, user);
        this->h_u[user] = p_calc_h(V, user, 1);
        update_h(this->h_u[user], user, false, one_rand());
        for (unsigned int j = 0; j < size; ++j) {
            for (unsigned int k = 0; k < NUM_FACTORS; ++k) {
                exp_recon[(int)V[j][0]][k][(int)V[j][1] - 1] += this->h_u[user][k];
            }
        }
    }
    // update all h's
    for (unsigned int i = 0; i < NUM_USERS; ++i) {
        update_h(this->h_u[i], i, false, one_rand());
    }

    // update W
    matrix_add(exp_data, exp_recon, NUM_MOVIES, NUM_FACTORS, NUM_RATINGS, -1);
    matrix_scalar_mult(exp_data, (LEARNING_RATE / size), NUM_MOVIES, NUM_FACTORS, NUM_RATINGS);
    matrix_add(W, exp_data, NUM_MOVIES, NUM_FACTORS, NUM_RATINGS, 1);

    // delete all pointer arrays
    for (unsigned int i = 0; i < size; ++i) {
        delete[] v[i];
        delete[] V[i];
    }

    for(unsigned int i = 0; i < NUM_MOVIES; ++i) {
        for (unsigned int j = 0; j < NUM_FACTORS; ++j) {
            delete[] exp_data[i][j];
            delete[] exp_recon[i][j];
        }
        delete[] exp_data[i];
        delete[] exp_recon[i];
    }

    delete[] exp_data;
    delete[] exp_recon;
    delete[] v;
    delete[] V;

}

void RbmLearner::train() {
    int user, rating, movie, train_count;
    double predict, err, train_err, numer, denom;
    time_t start, end;

    ofstream out_file;
    out_file.open("./rbm_train.txt");

    for (unsigned int i = 0; i < RBM_EPOCHS; ++i) {
        time(&start);
        printf("Epoch Number: %d.\n", i);
        create_minibatch();
        update_W();
        train_err = 0;
        train_count = 0;
        int size = user_offset[NUM_USERS - 1] + count_user_rating[NUM_USERS - 1];

        // how good are we doing yo
        if (i % 2  == 0) {
            for(unsigned int j = 0; j < size; ++j) {
                user = this->reader->train_set[j][USER_COL];
                movie = this->reader->train_set[j][MOVIE_COL];
                rating = this->reader->train_set[j][RATING_COL];
                predict = 0;
                numer = 0;
                denom = 0;

                for (unsigned int k = 1; k <= NUM_RATINGS; ++k) {
                    numer = exp(sum_over_features(movie, k, this->h_u[user]));
                    for (unsigned int l = 1; l <= NUM_RATINGS; ++l) {
                        denom += exp(sum_over_features(movie, l, this->h_u[user]));
                    }
                    predict += (numer / denom) * k;
                    out_file << predict;
                    out_file << "\n";
                }

                predict = bound(predict);

                err = (double)rating - predict;

                train_err += err * err;

                if (j % 10000 == 0)
                    printf("Training error: %f. Index: %d.\n", train_err, j);

                train_count++;
            }

            out_file.close();
            time(&end);
            printf("Train Error: %f. Took %.f seconds.\n",
                sqrt(train_err / ((double) size)), difftime(end, start));
        } else {
            time(&end);
            printf("Took %.f seconds.\n", difftime(end, start));
        }
    }
}
