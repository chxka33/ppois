#pragma once

#include <string>

#include "Date.h"
#include "Enums.h"

class Expense {
public:
    Expense(ExpenseCategory category, double amount, const Date& date, std::string description);

    ExpenseCategory getCategory() const;
    double getAmount() const;
    const Date& getDate() const;
    const std::string& getDescription() const;

    bool isInPeriod(const Date& from, const Date& to) const;

private:
    ExpenseCategory category_;
    double amount_;
    Date date_;
    std::string description_;
};
