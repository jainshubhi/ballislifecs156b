#include "rbm2.hpp"

int main() {
    // seed the random generator
    srand(time(NULL));

    // read data
    DataReader * reader = new DataReader;

    // read qual
    reader->read_qual();

    // init learner
    RbmLearner2 rbm;

    // set reader
    rbm.set_dr(reader);

    // learn parameters
    rbm.train();

    // write predictions on qual and blend to file
    rbm.pred(RBM_BLEND, /* blend */ false, /* write */ true);
    rbm.pred(RBM_QUAL, /* qual */ true, /* write */ true);

    printf("Done.\n");
    return 1;
}

RbmLearner2::RbmLearner2() {
    time_t start, end;
    time(&start);
    printf("Initializing RbmLearner2...\n");

    // Initialize third order tensors
    this->W = new double**[NUM_MOVIES];
    this->grad = new double**[NUM_MOVIES];

    this->B_grad = new double*[NUM_MOVIES];
    this->b_grad = new double[NUM_FACTORS];

    this->B_bias = new double*[NUM_MOVIES];
    this->b_bias = new double[NUM_FACTORS];

    for (unsigned int i = 0; i < NUM_MOVIES; ++i) {
        this->W[i] = new double*[NUM_FACTORS];
        this->grad[i] = new double*[NUM_FACTORS];

        this->B_grad[i] = new double[NUM_RATINGS];
        this->B_bias[i] = new double[NUM_RATINGS];
        for (unsigned int j = 0; j < NUM_FACTORS; ++j) {
            this->W[i][j] = new double[NUM_RATINGS];
            this->grad[i][j] = new double[NUM_RATINGS];
            for (unsigned int k = 0; k < NUM_RATINGS; ++k) {
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
    printf("RbmLearner2 initialized. Took %.f seconds.\n", difftime(end, start));
}

RbmLearner2::~RbmLearner2() {
    time_t start, end;
    time(&start);
    printf("Deinitializing RbmLearner2...\n");

    delete[] this->count_user_rating;
    delete[] this->user_offset;
    delete[] this->minibatch;

    for(unsigned int i = 0; i < NUM_MOVIES; ++i) {
        delete[] this->B_grad[i];
        delete[] this->B_bias[i];
        for (unsigned int j = 0; j < NUM_FACTORS; ++j) {
            delete[] this->W[i][j];
            delete[] this->grad[i][j];
        }
        delete[] this->W[i];
        delete[] this->grad[i];
    }

    delete[] this->W;
    delete[] this->grad;
    delete[] this->B_bias;
    delete[] this->B_grad;
    delete[] this->b_bias;
    delete[] this->b_grad;

    time(&end);
    printf("RbmLearner2 deinitialized. Took %.f seconds.\n", difftime(end, start));
}

// sets the data to the passed value
void RbmLearner2::set_dr(DataReader * reader) {
    this->reader = reader;
}

// create V for each user
double ** RbmLearner2::create_v(int user) {
    int index = this->user_offset[user],
        count = this->count_user_rating[user],
        movie, rating;
    double ** V = new double*[count];
    // fill up V with movies/ratings
    for (unsigned int i = 0; i < count; ++i) {
        // initialize array
        V[i] = new double[NUM_RATINGS + 1];
        for (unsigned int k = 0; k <= NUM_RATINGS; ++k) {
            V[i][k] = 0;
        }
    }
    for (unsigned int i = 0; i < count; ++i) {
        // fill it up
        movie = this->reader->train_set[index + i][MOVIE_COL];
        rating = this->reader->train_set[index + i][RATING_COL];
        V[i][0] = movie;
        V[i][rating] = 1;
    }
    return V;
}

// calculate h based on V (with bias component)
double* RbmLearner2::h_calc(double ** V, int user) {
    double * h = new double[NUM_FACTORS];
    int index = this->user_offset[user],
        count = this->count_user_rating[user], movie;
    double sum;
    for (unsigned int j = 0 ; j < NUM_FACTORS; ++j) {
        sum = 0;
        for (unsigned int i = index; i < index + count; ++i) {
            for (unsigned int k = 1; k <= NUM_RATINGS; ++k) {
                movie = this->reader->train_set[i][MOVIE_COL];
                sum += V[i - index][k] * W[movie][j][k - 1];
            }
        }
        h[j] = 1.0 / (1 + exp(-1.0 * (sum))); // + b_bias[j])));
    }
    return h;
}

// Copy h
double * RbmLearner2::copy_h(double * h) {
    double * copy_h = new double[NUM_FACTORS];
    for (unsigned int i = 0; i < NUM_FACTORS; ++i)
        copy_h[i] = h[i];
    return copy_h;
}

// To assign binary values for exp calculations.
void RbmLearner2::threshold_h(double * h) {
    for (unsigned int i = 0; i < NUM_FACTORS; ++i)
        if (h[i] > one_rand())
            h[i] = 1;
        else
            h[i] = 0;
}

// Find exp data
double *** RbmLearner2::exp_calc(double ** V, double * h, int user) {
    int count = this->count_user_rating[user];
    // initialize exp
    double *** exp_mat = new double**[NUM_MOVIES];
    for (unsigned int i = 0; i < NUM_MOVIES; ++i) {
        exp_mat[i] = new double*[NUM_FACTORS];
        for (unsigned int j = 0; j < NUM_FACTORS; ++j) {
            exp_mat[i][j] = new double[NUM_RATINGS];
            for (unsigned int k = 0; k < NUM_RATINGS; ++k) {
                exp_mat[i][j][k] = 0;
            }
        }
    }
    for (unsigned int i = 0; i < count; ++i) {
        for (unsigned int j = 0; j < NUM_FACTORS; ++j) {
            for (unsigned int k = 1; k <= NUM_RATINGS; ++k) {
                exp_mat[(int)V[i][0]][j][k - 1] = V[i][k] * h[j];
            }
        }
    }
    return exp_mat;
}

double ** RbmLearner2::v_calc(double * h, int user) {
    int index = this->user_offset[user],
        count = this->count_user_rating[user];
    double num, denom, temp;
    double ** V = new double*[count];
    for (unsigned int i = 0; i < count; ++i) {
        V[i] = new double[NUM_RATINGS + 1];
        for (unsigned int k = 0; k <= NUM_RATINGS; ++k) {
            V[i][k] = 0;
        }
    }
    for (unsigned int i = 0; i < count; ++i) {
        V[i][0] = this->reader->train_set[index + i][MOVIE_COL];
        for (unsigned int k = 1; k <= NUM_RATINGS; ++k) {
            num = 0;
            denom = 0;
            for (unsigned int j = 0; j < NUM_FACTORS; ++j) {
                num += h[j] * W[(int)V[i][0]][j][k - 1];
            }
            // num += B_bias[(int)V[i][0]][k - 1];
            for (unsigned int l = 1; l <= NUM_RATINGS; ++l) {
                temp = 0;
                for (unsigned int j = 0; j < NUM_FACTORS; ++j) {
                    temp += h[j] * W[(int)V[i][0]][j][l - 1];
                }
                denom += exp(temp);
                // denom += exp(temp + B_bias[int(V[i][0])][l - 1]);
            }
            V[i][k] = exp(num) / denom;
        }
    }
    return V;
}

double RbmLearner2::predict(double * h, int movie) {
    double prediction = 0, num, denom, temp;
    for (unsigned int k = 1; k <= NUM_RATINGS; ++k) {
        num = 0;
        denom = 0;
        for (unsigned int j = 0; j < NUM_FACTORS; ++j) {
            num += h[j] * W[movie][j][k - 1];
        }
        // num += B_bias[(int)V[i][0]][k - 1];
        for (unsigned int l = 0; l < NUM_RATINGS; ++l) {
            temp = 0;
            for (unsigned int j = 0; j < NUM_FACTORS; ++j) {
                temp += h[j] * W[movie][j][l];
            }
            // denom += exp(temp + B_bias[int(V[i][0])][l - 1]);
            denom += exp(temp);
        }
        prediction += k * exp(num) / denom;
    }
    return prediction;
}

void RbmLearner2::create_minibatch() {
    for (unsigned int i = 0; i < MINIBATCH_SIZE; ++i) {
        minibatch[i] = minibatch_random();
    }
}

void RbmLearner2::train() {
    // initialize variables
    int user, rating, movie, index, count, total_count = 0;
    double pre, err, train_err; //, squares;
    time_t start, end;

    // learn W
    for (unsigned int e = 0; e < RBM_EPOCHS; ++e) {
        printf("Epoch %d.\n", e);
        time(&start);
        // set gradient to 0;
        for (unsigned int i = 0; i < NUM_MOVIES; ++i) {
            for (unsigned int j = 0; j < NUM_FACTORS; ++j) {
                for (unsigned int k = 0; k < NUM_RATINGS; ++k) {
                    this->grad[i][j][k] = 0;
                }
            }
        }
        train_err = 0;
        create_minibatch();
        for (unsigned int u = 0; u < MINIBATCH_SIZE; ++u) {
            double ** V, ** new_V, * h, * c_h, * new_h, * c_new_h,
                *** exp_data, *** exp_recon;
            user = minibatch[u];
            index = this->user_offset[user];
            count = this->count_user_rating[user];
            total_count += count;
            V = create_v(user);
            h = h_calc(V, user);
            // binary values of h
            c_h = copy_h(h);
            threshold_h(c_h);
            exp_data = exp_calc(V, c_h, user);
            new_V = v_calc(h, user);
            // Reconstruct h
            new_h = h_calc(new_V, user);
            c_new_h = copy_h(new_h);
            threshold_h(c_new_h);
            exp_recon = exp_calc(new_V, c_new_h, user);

            // calculate gradient
            matrix_scalar_mult(grad, u, NUM_MOVIES, NUM_FACTORS, NUM_RATINGS);
            matrix_add(exp_data, exp_recon, NUM_MOVIES, NUM_FACTORS, NUM_RATINGS, -1);
            matrix_add(grad, exp_data, NUM_MOVIES, NUM_FACTORS, NUM_RATINGS, 1);
            matrix_scalar_mult(grad, 1/(u + 1), NUM_MOVIES, NUM_FACTORS, NUM_RATINGS);

            // Bias gradient
            // matrix_scalar_mult(B_grad, u, NUM_MOVIES, NUM_RATINGS);
            // matrix_add(V, new_V, count, NUM_RATINGS, -1);
            // for (unsigned int i = 0; i < count; ++i) {
            //     for (unsigned int k = 0; k < NUM_RATINGS; ++k) {
            //         B_grad[(int)V[i][0]][k] += V[i][k + 1];
            //     }
            // }
            // matrix_scalar_mult(B_grad, 1/(u + 1), NUM_MOVIES, NUM_RATINGS);
            //
            // for (unsigned int j = 0; j < NUM_FACTORS; ++j) {
            //     b_grad[j] *= u;
            //     b_grad[j] += h[j] - new_h[j];
            //     b_grad[j] /= u + 1;
            // }

            // calculate prediction
            for (unsigned int i = 0; i < count; ++i) {
                movie = this->reader->train_set[index + i][MOVIE_COL];
                rating = this->reader->train_set[index + i][RATING_COL];

                pre = bound(predict(new_h, movie));

                err = rating - pre;
                train_err += err * err;
            }

            // calculate sum of squares of differences
            // if (u == MINIBATCH_SIZE - 1) {
            //     matrix_add(new_V, V, count, NUM_RATINGS, -1);
            //     matrix_square(new_V, count, NUM_RATINGS);
            //     squares = total_matrix_sum(new_V, count, NUM_RATINGS);
            //     printf("Sum of squares is %f\n", squares);
            // }
            //
            // if (u <= MINIBATCH_SIZE - 1) {
            //     squares = 0;
            //     for (unsigned int i = 0; i < count; ++i) {
            //         for (unsigned int k = 1; k <= NUM_RATINGS; ++k) {
            //             // printf("new_V is %f.\n", new_V[i][k]);
            //             // printf("V is %f.\n", V[i][k]);
            //             squares += (new_V[i][k] - V[i][k]) * (new_V[i][k] - V[i][k]);
            //         }
            //     }
            // }

            // delete all this stuff
            for (unsigned int i = 0; i < NUM_MOVIES; ++i) {
                for (unsigned int j = 0; j < NUM_FACTORS; ++j) {
                    delete[] exp_data[i][j];
                    delete[] exp_recon[i][j];
                }
                delete[] exp_data[i];
                delete[] exp_recon[i];
            }

            for (unsigned int i = 0; i < count; ++i) {
                delete[] V[i];
                delete[] new_V[i];
            }

            delete[] exp_data;
            delete[] exp_recon;
            delete[] h;
            delete[] c_h;
            delete[] new_h;
            delete[] c_new_h;
        }
        // take gradient step for W & biases
        matrix_scalar_mult(grad, LEARNING_RATE, NUM_MOVIES, NUM_FACTORS, NUM_RATINGS);
        matrix_add(W, grad, NUM_MOVIES, NUM_FACTORS, NUM_RATINGS, 1);
        // matrix_scalar_mult(B_grad, LEARNING_RATE, NUM_MOVIES, NUM_RATINGS);
        // matrix_add(B_bias, B_grad, NUM_MOVIES, NUM_RATINGS, 1);
        // for (unsigned int j = 0; j < NUM_FACTORS; ++j) {
        //     b_bias[j] += LEARNING_RATE * b_grad[j];
        // }

        // checking NaN stuff
        if (W[0][0][0] != W[0][0][0]) {
            printf("RBM Nan-s at Epoch %d.\n", e);
            break;
        }

        time(&end);
        printf("Train RMSE: %f. Took %.f seconds.\n",
            sqrt(train_err / ((double) total_count)), difftime(end, start));
    }
}

void RbmLearner2::pred(string predictions, bool is_qual, bool write) {
    int user, movie, count, size;
    double pre, train_err = 0.0, err;
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
        double * h, ** V, * new_h, ** new_V;
        user = data[i][USER_COL];
        movie = data[i][MOVIE_COL];

        // get info for current user and date if needed
        count = this->count_user_rating[user];
        V = create_v(user);
        h = h_calc(V, user);
        // binary values of h
        new_V = v_calc(h, user);
        // Reconstruct h
        new_h = h_calc(new_V, user);

        // calculate prediction
        pre = bound(predict(new_h, movie));

        // if blend set, then calculate the error
        if (!is_qual) {
            err = pre - data[i][RATING_COL];
            train_err += err * err;
        }

        // write to file
        if (write) {
            outfile << pre;
            outfile << "\n";
        }

        // delete matrices/vectors
        for (unsigned int i = 0; i < count; ++i) {
            delete[] V[i];
            delete[] new_V[i];
        }
        delete[] h;
        delete[] new_h;
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
