#pragma once

#include <string>

#include "exceptions/SportsClubException.h"

class PaymentException : public SportsClubException {
public:
    explicit PaymentException(const std::string& details) : SportsClubException("Payment error: " + details) {}
};
