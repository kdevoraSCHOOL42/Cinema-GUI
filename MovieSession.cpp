#include "MovieSession.h"

MovieSession::MovieSession(const std::tm& dt) : dateTime(dt) {}

std::tm MovieSession::getDateTime() const {
    return dateTime;
}
