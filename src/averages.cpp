#include "compute.cpp"

int main() {
    // read in data
    DataReader * reader = new DataReader;
    int user, movie, rating, date, curr_user = -1, curr_date = -1;

    // compute averages and counts
    double * avg_user_rating = new double[NUM_USERS];
    int * count_user_rating = new int[NUM_USERS];
    double * avg_user_date = new double[NUM_USERS];

    int * count_user_unique_dates = new int[NUM_USERS];

    double * avg_movie_rating = new double[NUM_MOVIES];
    int * count_movie_rating = new int[NUM_MOVIES];

    for (unsigned int i = 0; i < NUM_USERS; ++i) {
        avg_user_rating[i] = 0;
        count_user_rating[i] = 0;
        avg_user_date[i] = 0;
        count_user_unique_dates[i] = 0;
    }

    for (unsigned int i = 0; i < NUM_MOVIES; ++i) {
        avg_movie_rating[i] = 0;
        count_movie_rating[i] = 0;
    }

    for (unsigned int i = 0; i < TRAIN_SIZE; ++i) {
        user = reader->train_set[i][USER_COL];
        movie = reader->train_set[i][MOVIE_COL];
        rating = reader->train_set[i][RATING_COL];
        date = reader->train_set[i][DATE_COL];

        avg_user_rating[user] += rating;
        avg_movie_rating[movie] += rating;
        avg_user_date[user] += date;

        count_user_rating[user]++;
        count_movie_rating[movie]++;

        if (user == curr_user) {
            if (date != curr_date) {
                count_user_unique_dates[user]++;
            }
        }
        else {
            count_user_unique_dates[user]++;
        }
        curr_user = user;
        curr_date = date;
    }

    int mean_user_count = mean(count_user_rating, NUM_USERS);
    int mean_movie_count = mean(count_movie_rating, NUM_MOVIES);


    for (unsigned int i = 0; i < NUM_USERS; ++i) {
        avg_user_rating[i] /= (double) count_user_rating[i];
        avg_user_date[i] /= (double) count_user_rating[i];
        avg_user_rating[i] -= AVG_RATING;
    }

    for (unsigned int i = 0; i < NUM_MOVIES; ++i) {
        avg_movie_rating[i] /= (double) count_movie_rating[i];
        avg_movie_rating[i] -= AVG_RATING;
    }

    // write averages and counts to files
    vector_write(AVG_USER_RATING, avg_user_rating, NUM_USERS);
    vector_write_int(CNT_USER_RATING, count_user_rating, NUM_USERS);
    vector_write(AVG_USER_DATE, avg_user_date, NUM_USERS);
    vector_write(AVG_MOVIE_RATING, avg_movie_rating, NUM_MOVIES);
    vector_write_int(CNT_MOVIE_RATING, count_movie_rating, NUM_MOVIES);

    // delete everything but count_user_rating
    delete[] avg_user_rating;
    delete[] avg_user_date;
    delete[] avg_movie_rating;
    delete[] count_movie_rating;

    // get movies and dates watched for each user
    int ** user_movies = new int*[NUM_USERS];
    int ** user_dates = new int*[NUM_USERS];

    // high-high, high-low, low-high, low-low is the order
    int * category_counts = new int[4];

    for (unsigned int i = 0; i < NUM_USERS; ++i) {
        user_movies[i] = new int[count_user_rating[i]];
        user_dates[i] = new int[count_user_unique_dates[i]];
    }

    // assumes data is being read in user - movie sorting
    int count = 0, date_ind = -1;
    curr_user = -1;
    curr_date = -1;
    for (unsigned int i = 0; i < TRAIN_SIZE; ++i) {
        date = reader->train_set[i][DATE_COL];
        user = reader->train_set[i][USER_COL];

        if (user == curr_user) {
            count++;
            if (date != curr_date) {
                curr_date = date;
                date_ind++;
                user_dates[user][date_ind] = curr_date;
            }
        }
        else {
            curr_user = reader->train_set[i][USER_COL];
            count = 0;
            date_ind = 0;
            curr_date = date;
            user_dates[user][date_ind] = curr_date;
        }

        user_movies[curr_user][count] = reader->train_set[i][MOVIE_COL];
        user_dates[curr_user][count] = reader->train_set[i][DATE_COL];
    }

    for (unsigned int i = 0; i < BLEND_SIZE; ++i) {
        // get blending counts
        user = reader->blend_set[i][USER_COL];
        movie = reader->blend_set[i][MOVIE_COL];
        if (count_user_rating[user] > mean_user_count) {
            if (count_movie_rating[movie] > mean_movie_count) {
                category_counts[0]++;
            }
            else {
                category_counts[1]++;
            }
        }
        else {
            if (count_movie_rating[movie] > mean_movie_count) {
                category_counts[2]++;
            }
            else {
                category_counts[3]++;
            }
        }
    }

    // write movies and dates that users have watched into file
    vector_write_int(USER_DATE_COUNTS, count_user_unique_dates, NUM_USERS);
    vector_write_int(BLEND_COUNTS, category_counts, 4);
    matrix_write_var_int(USER_MOVIES, user_movies, NUM_USERS, count_user_rating);
    matrix_write_var_int(USER_DATES, user_dates, NUM_USERS, count_user_unique_dates);

    // delete all remaining allocations
    for (unsigned int i = 0; i < NUM_USERS; ++i) {
        delete[] user_movies[i];
        delete[] user_dates[i];
    }

    delete[] category_counts;
    delete[] count_user_rating;
    delete[] user_movies;
    delete[] user_dates;
    delete[] count_user_unique_dates;
}
