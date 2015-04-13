// to change between um and mu these next four lines must be changed
#define DATA_FILE  "data/mu/all.dta"
#define QUAL_FILE  "data/mu/qual.dta"
#define MOVIE_COL  0
#define USER_COL   1
#define TIME_COL   2
#define RATING_COL 3

// constants regarding data
#define DATA_SIZE  102416306
#define QUAL_SIZE  2749898
#define NUM_USERS  458293
#define NUM_MOVIES 17770

// constants regarding learning
#define NUM_FEATS  50
#define NUM_ITERS  50

#define BETA       0.4
#define GAMMA      0.3
#define SPLINE_POW 0.25
