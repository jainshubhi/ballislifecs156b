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
    if ((argc <= 5) || (argc % 2 == 0)) {
        printf("Need at least two models in order to blend, and qual and blend
            predictions for each.\n");
        return 1;
    }

    int num_models = (argc - 1) / 2;

    // put all predictions into
    double ** qual_preds = new double*[num_models];
    double ** blend_preds = new double*[num_models];

    for (int i = 0; i < argc - 1; ++i) {
        // even ones are qual
        if (i % 2 == 0) {
            // allocate array for predictions on qual set
            qual_preds[i / 2] = new double[QUAL_SIZE];

            // read vector into array
            vector_read(string(argv[i + 1]), qual_preds[i / 2], QUAL_SIZE);
        }
        // odd ones are blend
        else {
            // allocate array for predictions on blend set
            blend_preds[(i / 2) + 1] = new double[BLEND_SIZE];

            // read vector into array
            vector_read(string(argv[i + 1]), blend_preds[i / 2], BLEND_SIZE);
        }
    }

    // get blend actual ratings into vector
    // TODO create BLEND_RATINGS file
    double blend_actuals = new double[BLEND_SIZE];
    vector_read(BLEND_RATINGS, blend_actuals, BLEND_SIZE);

    // train coefficients use blend set
    double * coeffs = linear_regression(blend_preds, blend_actuals,
        /* lambda */            0.0001,
        /* tolerance */         0.01,
        /* number of samples */ BLEND_SIZE,
        /* dimensions */        num_models);

    // make predictions using coefficients and qual set and write to file
    ofstream out_file;
    out_file.open(OUT_FILE);

    double pred;
    for (int i = 0; i < QUAL_SIZE; ++i) {
        pred = 0;
        for (int j = 0; j < (argc - 1) / 2; ++j) {
            pred += qual_preds[i] * coeffs[j];
        }
        pred = bound(pred);

        // write to file
        out_file << pred;
        out_file << "\n";
    }

    out_file.close();

    // deinit everything
    delete[] coeffs;
    delete[] blend_actuals;

    for (int i = 0; i < num_models; ++i) {
        delete[] qual_preds[i];
        delete[] blend_preds[i];
    }

    delete[] qual_preds;
    delete[] blend_preds;

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
        if (count_user_rating[user] > mean_user_count) {
            if (count_movie_rating[movie] > mean_movie_count) {
                // put model info in hh[hh_ind][0 ... NUM_MODELS - 1]
                hh_ind++;
            }
            else {
                // put model info in hl[hl_ind][0 ... NUM_MODELS - 1]
                hl_ind++;
            }
        }
        else {
            if (count_movie_rating[movie] > mean_movie_count) {
                // put model info in lh[lh_ind][0 ... NUM_MODELS - 1]
                lh_ind++;
            }
            else {
                // put model info in ll[ll_ind][0 ... NUM_MODELS - 1]
                ll_ind++;
            }
        }
    }

    // run classification of each bin
*/
    return 0;
}
