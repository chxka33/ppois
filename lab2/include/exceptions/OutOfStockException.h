#pragma once

#include <string>

#include "exceptions/SportsClubException.h"

class OutOfStockException : public SportsClubException {
public:
    explicit OutOfStockException(const std::string& details) : SportsClubException("Out of stock: " + details) {}
};
