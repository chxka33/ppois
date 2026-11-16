#pragma once

#include <string>

#include "exceptions/SportsClubException.h"

class MedicalClearanceException : public SportsClubException {
public:
    explicit MedicalClearanceException(const std::string& details) : SportsClubException("No medical clearance: " + details) {}
};
