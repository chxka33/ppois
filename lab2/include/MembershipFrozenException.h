#pragma once

#include <string>

#include "SportsClubException.h"

class MembershipFrozenException : public SportsClubException {
public:
    explicit MembershipFrozenException(const std::string& details) : SportsClubException("Membership frozen: " + details) {}
};
