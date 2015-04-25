#include "blender.hpp"

int main(int argc, const char * argv[]) {
    /* ++++++++++++++++++++++

    // read data
    DataReader reader;
    reader.read_qual();
    vector<double> svd_resp, temp_resp, knn_resp, final_resp;
    double coef_svd = 0.0, coef_temp = 0.0, coef_knn = 0.0;

    // initialize model objects and get data from model files
    Svd      svd(SVD_FILE);
    Temporal temporal(TEMP_FILE);
    Knn      knn(KNN_FILE);

    // make predictions on blender set using all models
    svd_resp  = svd.predict(reader.blend_set);
    temp_resp = temporal.predict(reader.blend_set);
    knn_resp  = knn.predict(reader.blend_set);

    // blend and choose coefficients
    // figure out what learning technique will be used
    // TODO

    // predict on qual set
    svd_resp  = svd.predict(reader.qual_set);
    temp_resp = temporal.predict(reader.qual_set);
    knn_resp  = knn.predict(reader.qual_set);

    for (unsigned int i = 0; i < reader.qual_set.size(); ++i) {
        final_resp.push_back(coef_svd * svd_resp[i]
                           + coef_temp * temp_resp[i]
                           + coef_knn * knn_resp[i]);
    }

    // write predictions to outfile
    ofstream out_file;
    out_file.open(OUT_FILE);

    for (unsigned int i = 0; i < final_resp.size(); ++i) {
        out_file << final_resp[i];
        out_file << "\n";
    }
    out_file.close();

    ++++++++++++++++++++++ */
    return 1;
}
