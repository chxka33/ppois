#pragma once

#include <string>

#include "SportsClubException.h"

class EmployeeNotFoundException : public SportsClubException {
public:
    explicit EmployeeNotFoundException(const std::string& details) : SportsClubException("Employee not found: " + details) {}
};
