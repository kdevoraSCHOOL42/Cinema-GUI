#ifndef SESSIONSCHEDULE_H
#define SESSIONSCHEDULE_H

#include <vector>
#include <ctime>

class SessionSchedule {
public:
    void addSession(const std::tm& dateTime);
    void removeSessionAt(int index);
    int count() const;
    std::vector<std::tm> getAllSessions() const;

private:
    std::vector<std::tm> sessions;
};

#endif // SESSIONSCHEDULE_H
