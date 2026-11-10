#pragma once

#include <string>

#include "Date.h"

class Equipment;

// Запись об обслуживании оборудования.
class MaintenanceRecord {
public:
    MaintenanceRecord(Equipment* equipment, std::string technicianId, const Date& date, double cost,
                      std::string description);

    Equipment* getEquipment() const;
    const std::string& getTechnicianId() const;
    const Date& getDate() const;
    double getCost() const;
    const std::string& getDescription() const;

    bool isWithinPeriod(const Date& from, const Date& to) const;
    std::string toString() const;

private:
    Equipment* equipment_;
    std::string technicianId_;
    Date date_;
    double cost_;
    std::string description_;
};
