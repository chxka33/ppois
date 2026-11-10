#pragma once

#include <string>

#include "SportsClubException.h"

class FacilityNotFoundException : public SportsClubException {
public:
    explicit FacilityNotFoundException(const std::string& details) : SportsClubException("Facility not found: " + details) {}
};
