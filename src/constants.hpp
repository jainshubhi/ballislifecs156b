// all includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

// to change between um, ut, mu these next few lines must be changed
#define DATA_FILE  "data/ut/all.dta"
#define IDX_FILE   "data/ut/all.idx"
#define QUAL_FILE  "data/um/qual.dta"
#define OUT_FILE   "pred/final_blend.txt"
#define USER_COL   0
#define MOVIE_COL  1
#define DATE_COL   2
#define RATING_COL 3

// files for blend and qual predictions
// svd
#define SVD_BLEND  "pred/svd_blend_f150_e20.txt"
#define SVD_QUAL   "pred/svd_qual_f150_e20.txt"
// knn
#define KNN_BLEND  "pred/knn_blend.txt"
#define KNN_QUAL   "pred/knn_qual.txt"
// rbm
#define RBM_BLEND  "pred/rbm_blend.txt"
#define RBM_QUAL   "pred/rbm_qual.txt"

// place holder for filenames
#define VOID_FILE  "x"

// files for initial biases & counts
#define AVG_USER_RATING   "gen/avg_user_rating.txt"
#define CNT_USER_RATING   "gen/cnt_user_rating.txt"
#define AVG_USER_DATE     "gen/avg_user_date.txt"
#define AVG_MOVIE_RATING  "gen/avg_movie_rating.txt"
#define CNT_MOVIE_RATING  "gen/cnt_movie_rating.txt"
#define USER_MOVIES       "gen/user_movies.txt"
#define USER_DATES        "gen/user_dates.txt"
#define BLEND_COUNTS      "gen/blend_counts.txt"
#define USER_DATE_COUNTS  "gen/user_date_counts.txt"
#define BLEND_RATINGS     "gen/blend_only_ratings.txt"
#define USER_OFFSETS      "gen/user_offsets.txt"

// constants regarding data
#define DATA_SIZE   102416306
#define TRAIN_SIZE  94362233
#define VALID_SIZE  1965045
#define BLEND_SIZE  1964391
#define QUAL_SIZE   2749898
#define NUM_USERS   458293
#define NUM_MOVIES  17770
#define AVG_RATING  3.609516

// constants regarding learning
#define FEAT_ADJ_RATE 0.015
#define BIAS_ADJ_RATE 0.01
#define NUM_FEATS     150
#define NUM_EPOCHS    20
#define TIME_BINS     30
#define BIN_SIZE      75
#define BETA          0.4
#define GAMMA_1       0.007
#define GAMMA_2       0.007
#define GAMMA_STEP    0.9

// for splitting up data
#define TRAIN_SET  1
#define VALID_SET  2
#define BLEND_SET  3

// constants regarding clustering
#define K      35
#define ALPHA  0.1 // still figuring out what this is

// constants regarding rbm
#define NUM_FACTORS    50
#define NUM_RATINGS    5
#define LEARNING_RATE  0.1
#define MINIBATCH_SIZE 100
#define RBM_EPOCHS     5000
