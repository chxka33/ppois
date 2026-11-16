#include "Equipment.h"

#include "EquipmentBrokenException.h"
#include "InvalidDataException.h"

Equipment::Equipment(std::string inventoryNumber, std::string name, const Date& purchaseDate,
                     double maintenanceIntervalHours)
    : inventoryNumber_(std::move(inventoryNumber)), name_(std::move(name)),
      purchaseDate_(purchaseDate), condition_(EquipmentCondition::New),
      usageHours_(0.0), hoursSinceMaintenance_(0.0),
      maintenanceIntervalHours_(maintenanceIntervalHours) {
    if (inventoryNumber_.empty() || name_.empty() || maintenanceIntervalHours_ <= 0.0) {
        throw InvalidDataException("Invalid equipment parameters");
    }
}

const std::string& Equipment::getInventoryNumber() const { return inventoryNumber_; }
const std::string& Equipment::getName() const { return name_; }
const Date& Equipment::getPurchaseDate() const { return purchaseDate_; }
EquipmentCondition Equipment::getCondition() const { return condition_; }
double Equipment::getUsageHours() const { return usageHours_; }
double Equipment::getHoursSinceMaintenance() const { return hoursSinceMaintenance_; }

void Equipment::use(double hours) {
    if (condition_ == EquipmentCondition::Broken) {
        throw EquipmentBrokenException("Cannot use broken equipment: " + inventoryNumber_);
    }
    if (hours < 0.0) {
        throw InvalidDataException("Usage hours must be non-negative");
    }
    usageHours_ += hours;
    hoursSinceMaintenance_ += hours;
    if (hoursSinceMaintenance_ >= maintenanceIntervalHours_) {
        condition_ = EquipmentCondition::Worn;
    } else if (condition_ == EquipmentCondition::New && usageHours_ > 0.0) {
        condition_ = EquipmentCondition::Good;
    }
}

bool Equipment::needsMaintenance() const {
    return condition_ == EquipmentCondition::Broken ||
           condition_ == EquipmentCondition::Worn ||
           hoursSinceMaintenance_ >= maintenanceIntervalHours_;
}

void Equipment::markBroken() {
    condition_ = EquipmentCondition::Broken;
}

void Equipment::repair() {
    condition_ = EquipmentCondition::Good;
    hoursSinceMaintenance_ = 0.0;
}

int Equipment::getAgeYears(const Date& today) const {
    return purchaseDate_.yearsUntil(today);
}
