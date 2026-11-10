#pragma once

#include <string>
#include <vector>

#include "Employee.h"

class Facility;

// Уборщик: отвечает за чистоту закреплённых объектов.
class Cleaner : public Employee {
public:
    Cleaner(std::string employeeId, std::string firstName, std::string lastName, const Date& birthDate,
            std::string phone, std::string email, double baseSalary, const Date& hireDate);

    std::string getPosition() const override;

    const std::vector<Facility*>& getAssignedFacilities() const;
    int getCleanedCount() const;

    void assignFacility(Facility& facility);
    bool isAssignedTo(const Facility& facility) const;
    void cleanFacility(Facility& facility, const Date& today);

private:
    std::vector<Facility*> assignedFacilities_;
    int cleanedCount_;
};
