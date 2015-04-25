#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#ifndef DATAREADER_HPP
#define DATAREADER_HPP
#include "DataReader.hpp"
#endif

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP
#include "constants.hpp"
#endif

using namespace std;

// returns random double between -0.01 and 0.01
double small_rand() {
    int num = ((int) rand()) % 100;
    return 2.0 * num / 10000.0 - 0.01;
}

// returns positive random double between 0 and 0.02
double small_pos_rand() {
    int num = ((int) rand()) % 100;
    return 2.0 * num / 10000.0;
}

// returns +1 if positve, -1 if negative, 0 if zero
int sign(double x) {
    if (x < 0) return -1;
    if (x > 0) return 1;
    return 0;
}

// returns 1 / sqrt(x)
double inv_sqrt(double x) {
    if (x < 0) printf("fuck\n");
    return 1.0 / sqrt(x);
}

// calculates time deviation cost
double dev_ut(int date, double avg_date) {
    return sign(date - avg_date) * pow(abs(date - avg_date), BETA);
}

// bounds the prediction to between 1 and 5
double bound(double x) {
    if (x > 5) return 5;
    if (x < 1) return 1;
    return x;
}

// check that a number refers to a real user
bool check_user(int user) {
    if ((user >= 0) && (user < NUM_USERS)) {
        return true;
    }
    return false;
}

// check that a number refers to a real movie
bool check_movie(int movie) {
    if ((movie >= 0) && (movie < NUM_MOVIES)) {
        return true;
    }
    return false;
}

// tranpose a matrix of doubles
// double** transpose(double** matrix, int r, int c) {
//     double **t = new double*[c];
//     for(unsigned int i = 0; i < c; ++i) {
//         t[i] = new double[r];
//         for(unsigned int j = 0; j < r; ++j) {
//             t[i][j] = matrix[j][i];
//         }
//     }
//     return t;
// }


// dot product two vectors of doubles
double dot(double* vec_1, double* vec_2, unsigned int length) {
    double product = 0.0;
    for (unsigned int i = 0; i < length; ++i) {
        product += vec_1[i] * vec_2[i];
    }
    return product;
}

// Multiplication of two matrices of doubles
// Not very fast O(n^3)
// double** multiply(double** vec_1, double** vec_2, int r_1, int c, int c_2) {
//     double** res = new double*[r_1];
//     for(unsigned int i = 0; i < r_1; ++i) {
//         res[i] = new double[c_2];
//         for(unsigned int j = 0; j < c_2; ++j) {
//             double* temp = new double[c];
//             for(unsigned int k = 0; k < c; ++k) {
//                 temp[k] = vec_2[k][j];
//             }
//             res[i][j] = dot(vec_1[i], temp, c);
//         }
//     }
//     return res;
// }

double frobenius_norm(double ** matrix, unsigned int r, unsigned int c) {
    double sum = 0.0;
    for (unsigned int i = 0; i < r; ++i) {
        for (unsigned int j = 0; j < c; ++j) {
            double val = matrix[i][j];
            sum += val * val;
        }
    }
    return sqrt(sum);
}

// matrix subtraction of two double matrices
double ** matrix_sub(double ** mat_1, double ** mat_2, unsigned int r, unsigned int c) {
    double ** res = new double*[r];
    for (unsigned int i = 0; i < r; ++i) {
        res[i] = new double[c];
        for (unsigned int j = 0; j < c; ++j) {
            res[i][j] = mat_1[i][j] - mat_2[i][j];
        }
    }
    return res;
}

// read a double matrix from a file
void matrix_read(string filename, double ** mat, unsigned int rows, unsigned int cols) {
    ifstream data(filename);
    string line;
    unsigned int r = 0, c = 0;

    while (getline(data, line)) {
        stringstream lineStream(line);
        string cell;

        if (r == rows) {
            printf("error: number of rows is greater than provided matrix\n");
        }

        while (getline(lineStream, cell, ' ')) {
            if (c == cols) {
                printf("error: number of columns is greater than provided matrix\n");
            }

            mat[r][c] = atof(cell.c_str());
            ++c;
        }
        c = 0;
        ++r;
    }
}

// read an int matrix from a file
void matrix_read_int(string filename, int ** mat, unsigned int rows, unsigned int cols) {
    ifstream data(filename);
    string line;
    unsigned int r = 0, c = 0;

    while (getline(data, line)) {
        stringstream lineStream(line);
        string cell;

        if (r == rows) {
            printf("error: number of rows is greater than provided matrix\n");
        }

        while (getline(lineStream, cell, ' ')) {
            if (c == cols) {
                printf("error: number of columns is greater than provided matrix\n");
            }

            mat[r][c] = atoi(cell.c_str());
            ++c;
        }
        c = 0;
        ++r;
    }
}

// write a double matrix into a file
void matrix_write(string filename, double ** mat, unsigned int rows, unsigned int cols) {
    ofstream out_file;
    out_file.open(filename);

    for (unsigned int i = 0; i < rows; ++i) {
        for (unsigned int j = 0; j < cols; ++j) {
            out_file << mat[i][j];
            out_file << " ";
        }
        out_file << "\n";
    }
    out_file.close();
}

// write an int matrix into a file
void matrix_write_int(string filename, int ** mat, unsigned int rows, unsigned int cols) {
    ofstream out_file;
    out_file.open(filename);

    for (unsigned int i = 0; i < rows; ++i) {
        for (unsigned int j = 0; j < cols; ++j) {
            out_file << mat[i][j];
            out_file << " ";
        }
        out_file << "\n";
    }
    out_file.close();
}

// write a double matrix with variable col lengths into a file
void matrix_write_var(string filename, double ** mat, unsigned int rows, int * cols) {
    ofstream out_file;
    out_file.open(filename);

    for (unsigned int i = 0; i < rows; ++i) {
        for (unsigned int j = 0; j < cols[i]; ++j) {
            out_file << mat[i][j];
            out_file << " ";
        }
        out_file << "\n";
    }
    out_file.close();
}

// write an int matrix with variable col lengths into a file
void matrix_write_var_int(string filename, int ** mat, unsigned int rows, int * cols) {
    ofstream out_file;
    out_file.open(filename);

    for (unsigned int i = 0; i < rows; ++i) {
        for (unsigned int j = 0; j < cols[i]; ++j) {
            out_file << mat[i][j];
            out_file << " ";
        }
        out_file << "\n";
    }
    out_file.close();
}

// read a double vector from a file
void vector_read(string filename, double * vec, unsigned int length) {
    ifstream data(filename);
    string line;
    unsigned int i = 0;

    while (getline(data, line)) {
        if (i == length) {
            printf("error: length is greater than provided vector\n");
        }

        vec[i] = atof(line.c_str());
        ++i;
    }
}

// read an int vector from a file
void vector_read_int(string filename, int * vec, unsigned int length) {
    ifstream data(filename);
    string line;
    unsigned int i = 0;

    while (getline(data, line)) {
        if (i == length) {
            printf("error: length is greater than provided vector\n");
        }

        vec[i] = atoi(line.c_str());
        ++i;
    }
}

// write a double vector into a file
void vector_write(string filename, double * vec, unsigned int length) {
    ofstream out_file;
    out_file.open(filename);

    for (unsigned int i = 0; i < length; ++i) {
        out_file << vec[i];
        out_file << "\n";
    }
    out_file.close();
}

// write an int vector into a file
void vector_write_int(string filename, int * vec, unsigned int length) {
    ofstream out_file;
    out_file.open(filename);

    for (unsigned int i = 0; i < length; ++i) {
        out_file << vec[i];
        out_file << "\n";
    }
    out_file.close();
}
