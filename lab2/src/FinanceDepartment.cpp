#include "FinanceDepartment.h"

#include "InsufficientFundsException.h"
#include "MaintenanceRecord.h"
#include "PaymentException.h"

FinanceDepartment::FinanceDepartment(double initialBalance) : balance_(initialBalance) {}

double FinanceDepartment::getBalance() const { return balance_; }
const std::vector<Payment>& FinanceDepartment::getPayments() const { return payments_; }
const std::vector<Expense>& FinanceDepartment::getExpenses() const { return expenses_; }

void FinanceDepartment::registerPayment(const Payment& payment) {
    payments_.push_back(payment);
    if (payment.isCompleted()) {
        balance_ += payment.getAmount();
    }
}

void FinanceDepartment::registerExpense(const Expense& expense) {
    if (expense.getAmount() > balance_) {
        throw InsufficientFundsException(expense.getAmount(), balance_);
    }
    expenses_.push_back(expense);
    balance_ -= expense.getAmount();
}

void FinanceDepartment::registerMaintenance(const MaintenanceRecord& record) {
    Expense expense(ExpenseCategory::Maintenance, record.getCost(), record.getDate(), record.getDescription());
    registerExpense(expense);
}

void FinanceDepartment::refundPayment(const std::string& paymentId) {
    for (auto& p : payments_) {
        if (p.getId() == paymentId) {
            if (p.getAmount() > balance_) {
                throw InsufficientFundsException(p.getAmount(), balance_);
            }
            p.refund();
            balance_ -= p.getAmount();
            return;
        }
    }
    throw PaymentException("Payment not found: " + paymentId);
}

double FinanceDepartment::calculateIncome(const Date& from, const Date& to) const {
    double income = 0.0;
    for (const auto& p : payments_) {
        if (p.isCompleted() && from <= p.getDate() && p.getDate() <= to) {
            income += p.getAmount();
        }
    }
    return income;
}

double FinanceDepartment::calculateExpenses(const Date& from, const Date& to) const {
    double total = 0.0;
    for (const auto& e : expenses_) {
        if (e.isInPeriod(from, to)) {
            total += e.getAmount();
        }
    }
    return total;
}

double FinanceDepartment::calculateProfit(const Date& from, const Date& to) const {
    return calculateIncome(from, to) - calculateExpenses(from, to);
}
