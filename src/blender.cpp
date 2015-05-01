#include "blender.hpp"

/*
 * Our blender works as follows:
 * Users are split into low volume raters, and high volume raters.
 * Movies are split into low volume rated, and high volume rated.
 * Blending coefficients are trained separately on the four resulting sets:
 * high user - high movie
 * high user - low movie
 * low user - high movie
 * low user - low movie
 *
 * Then, these new blending coefficients for each set are used to predict.
 */

int main(int argc, const char * argv[]) {
/*
    // read in all model predictions on blend_set and qual_set
    // svd

    // knn

    // rbm

    // read blend counts
    int * blend_count = new int[4];
    vector_read_int(BLEND_COUNTS, blend_count, 4);

    // get counts for users and movies from files
    int * user_rating = new int[NUM_USERS];
    int * movie_rating = new int[NUM_MOVIES];
    // TODO read files

    // iterate through data putting it in separate bins
    int ** hh = new int*[blend_count[0]];
    int ** hl = new int*[blend_count[1]];
    int ** lh = new int*[blend_count[2]];
    int ** ll = new int*[blend_count[3]];

    for (unsigned int i = 0; i < blend_count[0]; ++i) {
        hh[i] = new int[NUM_MODELS];
    }

    for (unsigned int i = 0; i < blend_count[1]; ++i) {
        hl[i] = new int[NUM_MODELS];
    }

    for (unsigned int i = 0; i < blend_count[2]; ++i) {
        lh[i] = new int[NUM_MODELS];
    }

    for (unsigned int i = 0; i < blend_count[3]; ++i) {
        ll[i] = new int[NUM_MODELS];
    }

    int user, movie, date, hh_ind = 0, hl_ind = 0, lh_ind = 0, ll_ind = 0;
    for (unsigned int i = 0; i < BLEND_SIZE; ++i) {
        user = reader->blend_set[i][USER_COL];
        movie = reader->blend_set[i][MOVIE_COL];
        if (count_user_rating[user] > median_user_count) {
            if (count_movie_rating[movie] > median_movie_count) {
                hh[hh_ind][USER_COL] = user;
                hh[hh_ind][MOVIE_COL] = movie;
                hh_ind++;
            }
            else {
                hh[hh_ind][USER_COL] = user;
                hh[hh_ind][MOVIE_COL] = movie;
                hl_ind++;
            }
        }
        else {
            if (count_movie_rating[movie] > median_movie_count) {
                hh[hh_ind][USER_COL] = user;
                hh[hh_ind][MOVIE_COL] = movie;
                lh_ind++;
            }
            else {
                hh[hh_ind][USER_COL] = user;
                hh[hh_ind][MOVIE_COL] = movie;
                ll_ind++;
            }
        }
    }

    // run classification of each bin
*/
    return 0;
}
