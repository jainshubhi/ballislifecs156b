// all includes
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

// to change between um and mu these next five lines must be changed
#define DATA_FILE  "data/mu/all.dta"
#define IDX_FILE   "data/mu/all.idx"
#define QUAL_FILE  "data/mu/qual.dta"
#define OUT_FILE   "predictions.txt"
#define MOVIE_COL  0
#define USER_COL   1
#define TIME_COL   2
#define RATING_COL 3

// files for learned values

// constants regarding data
#define DATA_SIZE   102416306
#define TRAIN_SIZE  94362233
#define VALID_SIZE  1965045
#define BLEND_SIZE  1964391
#define QUAL_SIZE   2749898
#define NUM_USERS   458293
#define NUM_MOVIES  17770

// constants regarding learning
#define NUM_FEATS  50
#define NUM_ITERS  50
#define BETA       0.4
#define GAMMA      0.3
#define SPLINE_POW 0.25

// for splitting up data
#define TRAIN_SET  1
#define VALID_SET  2
#define BLEND_SET  3
