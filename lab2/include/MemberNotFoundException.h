#pragma once

#include <string>

#include "SportsClubException.h"

class MemberNotFoundException : public SportsClubException {
public:
    explicit MemberNotFoundException(const std::string& details) : SportsClubException("Member not found: " + details) {}
};
