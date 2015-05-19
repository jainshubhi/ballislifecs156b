#include "blender.hpp"

#define LAMBDA    .00001
#define TOLERANCE .1
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
        printf("Need at least two models in order to blend, and qual and blend predictions for each.\n");
        return 1;
    }

    int num_models = (argc - 1) / 2;

    // put all predictions into
    double ** qual_preds = new double*[num_models];
    double ** blend_preds = new double*[num_models];

    printf("Reading blend and qual sets...\n");
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
            blend_preds[i / 2] = new double[BLEND_SIZE];

            // read vector into array
            vector_read(string(argv[i + 1]), blend_preds[i / 2], BLEND_SIZE);
        }
    }

    printf("Reading actual ratings for blend data...\n");
    // get blend actual ratings into vector
    double * blend_actuals = new double[BLEND_SIZE];
    vector_read(BLEND_RATINGS, blend_actuals, BLEND_SIZE);

    printf("Training model coefficients...\n");
    // train coefficients using blend set
    double * coeff = linear_regression(
        /* y */                 blend_actuals,
        /* x */                 blend_preds,
        /* lambda */            LAMBDA,
        /* tolerance */         TOLERANCE,
        /* number of samples */ BLEND_SIZE,
        /* dimensions */        num_models);

    printf("Coefficients:\n");
    for (int i = 0; i < num_models; ++i) {
        printf("%s %f\n", argv[i * 2 + 1], coeff[i]);
    }

    printf("Predicting on qual...\n");
    // make predictions using coefficients and qual set and write to file
    ofstream out_file;
    out_file.open(OUT_FILE);

    double pred;
    for (int i = 0; i < QUAL_SIZE; ++i) {
        pred = 0;
        for (int j = 0; j < (argc - 1) / 2; ++j) {
            pred += qual_preds[j][i] * coeff[j];
        }
        pred = bound(pred);

        // write to file
        out_file << pred;
        out_file << "\n";
    }

    out_file.close();

    // deinit everything
    delete[] coeff;
    delete[] blend_actuals;

    for (int i = 0; i < num_models; ++i) {
        delete[] qual_preds[i];
        delete[] blend_preds[i];
    }

    delete[] qual_preds;
    delete[] blend_preds;

    printf("Done.\n");

    return 0;
}
