#include "svd.hpp"

int main() {
    // read data
    DataReader * reader = new DataReader;

    // read qual
    reader->read_qual();

    // init learner
    SvdLearner svd;

    // set reader
    svd.set_dr(reader);

    // learn parameters
    svd.train();

    // write predictions on qual and blend to file
    svd.pred(SVD_BLEND, /* blend */ false, /* write */ true);
    svd.pred(SVD_QUAL, /* qual */ true, /* write */ true);

    printf("Done.\n");

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
    this->count_user_unique_dates = new int[NUM_USERS];

    this->movie_bias = new double[NUM_MOVIES];
    this->count_movie_rating = new int[NUM_MOVIES];

    // to speed up computaions store implicit contributions
    this->implicit_c = new double[NUM_FEATS];
    this->temp_implicit_c = new double[NUM_FEATS];

    // contains time deviation parameters
    this->user_c = new double[NUM_USERS];
    this->user_alpha = new double[NUM_USERS];

    for (unsigned int i = 0; i < NUM_USERS; ++i) {
        this->user_c[i] = small_pos_rand();
        this->user_alpha[i] = small_pos_rand();
    }

    vector_read(AVG_USER_RATING, this->user_bias, NUM_USERS);
    vector_read_int(CNT_USER_RATING, this->count_user_rating, NUM_USERS);
    vector_read(AVG_USER_DATE, this->avg_user_date, NUM_USERS);
    vector_read(AVG_MOVIE_RATING, this->movie_bias, NUM_MOVIES);
    vector_read_int(CNT_MOVIE_RATING, this->count_movie_rating, NUM_MOVIES);
    vector_read_int(USER_DATE_COUNTS, count_user_unique_dates, NUM_USERS);

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
        this->user_dates[i] = new int[this->count_user_unique_dates[i]];
        this->user_time_bias[i] = new double[this->count_user_unique_dates[i]];
        this->user_time_c[i] = new double[this->count_user_unique_dates[i]];

        for (unsigned int j = 0; j < NUM_FEATS; ++j) {
            this->U[i][j] = small_rand();
        }

        for (unsigned int j = 0; j < this->count_user_unique_dates[i]; ++j) {
            this->user_time_bias[i][j] = small_pos_rand();
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
    printf("Deinitializing SvdLearner...\n");
    for (unsigned int i = 0; i < NUM_USERS; ++i) {
        delete[] this->U[i];
        delete[] this->user_movies[i];
        delete[] this->user_dates[i];
        delete[] this->user_time_bias[i];
        delete[] this->user_time_c[i];
    }

    for (unsigned int i = 0; i < NUM_MOVIES; ++i) {
        delete[] this->V[i];
        delete[] this->item_bin_bias[i];
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
    delete[] this->count_user_unique_dates;

    delete[] this->implicit_c;
    delete[] this->temp_implicit_c;

    delete[] this->user_c;
    delete[] this->user_alpha;

    printf("SvdLearner deinitialized.\n");
}

// sets the data to the passed value
void SvdLearner::set_dr(DataReader * reader) {
    this->reader = reader;
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

        // this->V[movie][i] += this->gamma2 *(err * (uv + tc) - FEAT_ADJ_RATE * mv);
        this->V[movie][i] += this->gamma2 *(err * uv - FEAT_ADJ_RATE * mv);
        // this->U[user][i] += this->gamma2 * (err * mv - FEAT_ADJ_RATE * uv);
        this->U[user][i] += this->gamma2 * (err * mv - FEAT_ADJ_RATE * uv);

        this->temp_implicit_c[i] += this->gamma2 * (err * this->V[movie][i] - FEAT_ADJ_RATE * tc);
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

    // this->user_bias[user]                       += this->gamma1 * (err - BIAS_ADJ_RATE * uv);
    this->user_bias[user]                       += this->gamma1 * (err - BIAS_ADJ_RATE * uv);
    // this->movie_bias[movie]                     += this->gamma1 * (err * (uc + utc) - BIAS_ADJ_RATE * mv);
    this->movie_bias[movie]                     += this->gamma1 * (err - BIAS_ADJ_RATE * mv);

    this->item_bin_bias[movie][item_bin]        += this->gamma1 * (err * (uc + utc) - BIAS_ADJ_RATE * ib);
    this->user_alpha[user]                      += this->gamma1 * (err * dev/NUM_FEATS - BIAS_ADJ_RATE * alpha);
    this->user_time_bias[user][user_date_count] += this->gamma1 * (err - BIAS_ADJ_RATE * ub);
    this->user_c[user]                          += this->gamma1 * (err * (mv + ib) - BIAS_ADJ_RATE * uc);
    this->user_time_c[user][user_date_count]    += this->gamma1 * (err * (mv + ib) - BIAS_ADJ_RATE * utc);
}

// do SGD
// assumes data is being read in user - time - movie sorting
void SvdLearner::train() {
    time_t start, end;
    printf("Training model...\n");

    double train_err, err, predict, feature_c, n, alpha, dev, user_time_b,
        user_time_c_bias, bin_bias;
    unsigned int user, movie, date, rating, temp, date_temp, user_date_count, train_count, item_bin;

    // iterate for number of epochs
    for (unsigned int i = 0; i < NUM_EPOCHS; ++i) {
        time(&start);
        printf("Epoch %d...\n", i + 1);
        train_err = 0;
        temp = 0;
        date_temp = 0;
        user_date_count = -1;
        train_count = 0;

        // go through all training data
        for (unsigned int j = 0; j < TRAIN_SIZE; ++j) {
            user = this->reader->train_set[j][USER_COL];
            movie = this->reader->train_set[j][MOVIE_COL];
            date = this->reader->train_set[j][DATE_COL];
            rating = this->reader->train_set[j][RATING_COL];

            // Get rating from raw data if we have a new user
            if (temp != user) {
                // update implict feature for previous user
                if (temp > 0) {
                    this->update_implicit_features(temp, n);
                }
                n = inv_sqrt(this->count_user_rating[user]);
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

            // Get contribution from each feature
            feature_c = 0;
            for (unsigned int k = 0; k < NUM_FEATS; ++k) {
                feature_c += (U[user][k] + temp_implicit_c[k]) * V[movie][k]; //FIX
            }

            // Get user time bias and alpha
            alpha = user_alpha[user];
            // dev = this->user_dates[user][user_date_count] - this->avg_user_date[user];
            dev = dev_ut(date, this->avg_user_date[user]);
            user_time_b = this->user_time_bias[user][user_date_count];
            user_time_c_bias = user_time_c[user][user_date_count];

            // Get item bin number and bias
            item_bin = (int) floor((float) date / (float) BIN_SIZE);
            bin_bias = item_bin_bias[movie][item_bin];

            predict =
                AVG_RATING
                + user_bias[user]
                + (movie_bias[movie] + bin_bias) * (user_c[user] + user_time_c_bias)
                + alpha * dev
                + user_time_b + feature_c;

            predict = bound(predict);

            // basic prediction formula
            // ************************
            predict = AVG_RATING + user_bias[user] + movie_bias[movie] + feature_c;

            err = (double) rating - predict;

            train_err += err * err;
            this->update_features(user, movie, err);
            this->update_baselines(user, movie, err, item_bin, user_date_count, dev);
            train_count++;
        }

        // update last user
        this->update_implicit_features(user, n);

        // Update gammas by factor to decrease learning rate
        this->gamma1 *= GAMMA_STEP;
        this->gamma2 *= GAMMA_STEP;

        time(&end);

        printf("Train RMSE: %f. Took %.f seconds.\n",
            sqrt(train_err / (double) TRAIN_SIZE), difftime(end, start));

        // get RMSE on blend set every 5 epochs to get an idea of overfitting
        if (((i + 1) % 5 == 0) && (i != 0) && (i != NUM_EPOCHS - 1)) {
            this->pred(VOID_FILE, /* blend */ false, /* do not write */ false);
        }
    }
}

void SvdLearner::pred(string predictions, bool is_qual, bool write) {
    int user, movie, date, temp = 0, user_date_count, item_bin, size;
    double feature_c, predict, n, alpha, dev, user_time_b, user_time_c_bias,
        sum_b, sum_c, err, train_err = 0.0;
    ofstream outfile;

    int ** data;
    if (is_qual) {
        data = this->reader->qual_set;
        size = QUAL_SIZE;
        printf("Predicting on qual set...\n");
    }
    else {
        data = this->reader->blend_set;
        size = BLEND_SIZE;
        printf("Predicting on blend set...\n");
    }

    if (write) {
        outfile.open(predictions);
    }

    for (unsigned int i = 0; i < size; ++i) {
        user = data[i][USER_COL];
        movie = data[i][MOVIE_COL];
        date = data[i][DATE_COL];

        // get info for current user and date if needed
        if (temp != user) {
            n = inv_sqrt(this->count_user_rating[user]);
            this->get_implicit_c(user, n);
            temp = user;
        }

        // calculate prediction
        feature_c = 0;
        for (unsigned int j = 0; j < NUM_FEATS; ++j) {
            feature_c += (U[user][j] + temp_implicit_c[j]) * V[movie][j];
        }

        // Check if day was in training set and get day bias
        sum_b = 0;
        sum_c = 0;
        user_date_count = -1;
        for (unsigned int j = 0; j < count_user_unique_dates[user]; ++j) {
            sum_b += this->user_time_bias[user][j];
            sum_c += this->user_time_c[user][j];
            if (user_dates[user][j] == date) {
                user_date_count = j;
                break;
            }
        }
        // if date was there then use bias
        if (user_date_count >= 0) {
            user_time_b = this->user_time_bias[user][user_date_count];
            user_time_c_bias = user_time_c[user][user_date_count];
        }
        // otherwise use average or 0
        else {
            // user_time_b = sum_b / (double) count_user_unique_dates[user];
            // user_time_c_bias = sum_c / (double) count_user_unique_dates[user];
            user_time_b = 0;
            user_time_c_bias = 0;
        }

        // Get user time bias and alpha
        alpha = user_alpha[user];
        dev = dev_ut(date, this->avg_user_date[user]);

        // Get item bin number and bias
        item_bin = (int) floor((float) date / (float) BIN_SIZE);

        predict =
            AVG_RATING
            + user_bias[user]
            + (movie_bias[movie] + item_bin_bias[movie][item_bin]) * (user_c[user] + user_time_c_bias)
            + alpha * dev
            + user_time_b + feature_c;

        // basic prediction formula
        // ************************
        predict = AVG_RATING + user_bias[user] + movie_bias[movie] + feature_c;

        predict = bound(predict);

        // if blend set, then calculate the error
        if (!is_qual) {
            err = predict - data[i][RATING_COL];
            train_err += err * err;
        }

        // write to file
        if (write) {
            outfile << predict;
            outfile << "\n";
        }
    }

    if (write) {
        outfile.close();
    }

    if (is_qual) {
        printf("Qual set predicted.\n");
    }
    else {
        if (write) {
            printf("Blend set predicted. RMSE: %f\n",
                sqrt(train_err / (double) BLEND_SIZE));
        }
        else {
            printf("Blend RMSE: %f\n", sqrt(train_err / (double) BLEND_SIZE));
        }
    }
}
