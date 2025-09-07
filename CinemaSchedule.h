#ifndef CINEMASCHEDULE_H
#define CINEMASCHEDULE_H

#include <vector>
#include "Movie.h"

class CinemaSchedule {
public:
    CinemaSchedule(int capacity);
    void addMovie(Movie movie);
    void removeMovie(int index);
    Movie* getMovieAt(int index);
    int getMovieCount() const;
    void clear();

private:
    std::vector<Movie> movies;
    int maxCapacity;
};

#endif // CINEMASCHEDULE_H
