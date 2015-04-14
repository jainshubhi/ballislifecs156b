#include "DataPoint.hpp"

// constructor for data point
DataPoint::DataPoint(int user, int movie, int rating, int date) {
    this->user = user;
    this->movie = movie;
    this->rating = rating;
    this->date = date;
}

// constructor for data point
DataPoint::DataPoint(vector<int> point) {
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
DataPoint::~DataPoint() {
    // do nothing
}
