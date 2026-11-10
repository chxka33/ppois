#pragma once

#include <string>

#include "SportsClubException.h"

class BookingConflictException : public SportsClubException {
public:
    explicit BookingConflictException(const std::string& details) : SportsClubException("Booking conflict: " + details) {}
};
