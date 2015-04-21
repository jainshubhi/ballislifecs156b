#include "svd.hpp"

int main() {
    // read data
    DataReader reader;

    // learn parameters
    SvdLearner svd;
    svd.set_data(reader.train_set);
    // svd.train();
    // TODO

    // write parameters to files
    svd.write(SVD_FILE);
    // TODO

    return 1;
}

// Constructor
SvdLearner::SvdLearner() {
   // Initialize U, V
   this->U = new double*[NUM_USERS];
   this->V = new double*[NUM_MOVIES];

   for(unsigned int i = 0; i < NUM_USERS; ++i) {
       this->U[i] = new double[NUM_FEATS];
   }

   // Initialize V
   for(unsigned int i = 0; i < NUM_MOVIES; ++i) {
       this->V[i] = new double[NUM_FEATS];
   }
}

// Destructor
SvdLearner::~SvdLearner() {
    // Delete U and Y
    for (unsigned int i = 0; i < NUM_USERS; ++i) {
        delete[] this->U[i];
        delete[] this->Y[i];
    }

    // Delete V
    for (unsigned int i = 0; i < NUM_MOVIES; ++i) {
        delete[] this->V[i];
    }
    // Delete data
    for (unsigned int i = 0; i < data.size(); ++i) {
        delete data[i];
    }
    delete[] this->U;
    delete[] this->Y;
    delete[] this->V;
}

// sets the data to the passed value
void SVDLearner::set_data(vector<DataPoint *> data) {
    this->data = data;
}

/* Input
 * limit is the
 * Output
 * Notes
 * NUM_FEATS is the number of latent factors
 */
void SvdLearner::init(double limit) {
    // Initialize U and V to small random numbers
    for(unsigned int i = 0; i < NUM_USERS; ++i) {
        for (unsigned int k = 0; k < NUM_FEATS; ++k) {
            this->U[i][k] = small_rand();
        }
    }

    for(unsigned int j = 0; j < NUM_MOVIES; ++j) {
        for(unsigned int k = 0; k < NUM_FEATS; ++k) {
            this->V[j][k] = small_rand();
        }
    }
}

/* Input
 * lambda is our regularization factor
 * i is the chosen user
 * j is the chosen movie
 * Output
 * Notes
 * NUM_FEATS is the number of latent factors
 * This SVD is for a given user and movie.
 */
void SvdLearner::svd(double lambda, int i, int j, double le_dot) {
    for(unsigned int k = 0; k < NUM_FEATS; ++k) {
        // Can also have a learning rate here.
        // This value of V/U may be incorrect respectively
        // update user feature vector
        U[i][k] -= lambda * U[i][k] - V[j][k] * (Y[i][j] - le_dot);
        // update movie feature vector
        V[j][k] -= lambda * V[j][k] - U[i][k] * (Y[i][j] - le_dot);
    }
}
/* Inputf
 * norm_val is the stopping criteria
 * Output
 * Notes
 * Performs SVD
 */
void SvdLearner::train(double norm_val) {
    while(frobenius_norm(U, NUM_USERS, NUM_FEATS) > norm_val) { // this norm may be incorrect
        for(unsigned int i = 0; i < NUM_USERS; ++i) {
            for(unsigned int j = 0; j < NUM_MOVIES; ++j) {
                // For a given user, i and movie, j.
                double le_dot = dot(U[i], V[j], NUM_FEATS);
                svd(LAMBDA, i, j, le_dot);
            }
        }
    }
}

// calculates SUM(r - UV') + lambda * (||U|| + ||V||)
void SvdLearner::error() {

}

void SvdLearner::gradient() {

}

// do SGD
void SvdLearner::train(double lambda) {
    for (unsigned int i = 0; i < NUM_EPOCHS; ++i) {
        // calculate gradients
        this->gradient_U();
        this->gradient_V();

        // subtract gradients to descend
        this->U = matrix_sub(this->U, this->gradU);
        this->V = matrix_sub(this->V, this->gradV);
    }
}

    // next_U = new_U(Y, V, lambda);
    // n = norm((U - next_U), 'fro');
    // while n > 1.1
    //     U = next_U;
    //     V = new_V(Y, U, lambda);
    //     next_U = new_U(Y, V, lambda);
    //     n = norm((U - next_U), 'fro');
    // end
    //
    // U = next_U;
    // V = new_V(Y, U, lambda);
    //
    // // Part 2: Projecting U and V onto 2 dimensions.
    // %% Compute the SVD of both U and V.
    // [Au, Su, Bu] = svd(U, 'econ');
    // [Av, Sv, Bv] = svd(V, 'econ');
    //
    // %% Reduce U and V to highest 2 dimensions.
    // U2d = Au(1:2, :) * U';
    // V2d = Av(1:2, :) * V';

void SvdLearner::write(string filename) {

}
