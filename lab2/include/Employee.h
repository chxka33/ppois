#pragma once

#include <string>

#include "Person.h"

// Абстрактный сотрудник клуба.
class Employee : public Person {
public:
    Employee(std::string employeeId, std::string firstName, std::string lastName, const Date& birthDate,
             std::string phone, std::string email, double baseSalary, const Date& hireDate);

    const std::string& getEmployeeId() const;
    double getBaseSalary() const;
    const Date& getHireDate() const;
    bool isActive() const;
    double getWorkedHours() const;

    virtual std::string getPosition() const = 0;
    virtual double calculateSalary() const;

    int getExperienceYears(const Date& today) const;
    void logWorkHours(double hours);
    void resetWorkHours();
    void raiseSalary(double percent);
    void terminate();

protected:
    std::string employeeId_;
    double baseSalary_;
    Date hireDate_;
    bool active_;
    double workedHours_;
};
