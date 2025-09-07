#ifndef MOVIESESSION_H
#define MOVIESESSION_H

#include <ctime>

class MovieSession {
public:
    MovieSession(const std::tm& dateTime);
    std::tm getDateTime() const;

private:
    std::tm dateTime;
};

#endif // MOVIESESSION_H
