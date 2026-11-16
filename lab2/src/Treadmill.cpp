#include "Treadmill.h"

#include "EquipmentBrokenException.h"
#include "InvalidDataException.h"

Treadmill::Treadmill(std::string inventoryNumber, std::string name, const Date& purchaseDate,
                     double maintenanceIntervalHours, double maxSpeedKmh, double maxUserWeightKg)
    : Equipment(std::move(inventoryNumber), std::move(name), purchaseDate, maintenanceIntervalHours),
      maxSpeedKmh_(maxSpeedKmh), maxUserWeightKg_(maxUserWeightKg), currentSpeedKmh_(0.0) {
    if (maxSpeedKmh_ <= 0.0 || maxUserWeightKg_ <= 0.0) {
        throw InvalidDataException("Treadmill speed and weight limits must be positive");
    }
}

std::string Treadmill::getCategory() const {
    return "Treadmill";
}

double Treadmill::getMaxSpeedKmh() const { return maxSpeedKmh_; }
double Treadmill::getMaxUserWeightKg() const { return maxUserWeightKg_; }
double Treadmill::getCurrentSpeedKmh() const { return currentSpeedKmh_; }

bool Treadmill::isSuitableFor(double userWeightKg) const {
    return userWeightKg <= maxUserWeightKg_;
}

void Treadmill::setSpeed(double speedKmh) {
    if (condition_ == EquipmentCondition::Broken) {
        throw EquipmentBrokenException("Treadmill is broken");
    }
    if (speedKmh < 0.0 || speedKmh > maxSpeedKmh_) {
        throw InvalidDataException("Speed out of range");
    }
    currentSpeedKmh_ = speedKmh;
}

void Treadmill::stop() {
    currentSpeedKmh_ = 0.0;
}
