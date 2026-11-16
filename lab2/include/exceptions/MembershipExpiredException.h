#pragma once

#include <string>

#include "exceptions/SportsClubException.h"

class MembershipExpiredException : public SportsClubException {
public:
    explicit MembershipExpiredException(const std::string& details) : SportsClubException("Membership expired: " + details) {}
};
