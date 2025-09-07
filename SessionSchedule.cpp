#include "SessionSchedule.h"

void SessionSchedule::addSession(const std::tm& dateTime) {
    sessions.push_back(dateTime);
}

void SessionSchedule::removeSessionAt(int index) {
    if (index >= 0 && index < static_cast<int>(sessions.size())) {
        sessions.erase(sessions.begin() + index);
    }
}

int SessionSchedule::count() const {
    return static_cast<int>(sessions.size());
}

std::vector<std::tm> SessionSchedule::getAllSessions() const {
    return sessions;
}
