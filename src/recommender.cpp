#include "recommender.hpp"

MatrixXd read_data(int rows, int cols, char * filename) {
    ifstream data(filename);
    MatrixXd vals(rows, cols);
    string line;
    int row = 0, col = 0;

    while (getline(data, line)) {
        stringstream lineStream(line);
        string cell;
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
