#ifndef CONSTANTS_CPP
#define CONSTANTS_CPP
#include "constants.hpp"
#endif

#ifndef DATAPOINT_HPP
#define DATAPOINT_HPP
#include "DataPoint.hpp"
#endif

using namespace std;

class DataReader {
public:
    // data holders
    vector<DataPoint *> train_set;
    vector<DataPoint *> valid_set;
    vector<DataPoint *> blend_set;

    // inits by reading all data
    DataReader() {
        ifstream data(DATA_FILE);
        ifstream idx(IDX_FILE);
        string line, idx_line;
        vector<int> current;
        int data_set;

        while (getline(data, line)) {
            getline(idx, idx_line);
            stringstream lineStream(line);
            string cell;
            current.clear();
            data_set = atoi(idx_line.c_str());

            // part of qual, ignore
            if (data_set > 3) {
                continue;
            }

            while (getline(lineStream, cell, ' ')) {
                current.push_back(atoi(cell.c_str()));
            }
            DataPoint * new_point = new DataPoint(current);
            switch (data_set) {
                case TRAIN_SET:
                    this->train_set.push_back(new_point);
                    break;
                case VALID_SET:
                    this->valid_set.push_back(new_point);
                    break;
                case BLEND_SET:
                    this->blend_set.push_back(new_point);
                    break;
                default:
                    delete new_point;
                    printf("something went wrong, got bad data set val\n");
                    break;
            }
        }
    }

    // deinits by deleting all DataPoint objects
    ~DataReader() {
        for (unsigned int i = 0; i < train_set.size(); ++i) {
            delete train_set[i];
        }
        for (unsigned int i = 0; i < valid_set.size(); ++i) {
            delete valid_set[i];
        }
        for (unsigned int i = 0; i < blend_set.size(); ++i) {
            delete blend_set[i];
        }
    }

private:
    // nothing
};
