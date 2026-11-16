#include "MaintenanceRecord.h"

#include "Equipment.h"
#include "InvalidDataException.h"

MaintenanceRecord::MaintenanceRecord(Equipment* equipment, std::string technicianId,
                                     const Date& date, double cost, std::string description)
    : equipment_(equipment), technicianId_(std::move(technicianId)), date_(date),
      cost_(cost), description_(std::move(description)) {
    if (cost_ < 0.0) {
        throw InvalidDataException("Maintenance cost cannot be negative");
    }
}

Equipment* MaintenanceRecord::getEquipment() const { return equipment_; }
const std::string& MaintenanceRecord::getTechnicianId() const { return technicianId_; }
const Date& MaintenanceRecord::getDate() const { return date_; }
double MaintenanceRecord::getCost() const { return cost_; }
const std::string& MaintenanceRecord::getDescription() const { return description_; }

bool MaintenanceRecord::isWithinPeriod(const Date& from, const Date& to) const {
    return from <= date_ && date_ <= to;
}

std::string MaintenanceRecord::toString() const {
    std::string eqName = equipment_ ? equipment_->getName() : "Unknown";
    return date_.toString() + " [" + technicianId_ + "] " + eqName + ": " + description_ + " (cost: " + std::to_string(cost_) + ")";
}
