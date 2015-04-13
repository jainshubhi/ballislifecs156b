#include "recommender.hpp"

MatrixXi read_data(int rows, int cols, string filename) {
    ifstream data(filename);
    MatrixXi vals(rows, cols);
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

void Learner::set_data(MatrixXi data) {
    this->data = data;
}

void Learner::set_qual(MatrixXi qual) {
    this->qual = qual;
}

void Learner::train() {
    // nead to write this
}

void Learner::predict() {
    // need to write this
}

int main() {
    Learner * learner = new Learner();
    learner->set_data(read_data(DATA_SIZE, 4, DATA_FILE));
    learner->set_qual(read_data(QUAL_SIZE, 3, QUAL_FILE));
    learner->train();
    learner->test();

    return 0;
}
