#include "finance/Payroll.h"

#include "common/Date.h"
#include "staff/Employee.h"
#include "finance/FinanceDepartment.h"
#include "exceptions/InvalidDataException.h"

Payroll::Payroll(double taxRate) : taxRate_(taxRate), totalPaid_(0.0) {
    if (taxRate_ < 0.0 || taxRate_ > 1.0) {
        throw InvalidDataException("Tax rate must be between 0 and 1");
    }
}

double Payroll::getTaxRate() const { return taxRate_; }
double Payroll::getTotalPaid() const { return totalPaid_; }

double Payroll::calculateTax(const Employee& employee) const {
    return employee.calculateSalary() * taxRate_;
}

double Payroll::calculateNetSalary(const Employee& employee) const {
    double gross = employee.calculateSalary();
    return gross - (gross * taxRate_);
}

void Payroll::payEmployee(Employee& employee, FinanceDepartment& finance, const Date& date) {
    if (!employee.isActive()) {
        return;
    }
    double gross = employee.calculateSalary();
    double net = calculateNetSalary(employee);
    employee.resetWorkHours();
    totalPaid_ += net;
    Expense expense(ExpenseCategory::Salary, gross, date, "Salary payment for " + employee.getFullName());
    finance.registerExpense(expense);
}

void Payroll::payAll(const std::vector<Employee*>& employees, FinanceDepartment& finance, const Date& date) {
    for (auto* emp : employees) {
        if (emp) {
            payEmployee(*emp, finance, date);
        }
    }
}
