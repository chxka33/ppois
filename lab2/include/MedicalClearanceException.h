#pragma once

#include <string>

#include "SportsClubException.h"

class MedicalClearanceException : public SportsClubException {
public:
    explicit MedicalClearanceException(const std::string& details) : SportsClubException("No medical clearance: " + details) {}
};
