#ifndef COMPUTE_CPP
#define COMPUTE_CPP
#include "compute.cpp"
#endif


double l2_norm(double * vec, int length);
void inplace_sub(double * vec1, double * vec2, int length);
void calculate_gradient(double * y, double ** x, double lambda,
    int n, int d, double * w, double * grad);
double * linear_regression(double * y, double ** x, double lambda,
    double tolerance, int n, int d);

double l2_norm(double * vec, int length) {
    double sum = 0;
    for (int i = 0; i < length; ++i) {
        sum += pow(vec[i], 2);
    }
    return pow(sum, 0.5);
}

void inplace_sub(double * vec1, double * vec2, int length) {
    for (int i = 0; i < length; ++i) {
        vec1[i] -= vec2[i];
    }
}

void calculate_gradient(double * y, double ** x, double lambda,
    int n, int d, double * w, double * grad) {

    double gradient;
    double * pred = new double[n];
    for (int i = 0; i < n; ++i){
      for (int j = 0; j < d; ++j){
        pred[i] += w[j] * x[j][i];
      }
    }

    for (int i = 0; i < d; ++i) {
        gradient = 0;
        // calculate -2x(y - w'x)'
        for (int j = 0; j < n; ++j) {
            gradient += -2 * x[i][j] * (y[j] - pred[j]);
        }

        // add 2 * lambda * w
        gradient += 2 * lambda * w[i];

        // set gradient
        grad[i] = gradient;
    }
    delete[] pred;
}

double * linear_regression(double * y, double ** x, double lambda,
    double tolerance, int n, int d) {

    int count = 0;
    // init weight vector to small random, gradient to small pos rands
    double * w = new double[d];
    double * grad = new double[d];
    for (int i = 0; i < d; ++i) {
        w[i] = small_rand();
        grad[i] = small_pos_rand();
    }

    // sgd till gradient is small enough
    do {
        count++;
        // calculate new gradient
        calculate_gradient(y, x, lambda, n, d, w, grad);
        // subtract gradient from weights
        inplace_sub(w, grad, d);

        printf("%f\n", l2_norm(grad, d));
    } while (l2_norm(grad, d) > tolerance);

    printf("Linear Regression epochs: %d\n", count);

    return w;
}
