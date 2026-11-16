#pragma once

#include <string>

#include "exceptions/SportsClubException.h"

class InvalidDateException : public SportsClubException {
public:
    explicit InvalidDateException(const std::string& details) : SportsClubException("Invalid date: " + details) {}
};
