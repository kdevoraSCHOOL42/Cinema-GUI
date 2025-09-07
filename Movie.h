#ifndef MOVIE_H
#define MOVIE_H

#include <string>
#include <vector>
#include <ctime>
#include "SessionSchedule.h"

class Movie {
public:
    Movie(const std::string& title, int duration);
    std::string getTitle() const;
    int getDurationMinutes() const;
    int getSessionCount() const;
    int getTotalDuration() const;
    void addSessionToSchedule(const std::tm& dateTime);
    void removeSessionAt(int index);
    std::vector<std::tm> getSessionTimes() const;

private:
    std::string title;
    int durationMinutes;
    SessionSchedule schedule;
};

#endif // MOVIE_H
