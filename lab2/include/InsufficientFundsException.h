#pragma once

#include <string>

#include "SportsClubException.h"

class InsufficientFundsException : public SportsClubException {
public:
    InsufficientFundsException(double required, double available)
        : SportsClubException("Insufficient funds: required " + std::to_string(required) + ", available " +
                              std::to_string(available)),
          required_(required),
          available_(available) {}

    double getRequired() const { return required_; }
    double getAvailable() const { return available_; }

private:
    double required_;
    double available_;
};
