#pragma once

#include <string>

#include "Date.h"
#include "Enums.h"

// Базовый класс спортивного оборудования.
class Equipment {
public:
    Equipment(std::string inventoryNumber, std::string name, const Date& purchaseDate,
              double maintenanceIntervalHours);
    virtual ~Equipment() = default;

    const std::string& getInventoryNumber() const;
    const std::string& getName() const;
    const Date& getPurchaseDate() const;
    EquipmentCondition getCondition() const;
    double getUsageHours() const;
    double getHoursSinceMaintenance() const;

    virtual std::string getCategory() const = 0;

    void use(double hours);
    bool needsMaintenance() const;
    void markBroken();
    void repair();
    int getAgeYears(const Date& today) const;

protected:
    std::string inventoryNumber_;
    std::string name_;
    Date purchaseDate_;
    EquipmentCondition condition_;
    double usageHours_;
    double hoursSinceMaintenance_;
    double maintenanceIntervalHours_;
};
