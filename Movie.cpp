#include "Movie.h"

Movie::Movie(const std::string& title, int duration)
    : title(title), durationMinutes(duration) {}

std::string Movie::getTitle() const {
    return title;
}

int Movie::getDurationMinutes() const {
    return durationMinutes;
}

int Movie::getSessionCount() const {
    return schedule.count();
}

int Movie::getTotalDuration() const {
    return getSessionCount() * durationMinutes;
}

void Movie::addSessionToSchedule(const std::tm& dateTime) {
    schedule.addSession(dateTime);
}

void Movie::removeSessionAt(int index) {
    schedule.removeSessionAt(index);
}

std::vector<std::tm> Movie::getSessionTimes() const {
    return schedule.getAllSessions();
}
