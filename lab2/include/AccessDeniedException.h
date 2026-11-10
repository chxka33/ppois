#pragma once

#include <string>

#include "SportsClubException.h"

class AccessDeniedException : public SportsClubException {
public:
    explicit AccessDeniedException(const std::string& details) : SportsClubException("Access denied: " + details) {}
};
