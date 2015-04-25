#include "svd.hpp"

int main() {
    // read data
    DataReader * reader = new DataReader;

    // learn parameters
    SvdLearner svd;

    // +++++++++++++++++++++++++
    svd.set_data(reader->train_set);
    // svd.set_data(reader->blend_set);
    // +++++++++++++++++++++++++
    svd.train();

    // write parameters to files
    svd.write();

    return 1;
}

// Constructor
SvdLearner::SvdLearner() {

    time_t start, end;
    time(&start);
    printf("Initializing SvdLearner...\n");

    // initialize all biases
    this->user_bias = new double[NUM_USERS];
    this->count_user_rating = new int[NUM_USERS];
    this->avg_user_date = new double[NUM_USERS];

    this->movie_bias = new double[NUM_MOVIES];
    this->count_movie_rating = new int[NUM_MOVIES];

    // to speed up computaions store implicit contributions
    this->implicit_c = new double[NUM_FEATS];
    this->temp_implicit_c = new double[NUM_FEATS];

    // contains time deviation parameters
    this->user_c = new double[NUM_USERS];
    this->user_alpha = new double[NUM_USERS];

    vector_read(AVG_USER_RATING, this->user_bias, NUM_USERS);
    vector_read_int(CNT_USER_RATING, this->count_user_rating, NUM_USERS);
    vector_read(AVG_USER_DATE, this->avg_user_date, NUM_USERS);
    vector_read(AVG_MOVIE_RATING, this->movie_bias, NUM_MOVIES);
    vector_read_int(CNT_MOVIE_RATING, this->count_movie_rating, NUM_MOVIES);

    // initialize U, V with small random numbers
    // initialize user_movies, user_dates
    this->U = new double*[NUM_USERS];
    this->V = new double*[NUM_MOVIES];

    // to speed up computations store implicit movie contributions
    this->implicit_features = new double*[NUM_MOVIES];
    this->user_movies = new int*[NUM_USERS];
    this->user_dates = new int*[NUM_USERS];

    // time bias variables
    this->user_time_bias = new double*[NUM_USERS];
    this->user_time_c = new double*[NUM_USERS];
    this->item_bin_bias = new double*[NUM_MOVIES];

    for (unsigned int i = 0; i < NUM_USERS; ++i) {
        this->U[i] = new double[NUM_FEATS];
        this->user_movies[i] = new int[this->count_user_rating[i]];
        this->user_dates[i] = new int[this->count_user_rating[i]];
        this->user_time_bias[i] = new double[this->count_user_rating[i]];
        this->user_time_c[i] = new double[this->count_user_rating[i]];

        for (unsigned int j = 0; j < NUM_FEATS; ++j) {
            this->U[i][j] = small_rand();
        }

        for (unsigned int j = 0; j < this->count_user_rating[i]; ++j) {
            this->user_time_bias[i][j] = small_pos_rand();
        }

        for (unsigned int j = 0; j < this->count_user_rating[i]; ++j) {
            this->user_time_c[i][j] = small_pos_rand();
        }
    }

    for (unsigned int i = 0; i < NUM_MOVIES; ++i) {
        this->V[i] = new double[NUM_FEATS];
        this->implicit_features[i] = new double[NUM_FEATS];
        this->item_bin_bias[i] = new double[TIME_BINS];

        for (unsigned int j = 0; j < NUM_FEATS; ++j) {
            this->U[i][j] = small_rand();
            this->implicit_features[i][j] = 0;
        }

        for (unsigned int j = 0; j < TIME_BINS; ++j) {
            this->item_bin_bias[i][j] = small_pos_rand();
        }
    }

    matrix_read_int(USER_MOVIES, this->user_movies, NUM_USERS, NUM_MOVIES);
    matrix_read_int(USER_DATES, this->user_dates, NUM_USERS, NUM_MOVIES);

    time(&end);
    printf("SvdLearner initialized. Took %.f seconds.\n", difftime(end, start));
}

