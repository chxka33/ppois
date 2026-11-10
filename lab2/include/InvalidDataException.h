#pragma once

#include <string>

#include "SportsClubException.h"

class InvalidDataException : public SportsClubException {
public:
    explicit InvalidDataException(const std::string& details) : SportsClubException("Invalid data: " + details) {}
};
