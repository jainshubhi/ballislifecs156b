#include "rbm2.hpp"

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

    // Initialize third order tensors
    this->W = new double**[NUM_MOVIES + 1];
    this->grad = new double**[NUM_MOVIES + 1];

    for (unsigned int i = 0; i <= NUM_MOVIES; ++i) {
        this->W[i] = new double*[NUM_FACTORS + 1];
        this->grad = new double*[NUM_FACTORS + 1];
        for (unsigned int j = 0; j <= NUM_FACTORS; ++j) {
            this->W[i][j] = new double[NUM_RATINGS];
            this->grad = new double[NUM_RATINGS];
            for (unsigned int k = 0; k < NUM_RATINGS; ++k) {
                if (i == NUM_MOVIES)
                    this->W[i][j][k] = 1;
                else if (j == NUM_FACTORS)
                    this->W[i][j][k] = 1;
                else
                    this->W[i][j][k] = one_rand();
            }
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
            delete[] this->exp_data[i][j];
            delete[] this->exp_recon[i][j];
            delete[] this->grad[i][j];
        }
        delete[] this->W[i];
        delete[] this->exp_data[i];
        delete[] this->exp_recon[i];
        delete[] this->grad[i];
    }

    delete[] this->W;
    delete[] this->exp_data;
    delete[] this->exp_recon;
    delete[] this->grad;

    time(&end);
    printf("RbmLearner deinitialized. Took %.f seconds.\n", difftime(end, start));
}

// sets the data to the passed value
void RbmLearner::set_dr(DataReader * reader) {
    this->reader = reader;
}

// create V for each user
double ** RbmLearner::create_v(int user) {
    int index = this->user_offset[user],
        count = this->count_user_rating[user],
        movie, rating;
    double ** V = new double*[count + 1];
    // fill up V with movies/ratings
    for (unsigned int i = 0; i <= count; ++i) {
        // initialize array
        V[i] = new double[NUM_RATINGS + 1];
        // fill it up
        movie = this->reader->train_set[index + i][MOVIE_COL];
        rating = this->reader->train_set[index + i][RATING_COL];
        V[i][0] = movie;
        if (i != count)
            V[i][rating] = 1;
        // bias unit
        else
            for (unsigned int j = 1; j <= NUM_RATINGS; ++j)
                V[i][j] = 1;
    }
    return V;
}

// calculate h based on V (with bias component)
double* RbmLearner::h_calc(double ** V, int user) {
    double * h = new double[NUM_FACTORS + 1];
    int index = this->user_offset[user],
        count = this->count_user_rating[user];
    double sum;
    for (unsigned int i = 0 ; i <= NUM_FACTORS; ++i) {
        sum = 0;
        for (unsigned int j = index; j <= index + count; ++j) {
            for (unsigned int k = 1; k <= NUM_RATINGS; ++k) {
                // bias unit
                if (j == index + count)
                    sum += V[j - index][k];
                // bias unit when i goes beyond NUM_FACTORS & normal h_clc
                else
                    sum += V[j - index][k] * W[j][i][k];
            }
        }
        h[i] = 1.0 / (1 + exp(-1.0 * sum));
    }
    return h;
}

// Copy h
double * RbmLearner::copy_h(double * h) {
    double * copy_h = new double[NUM_FACTORS + 1];
    for (unsigned int i = 0; i <= NUM_FACTORS; ++i)
        copy_h[i] = h[i]
    return copy_h;
}

// To assign binary values for exp calculations.
void RbmLearner::threshold_h(double * h) {
    for (unsigned int i = 0; i <= NUM_FACTORS; ++i)
        if (h[i] > one_rand())
            h[i] = 1;
        else
            h[i] = 0;
}

// Find exp data
double *** exp_calc(double ** V, double * h, int user) {
    int index = this->user_offset[user],
        count = this->count_user_rating[user];
    // initialize exp
    double *** exp = new double**[NUM_MOVIES + 1];
    for (unsigned int i = 0; i <= count; ++i) {
        exp[i] = new double*[NUM_FACTORS + 1];
        for (unsigned int j = 0; j <= NUM_FACTORS; ++j) {
            exp[i][j] = new double[NUM_RATINGS];
            for (unsigned int k = 0; k < NUM_RATINGS; ++k) {
                exp[V[i][0]][j][k] = V[i][k + 1] * h[j];
            }
        }
    }
    return exp;
}

// calculate visible states
double ** RbmLearner::v_calc(double * h, int user) {
    int index = this->user_offset[user],
        count = this->count_user_rating[user];
    double num, denom, temp;
    double ** V = new double*[count + 1];
    for (unsigned int i = 0; i <= count; ++i) {
        V[i] = new double[NUM_RATINGS + 1];
        V[i][0] = index;
        for (unsigned int k = 1; k <= NUM_RATINGS; ++k) {
            num = 0;
            denom = 0;
            for (unsigned int j = 0; j <= NUM_FACTORS; ++j) {
                // bias unit (multiply by 1)
                if (j == NUM_FACTORS || i == count)
                    num += h[j];
                else
                    num += h[j] * W[index + i][j][k - 1];
            }
            for (unsigned int j = 1; j <= NUM_RATINGS; ++j) {
                temp = 0;
                for (unsigned int l = 0; l <= NUM_FACTORS; ++l) {
                    // bias unit (multiply by 1)
                    if (l == NUM_FACTORS || i == count)
                        temp += h[l]
                    else
                        temp += h[l] * W[index + i][l][k - 1];
                }
                den += exp(temp);
            }
            V[i][k] = exp(num) / den;
        }
    }
    return V;
}

// double * predict(int user) {
//     double ** V, ** new_V, * preds;
//     double * h;
//     V = create_v(user);
//     h = h_calc(V, user);
//     new_V = v_calc(h, user);
//     preds = new double[NUM_MOVIES];
//     for (unsigned int i = 0; i < NUM_MOVIES; ++i) {
//         preds[i] =
//     }
// }
// 
// double rmse(double *** exp_recon) {
//     int user, movie, rating;
//     double err, total_err, rmse, predict;
//     for (unsigned int i = 0; i < TRAIN_SIZE; ++i) {
//         user = this->reader->train_set[i][USER_COL];
//         movie = this->reader->train_set[i][MOVIE_COL];
//         rating = this->reader->train_set[i][RATING_COL];
//
//         err = rating - exp_recon
//     }
// }
