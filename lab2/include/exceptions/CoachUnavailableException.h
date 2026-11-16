#pragma once

#include <string>

#include "exceptions/SportsClubException.h"

class CoachUnavailableException : public SportsClubException {
public:
    explicit CoachUnavailableException(const std::string& details) : SportsClubException("Coach unavailable: " + details) {}
};
