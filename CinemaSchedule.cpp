#include "CinemaSchedule.h"

CinemaSchedule::CinemaSchedule(int capacity) : maxCapacity(capacity) {}

void CinemaSchedule::addMovie(Movie movie) {
    if (static_cast<int>(movies.size()) < maxCapacity) {
        movies.push_back(std::move(movie));
    }
}

void CinemaSchedule::removeMovie(int index) {
    if (index >= 0 && index < static_cast<int>(movies.size())) {
        movies.erase(movies.begin() + index);
    }
}

Movie* CinemaSchedule::getMovieAt(int index) {
    if (index >= 0 && index < static_cast<int>(movies.size())) {
        return &movies[index];
    }
    return nullptr;
}

int CinemaSchedule::getMovieCount() const {
    return static_cast<int>(movies.size());
}

void CinemaSchedule::clear() {
    movies.clear();
}
