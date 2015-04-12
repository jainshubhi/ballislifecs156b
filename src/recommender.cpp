#include "recommender.hpp"

#define DATA_FILE "data/mu/all.dta"
#define QUAL_FILE "data/mu/qual.dta"
#define DATA_SIZE 102416306
#define QUAL_SIZE 2749898
#define NUM_FEATS 50
#define NUM_ITERS 50
#define NUM_USERS 1
#define NUM_MOVIES 1

MatrixXd read_data(int rows, int cols, char * filename) {
    std::ifstream data(filename);
    MatrixXd vals(rows, cols);
    std::string line;
    int row = 0, col = 0;

    while (getline(data,line)) {
        std::stringstream lineStream(line);
        std::string cell;
        while (getline(lineStream, cell, ' ')) {
            vals(row, col) = atoi(cell.c_str());
            ++col;
        }
        ++row;
        col = 0;
    }
    return vals;
}

Learner::Learner() {
    // do nothing
}

Learner::~Learner() {
    // do nothing
}

void Learner::train() {
    int a = 1;
}

void Learner::test() {
    int a = 1;
}

int main() {
    Learner * learner = new Learner();
    learner->data = read_data(DATA_SIZE, 4, DATA_FILE);
    learner->qual = read_data(QUAL_SIZE, 3, QUAL_FILE);
    learner->train();
    learner->test();

    return 0;
}
