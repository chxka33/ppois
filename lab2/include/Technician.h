#pragma once

#include <string>

#include "Employee.h"
#include "MaintenanceRecord.h"

class Equipment;

// Техник: обслуживает и ремонтирует тренажёры.
class Technician : public Employee {
public:
    Technician(std::string employeeId, std::string firstName, std::string lastName, const Date& birthDate,
               std::string phone, std::string email, double baseSalary, const Date& hireDate,
               std::string qualification);

    std::string getPosition() const override;

    const std::string& getQualification() const;
    int getRepairsCount() const;

    bool inspect(const Equipment& equipment) const;
    MaintenanceRecord repair(Equipment& equipment, const Date& date, double cost, const std::string& description);

private:
    std::string qualification_;
    int repairsCount_;
};
