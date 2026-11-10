#pragma once

#include <vector>

class Employee;
class FinanceDepartment;
class Date;

// Расчёт и выплата зарплаты сотрудникам.
class Payroll {
public:
    explicit Payroll(double taxRate);

    double getTaxRate() const;
    double getTotalPaid() const;

    double calculateTax(const Employee& employee) const;
    double calculateNetSalary(const Employee& employee) const;
    void payEmployee(Employee& employee, FinanceDepartment& finance, const Date& date);
    void payAll(const std::vector<Employee*>& employees, FinanceDepartment& finance, const Date& date);

private:
    double taxRate_;
    double totalPaid_;
};
