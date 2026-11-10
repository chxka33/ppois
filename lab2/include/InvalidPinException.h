#pragma once

#include <string>

#include "SportsClubException.h"

class InvalidPinException : public SportsClubException {
public:
    explicit InvalidPinException(const std::string& details) : SportsClubException("Invalid PIN: " + details) {}
};