// Destructor
SvdLearner::~SvdLearner() {
    // Delete U
    for (unsigned int i = 0; i < NUM_USERS; ++i) {
        delete[] this->U[i];
        delete[] this->user_movies[i];
        delete[] this->user_dates[i];
    }

    // Delete V
    for (unsigned int i = 0; i < NUM_MOVIES; ++i) {
        delete[] this->V[i];
    }

    delete[] this->U;
    delete[] this->V;

    delete[] this->user_movies;
    delete[] this->user_dates;

    delete[] this->user_bias;
    delete[] this->count_user_rating;
    delete[] this->avg_user_date;
    delete[] this->movie_bias;
    delete[] this->count_movie_rating;

    delete[] this->implicit_c;
    delete[] this->temp_implicit_c;

    delete[] this->user_c;
    delete[] this->user_alpha;
}

// sets the data to the passed value
void SvdLearner::set_data(int ** data) {
    this->data = data;
}

// Calculate once per user, the sum of his implicit features
void SvdLearner::get_implicit_c(unsigned int user, double n) {

    // Clear array
    for (int j = 0; j < NUM_FEATS; ++j) {
        this->implicit_c[j] = 0;
    }
    for (int j = 0; j < NUM_FEATS; ++j) {
        this->temp_implicit_c[j] = 0;
    }
    unsigned int u_num_movies = this->count_user_rating[user];
    unsigned int u_movie;
    for (unsigned int i = 0; i < u_num_movies; ++i) {
        u_movie = this->user_movies[user][i];

        for (unsigned int j = 0; j < NUM_FEATS; ++j) {
            this->implicit_c[j] += this->implicit_features[u_movie][j];
        }
    }
    for (unsigned int j = 0; j < NUM_FEATS; ++j) {
        this->implicit_c[j] *= n;
        this->temp_implicit_c[j] = this->implicit_c[j];
    }
}

void SvdLearner::update_implicit_features(unsigned int user, double n) {
    unsigned int u_num_movies = this->count_user_rating[user];
    unsigned int u_movie;
    // For each movie vector that user has seen
    for (unsigned int i = 0; i < u_num_movies; i++) {
        // get movie and iterate update movie feature vector
        u_movie = this->user_movies[user][i];
        for (unsigned int j = 0; j < NUM_FEATS; j++) {
            this->implicit_features[u_movie][j] += n * (this->temp_implicit_c[j] - this->implicit_c[j]);
        }
    }
}

// update features based on an error
void SvdLearner::update_features(unsigned int user, unsigned int movie, double err) {
    double uv, mv, tc;
    for (int i = 0; i < NUM_FEATS; ++i) {
        uv = this->U[user][i];
        mv = this->V[movie][i];
        tc = this->temp_implicit_c[i];
        this->V[movie][i] += this->gamma2 *(err * (uv + tc) - FEAT_ADJ_RATE * mv);
        this->temp_implicit_c[i] += this->gamma2 * (err * this->V[movie][i] - FEAT_ADJ_RATE * tc);
        this->U[user][i] += this->gamma2 * (err * mv - FEAT_ADJ_RATE * uv);
    }
}

// update baselines based on an error
void SvdLearner::update_baselines(unsigned int user, unsigned int movie, double err,
    unsigned int item_bin, int user_date_count, double dev) {
    double uv = this->user_bias[user];
    double mv = this->movie_bias[movie];
    double ib = this->item_bin_bias[movie][item_bin];
    double alpha = this->user_alpha[user];
    double ub = this->user_time_bias[user][user_date_count];
    double uc = this->user_c[user];
    double utc = this->user_time_c[user][user_date_count];
    this->user_bias[user]                       += this->gamma1 * (err - BIAS_ADJ_RATE * uv);
    this->movie_bias[movie]                     += this->gamma1 * (err * (uc + utc) - BIAS_ADJ_RATE * mv);
    this->item_bin_bias[movie][item_bin]        += this->gamma1 * (err * (uc + utc) - BIAS_ADJ_RATE * ib);
    this->user_alpha[user]                      += this->gamma1 * (err * dev/NUM_FEATS - BIAS_ADJ_RATE * alpha);
    this->user_time_bias[user][user_date_count] += this->gamma1 * (err - BIAS_ADJ_RATE * ub);
    this->user_c[user]                          += this->gamma1 * (err * (mv + ib) - BIAS_ADJ_RATE * uc);
    this->user_time_c[user][user_date_count]    += this->gamma1 * (err * (mv + ib) - BIAS_ADJ_RATE * utc);
}

