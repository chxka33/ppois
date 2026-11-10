#pragma once

#include <string>

#include "Date.h"

// Разовый гостевой пропуск.
class GuestPass {
public:
    GuestPass(std::string code, std::string guestName, const Date& validDate, double price);

    const std::string& getCode() const;
    const std::string& getGuestName() const;
    const Date& getValidDate() const;
    double getPrice() const;
    bool isUsed() const;

    bool isValidOn(const Date& date) const;
    void use(const Date& today);

private:
    std::string code_;
    std::string guestName_;
    Date validDate_;
    double price_;
    bool used_;
};
