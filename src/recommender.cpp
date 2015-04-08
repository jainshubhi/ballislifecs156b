#include "recommender.hpp"

#define DATA_FILE "data/mu/all.dta"
#define QUAL_FILE "data/mu/qual.dta"

MatrixXd read_data() {
    std::ifstream data(DATA_FILE);
    MatrixXd vals(102416306, 4);
    std::string line;
    int row = 0, col = 0;

    while (std::getline(data,line)) {
        std::stringstream lineStream(line);
        std::string cell;
        while (std::getline(lineStream, cell, ' ')) {
            vals(row, col) = atoi(cell.c_str());
            ++col;
        }
        ++row;
        col = 0;
    }
    return vals;
}

MatrixXd read_qual() {
    std::ifstream data(QUAL_FILE);
    MatrixXd vals(2749898, 3);
    std::string line;
    int row = 0, col = 0;

    while (std::getline(data,line)) {
        std::stringstream lineStream(line);
        std::string cell;
        while (std::getline(lineStream, cell, ' ')) {
            vals(row, col) = atoi(cell.c_str());
            ++col;
        }
        ++row;
        col = 0;
    }
    return vals;
}

int main() {
    MatrixXd data = read_data();

    return 0;
}
