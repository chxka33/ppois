#include "Technician.h"

#include "Equipment.h"
#include "InvalidDataException.h"

Technician::Technician(std::string employeeId, std::string firstName, std::string lastName,
                       const Date& birthDate, std::string phone, std::string email,
                       double baseSalary, const Date& hireDate, std::string qualification)
    : Employee(std::move(employeeId), std::move(firstName), std::move(lastName), birthDate,
               std::move(phone), std::move(email), baseSalary, hireDate),
      qualification_(std::move(qualification)), repairsCount_(0) {
    if (qualification_.empty()) {
        throw InvalidDataException("Qualification cannot be empty");
    }
}

std::string Technician::getPosition() const {
    return "Technician";
}

const std::string& Technician::getQualification() const {
    return qualification_;
}

int Technician::getRepairsCount() const {
    return repairsCount_;
}

bool Technician::inspect(const Equipment& equipment) const {
    return equipment.needsMaintenance();
}

MaintenanceRecord Technician::repair(Equipment& equipment, const Date& date, double cost, const std::string& description) {
    equipment.repair();
    repairsCount_++;
    return MaintenanceRecord(&equipment, employeeId_, date, cost, description);
}
