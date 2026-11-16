#include "Expense.h"

#include "InvalidDataException.h"

Expense::Expense(ExpenseCategory category, double amount, const Date& date, std::string description)
    : category_(category), amount_(amount), date_(date), description_(std::move(description)) {
    if (amount_ < 0.0) {
        throw InvalidDataException("Expense amount cannot be negative");
    }
}

ExpenseCategory Expense::getCategory() const { return category_; }
double Expense::getAmount() const { return amount_; }
const Date& Expense::getDate() const { return date_; }
const std::string& Expense::getDescription() const { return description_; }

bool Expense::isInPeriod(const Date& from, const Date& to) const {
    return from <= date_ && date_ <= to;
}
