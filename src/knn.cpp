#include "knn.hpp"

int main() {
    // read data
    DataReader * reader = new DataReader;

    // read qual
    reader->read_qual();

    // init learner
    KnnLearner knn;

    // learn parameters
    knn.train();
}

KnnLearner::KnnLearner() {

    time_t start, end;
    time(&start);
    printf("Initializing KnnLearner...\n");

    this->s = new double*[NUM_MOVIES];
    // this->ratings = new int*[NUM_MOVIES];
    // this->count_movie_rating = new int[NUM_MOVIES];
    // this->ratings_2 = new int*[NUM_MOVIES];

    for (unsigned int i = 0; i < NUM_MOVIES; ++i) {
        this->s[i] = new double[NUM_MOVIES];
    }

    // vector_read_int(CNT_MOVIE_RATING, this->count_movie_rating, NUM_MOVIES);

    // for (unsigned int i = 0; i < NUM_USERS; ++i) {
    //     this->ratings[i] = new int[this->count_movie_rating[i]];
    //     // this->ratings_2[i] = new int[NUM_MOVIES];
    // }

    this->N = new int[K];
    this->sim = new int[NUM_MOVIES];

    // TODO: FIX INPUT OF RATINGS
    // matrix_read_int(USER_MOVIES, this->ratings, NUM_USERS, NUM_MOVIES);

    printf("a\n");

    this->set_dr(reader);

    for (unsigned int i = 0; i < TRAIN_SIZE; ++i) {
        int rating = this->reader->train_set[i][RATING_COL];
        printf("b\n");
        if (rating > 0) {
            printf("d\n");
            this->ratings_map[make_pair(this->reader->train_set[i][USER_COL], this->reader->train_set[i][MOVIE_COL])] = rating;
            // printf("%d\n", rating);
        }
        printf("e\n");
    }

    printf("b\n");

    this->compute_similarity_coef();

    printf("c\n");

    time(&end);
    printf("KnnLearner initialized. Took %.f seconds.\n", difftime(end, start));
}

KnnLearner::~KnnLearner() {

    printf("Deinitializing KnnLearner...\n");

    for (unsigned int i = 0; i < NUM_MOVIES; ++i) {
        delete[] this->s[i];
    }

    // for (unsigned int i = 0; i < NUM_USERS; ++i) {
    //     delete[] this->ratings[i];
    //     // delete[] this->ratings_2[i];
    // }

    delete[] this->s;
    // delete[] this->ratings;
    // delete[] this->ratings_2;

    delete[] this->N;
    delete[] this->sim;
    // delete[] this->count_movie_rating;
}

// sets the data to the passed value
void KnnLearner::set_dr(DataReader * reader) {
    this->reader = reader;
}

// precompute the values of similarities between all movies
void KnnLearner::compute_similarity_coef() {

    // I don't halve the matrix because then I have to do additional if statements
    vector <int> U;

    for (unsigned int i = 0; i < NUM_MOVIES; ++i) {
        for (unsigned int j = 0; j < NUM_MOVIES; ++j) {
            U = compute_U(i, j);
            int size_U = U.size();
            double se = 0;

            for (unsigned int u = 0; u < size_U; ++u) {
                se += (this->ratings_map[make_pair(U[u],i)] - this->ratings_map[make_pair(U[u],j)]) *
                (this->ratings_map[make_pair(U[u],i)] - this->ratings_map[make_pair(U[u],j)]);
            }
            this->s[i][j] = size_U / (se + ALPHA);
        }
    }
}

// compute users that rated both movies i and j
vector <int> KnnLearner::compute_U(int i, int j) {
    vector <int> U;

    for (unsigned int k = 0; k < NUM_USERS; ++k) {
        if (this->ratings_map[make_pair(k,i)] > 0 && this->ratings_map[make_pair(k,i)] > 0) {
            U.push_back(k);
        }
    }

    return U;
}

// calculate neighboring items that user has rated with top similarity coef
void KnnLearner::sort_neighbors(int user, int movie) {
    // Set N from previous iteration to 0 just in case
    for (unsigned int i = 0; i < K; ++i) {
        N[i] = 0;
    }

    for (unsigned int i = 0; i < NUM_MOVIES; ++i) {
        sim[i] = s[user][i];
    }

    // This isn't the fastest way to do this right now
    // TODO MAKE FASTER
    double highest_val;
    int highest_movie;

    for (unsigned int i = 0; i < K; ++i) {
        for (unsigned int j = 0; j < NUM_MOVIES; ++j) {
            if (sim[j] > highest_val && this->ratings_map[make_pair(user,j)] > 0 && j != movie) {
                highest_val = sim[j];
                highest_movie = j;
            }
        }

        N[i] = highest_movie;
        sim[highest_movie] = 0;
        highest_movie = 0;
        highest_val = 0;
    }
}

// train using knn
void KnnLearner::train() {
    // I don't think we need epochs here
    // for (unsigned int i = 0; i < NUM_EPOCHS; ++i) { }
    time_t start, end;
    printf("Training model...\n");
    time(&start);

    int user, movie, rating, train_count = 0;
    double numerator, denominator, coef, predict, err, train_err = 0;

    for (unsigned int i = 0; i < TRAIN_SIZE; ++i) {
        user = this->reader->train_set[i][USER_COL];
        movie = this->reader->train_set[i][MOVIE_COL];
        rating = this->reader->train_set[i][RATING_COL];
        train_err = 0;

        for (unsigned int j = 0; j < K; ++j) {
            if (N[j] > 0) {
                coef = s[movie][N[j]];

                numerator += coef * this->ratings_map[make_pair(user,N[j])];
                denominator += coef;
            }
        }

        predict = numerator / denominator;
        predict = bound(predict);
        err = (double) rating - predict;

        train_err += err * err;
        train_count++;

        numerator = 0;
        denominator = 0;
    }

    time(&end);
    printf("Train RMSE: %f. Took %.f seconds.\n",
        sqrt(train_err / (double) TRAIN_SIZE), difftime(end, start));
}

void KnnLearner::pred() {
    int user, movie = 0;

    ofstream out_file;
    // out_file.open(KNN_FILE);

    for (unsigned int i = 0; i < QUAL_SIZE; ++i) {
        user = this->reader->qual_set[i][USER_COL];
        movie = this->reader->qual_set[i][MOVIE_COL];

    }
}
