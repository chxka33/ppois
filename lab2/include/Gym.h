#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Equipment.h"
#include "Facility.h"

// Тренажёрный зал с оборудованием.
class Gym : public Facility {
public:
    Gym(std::string id, std::string name, int capacity, AccessLevel requiredAccessLevel, double areaSquareMeters);

    FacilityType getType() const override;

    double getAreaSquareMeters() const;
    const std::vector<std::unique_ptr<Equipment>>& getEquipment() const;

    void addEquipment(std::unique_ptr<Equipment> equipment);
    void removeEquipment(const std::string& inventoryNumber);
    Equipment& findEquipment(const std::string& inventoryNumber);
    std::vector<Equipment*> getEquipmentNeedingMaintenance() const;
    int countWorkingEquipment() const;

private:
    double areaSquareMeters_;
    std::vector<std::unique_ptr<Equipment>> equipment_;
};
