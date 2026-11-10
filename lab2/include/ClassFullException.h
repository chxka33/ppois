#pragma once

#include <string>

#include "SportsClubException.h"

class ClassFullException : public SportsClubException {
public:
    explicit ClassFullException(const std::string& details) : SportsClubException("Group class is full: " + details) {}
};
