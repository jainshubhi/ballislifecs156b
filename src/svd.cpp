#include "svd.hpp"

int main() {
    // read data
    DataReader reader;

    // learn parameters
    // TODO

    // write parameters to files
    // TODO

    return 1;
}

// Constructor
SvdLearner::SvdLearner() {
   // Initialize U & Y
   this->U = new double*[NUM_USERS];
   this->Y = new double*[NUM_USERS];
   this->V = new double*[NUM_MOVIES];
   for(unsigned int i = 0; i < NUM_USERS; ++i) {
       this->U[i] = new double[NUM_FEATS];
       this->Y[i] = new double[NUM_MOVIES];
   }

   // Initialize V
   for(unsigned int i = 0; i < NUM_MOVIES; ++i) {
       this->V[i] = new double[NUM_FEATS];
   }
}

// Destructor
SvdLearner::~SvdLearner() {

    for (unsigned int i = 0; i < NUM_USERS; ++i) {
        delete[] this->U[i];
        delete[] this->Y[i];

    }

    for (unsigned int i = 0; i < NUM_MOVIES; ++i) {
        delete[] this->V[i];
    }

    for (unsigned int i = 0; i < data.size(); ++i) {
        delete data[i];
    }
    delete[] this->U;
    delete[] this->Y;
    delete[] this->V;
}

/* Input
 * lambda is regularization constant
 * limit is the
 * Output
 * Notes
 * NUM_FEATS is the number of latent factors
 */
void SvdLearner::svd(double lambda, double limit) {
    // Part 1: Finding Y = UV'
    // number of users = NUM_USERS, number of movies = NUM_MOVIES
    // Loop through the data set.
    for(unsigned int i = 0; i < data.size(); ++i) {
        DataPoint *val = data[i];
        this->Y[val->user][val->movie] = val->rating;
    }

    // Initialize U and V randomly
    for(unsigned int i = 0; i < NUM_USERS; ++i) {
        for (unsigned int k = 0; k < NUM_FEATS; ++k) {
            this->U[i][k] = small_rand();
            // if(i < NUM_MOVIES)
            //     V[i][k] = small_rand();
        }
    }

    // Which is faster?
    for(unsigned int j = 0; j < NUM_MOVIES; ++j) {
        for(unsigned int k = 0; k < NUM_FEATS; ++k) {
            this->V[j][k] = small_rand();
        }
    }

    // Optimize U and V using CGD, it's more accurate than SGD
    // int rand_i =
    // double* sum_u = [];
    // for(unsigned int j = 0; j < NUM_MOVIES; ++j) {
    //     sum_u += multiply(V[j], )
    // }
    // grad_u = lambda * u_i - ;
    // grad_v = ;

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


}
