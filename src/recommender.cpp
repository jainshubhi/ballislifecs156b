#include "recommender.hpp"

MatrixXd read_file() {
    std::ifstream data("../data/mu/all.dta");
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

int main() {
    MatrixXd a = read_file();

    for (int i = 0; i < 100; i++) {
        std::cout << a(i, 2) << std::endl;
    }
    return 0;
}
