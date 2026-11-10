#pragma once

#include <vector>

#include "Expense.h"
#include "Payment.h"

class MaintenanceRecord;

// Учёт доходов и расходов клуба.
class FinanceDepartment {
public:
    explicit FinanceDepartment(double initialBalance);

    double getBalance() const;
    const std::vector<Payment>& getPayments() const;
    const std::vector<Expense>& getExpenses() const;

    void registerPayment(const Payment& payment);
    void registerExpense(const Expense& expense);
    void registerMaintenance(const MaintenanceRecord& record);
    void refundPayment(const std::string& paymentId);
    double calculateIncome(const Date& from, const Date& to) const;
    double calculateExpenses(const Date& from, const Date& to) const;
    double calculateProfit(const Date& from, const Date& to) const;

private:
    double balance_;
    std::vector<Payment> payments_;
    std::vector<Expense> expenses_;
};
