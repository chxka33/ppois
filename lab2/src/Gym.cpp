#include "Gym.h"

#include <algorithm>

#include "InvalidDataException.h"
#include "SportsClubException.h"

Gym::Gym(std::string id, std::string name, int capacity, AccessLevel requiredAccessLevel, double areaSquareMeters)
    : Facility(std::move(id), std::move(name), capacity, requiredAccessLevel),
      areaSquareMeters_(areaSquareMeters) {
    if (areaSquareMeters_ <= 0.0) {
        throw InvalidDataException("Area must be positive");
    }
}

FacilityType Gym::getType() const {
    return FacilityType::Gym;
}

double Gym::getAreaSquareMeters() const {
    return areaSquareMeters_;
}

const std::vector<std::unique_ptr<Equipment>>& Gym::getEquipment() const {
    return equipment_;
}

void Gym::addEquipment(std::unique_ptr<Equipment> equipment) {
    if (!equipment) {
        throw InvalidDataException("Equipment is null");
    }
    equipment_.push_back(std::move(equipment));
}

void Gym::removeEquipment(const std::string& inventoryNumber) {
    equipment_.erase(
        std::remove_if(equipment_.begin(), equipment_.end(),
                       [&inventoryNumber](const std::unique_ptr<Equipment>& eq) {
                           return eq && eq->getInventoryNumber() == inventoryNumber;
                       }),
        equipment_.end());
}

Equipment& Gym::findEquipment(const std::string& inventoryNumber) {
    for (auto& eq : equipment_) {
        if (eq && eq->getInventoryNumber() == inventoryNumber) {
            return *eq;
        }
    }
    throw SportsClubException("Equipment not found: " + inventoryNumber);
}

std::vector<Equipment*> Gym::getEquipmentNeedingMaintenance() const {
    std::vector<Equipment*> result;
    for (const auto& eq : equipment_) {
        if (eq && eq->needsMaintenance()) {
            result.push_back(eq.get());
        }
    }
    return result;
}

int Gym::countWorkingEquipment() const {
    int count = 0;
    for (const auto& eq : equipment_) {
        if (eq && eq->getCondition() != EquipmentCondition::Broken) {
            count++;
        }
    }
    return count;
}
