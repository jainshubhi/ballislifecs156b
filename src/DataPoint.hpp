#include "constants.hpp"

using namespace std;

// class for each data point
class DataPoint {
public:
    // info about a point
    int user;
    int movie;
    int rating;
    int date;

    // values constructor for data point
    DataPoint(int user, int movie, int rating, int date) {
        this->user = user;
        this->movie = movie;
        this->rating = rating;
        this->date = date;
    }

    // vector constructor for data point
    DataPoint(vector<int> point) {
        this->movie = point[MOVIE_COL];
        this->user = point[USER_COL];
        this->date = point[TIME_COL];

        if (point.size() == 3) {
            this->rating = 0;
        }

        else if (point.size() == 4) {
            this->rating = point[RATING_COL];
        }
    }

    // destructor for data point
    ~DataPoint() {
        // do nothing
    }

private:
    // nothing
};
