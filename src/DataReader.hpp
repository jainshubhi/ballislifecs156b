#ifndef CONSTANTS_CPP
#define CONSTANTS_CPP
#include "constants.hpp"
#endif

using namespace std;

class DataReader {
public:
    // data holders
    int ** train_set;
    int ** valid_set;
    int ** blend_set;
    int ** qual_set;

    // inits by reading all data
    DataReader() {
        time_t start, end;
        time(&start);

        printf("Reading data...\n");

        this->train_set = new int*[TRAIN_SIZE];
        for (unsigned int i = 0; i < TRAIN_SIZE; ++i) {
            this->train_set[i] = new int[4];
        }

        this->blend_set = new int*[BLEND_SIZE];
        for (unsigned int i = 0; i < BLEND_SIZE; ++i) {
            this->blend_set[i] = new int[4];
        }

        this->valid_set = new int*[VALID_SIZE];
        for (unsigned int i = 0; i < VALID_SIZE; ++i) {
            this->valid_set[i] = new int[4];
        }

        ifstream data(DATA_FILE);
        ifstream idx(IDX_FILE);
        string line, idx_line;
        int current[4];
        int train_row = 0, blend_row = 0, valid_row = 0, data_set, col;

        while (getline(data, line)) {
            getline(idx, idx_line);
            stringstream lineStream(line);
            string cell;
            col = 0;
            data_set = atoi(idx_line.c_str());

            while (getline(lineStream, cell, ' ')) {
                current[col] = atoi(cell.c_str());
                col++;
            }

            // part of qual, ignore
            if (data_set > 3) {
                continue;
            }

            // go from 1-indexed to 0-indexed
            current[USER_COL] -= 1;
            current[MOVIE_COL] -= 1;

            switch (data_set) {
                case TRAIN_SET:
                    this->train_set[train_row][0] = current[0];
                    this->train_set[train_row][1] = current[1];
                    this->train_set[train_row][2] = current[2];
                    this->train_set[train_row][3] = current[3];
                    train_row++;
                    break;
                case VALID_SET:
                    this->valid_set[valid_row][0] = current[0];
                    this->valid_set[valid_row][1] = current[1];
                    this->valid_set[valid_row][2] = current[2];
                    this->valid_set[valid_row][3] = current[3];
                    valid_row++;
                    break;
                case BLEND_SET:
                    this->blend_set[blend_row][0] = current[0];
                    this->blend_set[blend_row][1] = current[1];
                    this->blend_set[blend_row][2] = current[2];
                    this->blend_set[blend_row][3] = current[3];
                    blend_row++;
                    break;
                default:
                    printf("something went wrong - got bad data set val\n");
                    continue;
                    break;
            }
        }

        time(&end);

        if (!((train_row == TRAIN_SIZE) && (blend_row == BLEND_SIZE)
            && (valid_row == VALID_SIZE))) {
            printf("something reading went wrong - sets are not correct size.\n");
            printf("train size is %d supposed to be %d.\n", train_row, TRAIN_SIZE);
            printf("blend size is %d supposed to be %d.\n", blend_row, BLEND_SIZE);
            printf("valid size is %d supposed to be %d.\n", valid_row, VALID_SIZE);
        }

        printf("All data read. Took %.f seconds.\n", difftime(end, start));
    }

    // deinits by deleting all arrays
    ~DataReader() {
        for (unsigned int i = 0; i < TRAIN_SIZE; ++i) {
            delete[] train_set[i];
        }
        for (unsigned int i = 0; i < VALID_SIZE; ++i) {
            delete[] valid_set[i];
        }
        for (unsigned int i = 0; i < BLEND_SIZE; ++i) {
            delete[] blend_set[i];
        }

        delete[] this->train_set;
        delete[] this->valid_set;
        delete[] this->blend_set;

        if (this->qual_set) {
            for (unsigned int i = 0; i < QUAL_SIZE; ++i) {
                delete[] this->qual_set[i];
            }
            delete[] this->qual_set;
        }
    }

    void read_qual() {
        int row = 0, col;
        ifstream data(QUAL_FILE);
        string line;

        // allocate qual_set
        this->qual_set = new int*[QUAL_SIZE];
        for (unsigned int i = 0; i < QUAL_SIZE; ++i) {
            this->qual_set[i] = new int[3];
        }

        // read data into qual_set
        while (getline(data, line)) {
            stringstream lineStream(line);
            string cell;
            col = 0;

            while (getline(lineStream, cell, ' ')) {
                this->qual_set[row][col] = atoi(cell.c_str());
                col++;
            }

            // go from 1-indexed to 0-indexed
            this->qual_set[row][USER_COL] -= 1;
            this->qual_set[row][MOVIE_COL] -= 1;

            row++;

        }
    }

private:
    // nothing
};