// do SGD
// assumes data is being read in user - movie sorting
void SvdLearner::train() {
    time_t start, end;
    printf("Training model...\n");

    double train_err, err, predict, feature_c, n, alpha, dev, user_time_b;
    unsigned int user, movie, date, rating, temp, date_temp, user_date_count, train_count, item_bin;

    // iterate for number of epochs
    for (unsigned int i = 0; i < NUM_EPOCHS; ++i) {
        time(&start);
        printf("Epoch %d...\n", i);
        train_err = 0;
        temp = 0;
        date_temp = 0;
        user_date_count = -1;
        train_count = 0;

        // go through all training data
        for (unsigned int i = 0; i < TRAIN_SIZE; ++i) {
            user = this->data[i][USER_COL];
            movie = this->data[i][MOVIE_COL];
            date = this->data[i][DATE_COL];
            rating = this->data[i][RATING_COL];

            // printf("user: %d movie: %d\n", user, movie);

            if (!(check_user(user) && check_movie(movie))) {
                printf("Invalid user (%d) or movie (%d).\n", user, movie);
            }

            // printf("b1\n");

            // Get rating from raw data if we have a new user
            if (temp != user) {
                // update implict feature for previous user
                if (temp > 0) {
                    // printf("b2\n");
                    this->update_implicit_features(temp, n);
                }
                // printf("b3 %d %d\n", user, this->count_user_rating[user]);
                n = inv_sqrt(this->count_user_rating[user]);
                // printf("b4\n");
                this->get_implicit_c(user, n);
                temp = user;
                date_temp = 0;
                user_date_count = -1;
            }
            // Update different date count
            if (date != date_temp) {
                user_date_count++;
                date_temp = date;
            }

            // printf("b5\n");
            // Get contribution from each feature
            feature_c = 0;
            for (unsigned int i = 0; i < NUM_FEATS; i++) {
                feature_c += (U[user][i] + temp_implicit_c[i]) * V[movie][i]; //FIX
            }

            // printf("c1\n");

            // Get user time bias and alpha
            alpha = user_alpha[user];
            // printf("c11\n");
            dev = this->user_dates[user][user_date_count] - this->avg_user_date[user];
            // printf("c12\n");
            user_time_b = this->user_time_bias[user][user_date_count];

            // printf("c2\n");
            // Get item bin number and bias
            item_bin = (int) floor((float) date / (float) BIN_SIZE);
            // printf("c3\n");
            // printf("item_bin %f\n", item_bin_bias[movie][item_bin]);
            // printf("user_date_count %f\n", user_time_c[user][user_date_count]);
            predict = (AVG_RATING + user_bias[user]
                + (movie_bias[movie] + item_bin_bias[movie][item_bin])
                    * (user_c[user] + user_time_c[user][user_date_count])
                 + alpha * dev + user_time_b + feature_c);

            // printf("predict %f\n", predict);
            // printf("c4\n");
            err = (double) rating - predict;

            train_err += err * err;
            // printf("c5\n");
            this->update_features(user, movie, err);
            // printf("c6\n");
            this->update_baselines(user, movie, err, item_bin, user_date_count, dev);
            train_count++;
        }

        // printf("d\n");

        // update last user
        this->update_implicit_features(user, n);

        // printf("e\n");

        // Update gammas by factor to decrease learning rate
        this->gamma1 *= GAMMA_STEP;
        this->gamma2 *= GAMMA_STEP;

        time(&end);

        printf("Train RMSE: %f. Took %.f seconds.\n",
            sqrt(train_err / (double) TRAIN_SIZE), difftime(end, start));
    }
}

// write all learned parameters to files
void SvdLearner::write() {
    matrix_write(SVD_U_FILE, this->U, NUM_USERS, NUM_FEATS);
    matrix_write(SVD_V_FILE, this->V, NUM_MOVIES, NUM_FEATS);
    vector_write(USER_BIAS_FILE, this->user_bias, NUM_USERS);
    vector_write(MOVIE_BIAS_FILE, this->movie_bias, NUM_MOVIES);

    /* need to save:
    implicit_features
    item_bin_bias
    user_time_bias - userDev[+1]
    user_alpha
    user_c
    user_time_c
    */
}
