#pragma once

#include <string>

#include "SportsClubException.h"

class FacilityFullException : public SportsClubException {
public:
    explicit FacilityFullException(const std::string& details) : SportsClubException("Facility is full: " + details) {}
};
