#pragma once

#include <string>

#include "SportsClubException.h"

class EquipmentBrokenException : public SportsClubException {
public:
    explicit EquipmentBrokenException(const std::string& details) : SportsClubException("Equipment is broken: " + details) {}
};
