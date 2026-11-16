#include "Cleaner.h"

#include <algorithm>

#include "Facility.h"

Cleaner::Cleaner(std::string employeeId, std::string firstName, std::string lastName,
                 const Date& birthDate, std::string phone, std::string email,
                 double baseSalary, const Date& hireDate)
    : Employee(std::move(employeeId), std::move(firstName), std::move(lastName), birthDate,
               std::move(phone), std::move(email), baseSalary, hireDate),
      cleanedCount_(0) {}

std::string Cleaner::getPosition() const {
    return "Cleaner";
}

const std::vector<Facility*>& Cleaner::getAssignedFacilities() const {
    return assignedFacilities_;
}

int Cleaner::getCleanedCount() const {
    return cleanedCount_;
}

void Cleaner::assignFacility(Facility& facility) {
    if (!isAssignedTo(facility)) {
        assignedFacilities_.push_back(&facility);
    }
}

bool Cleaner::isAssignedTo(const Facility& facility) const {
    return std::find(assignedFacilities_.begin(), assignedFacilities_.end(), &facility) != assignedFacilities_.end();
}

void Cleaner::cleanFacility(Facility& facility, const Date& today) {
    facility.markCleaned(today);
    cleanedCount_++;
}
