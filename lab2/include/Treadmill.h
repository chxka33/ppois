#pragma once

#include <string>

#include "Equipment.h"

class Treadmill : public Equipment {
public:
    Treadmill(std::string inventoryNumber, std::string name, const Date& purchaseDate,
              double maintenanceIntervalHours, double maxSpeedKmh, double maxUserWeightKg);

    std::string getCategory() const override;

    double getMaxSpeedKmh() const;
    double getMaxUserWeightKg() const;
    double getCurrentSpeedKmh() const;

    bool isSuitableFor(double userWeightKg) const;
    void setSpeed(double speedKmh);
    void stop();

private:
    double maxSpeedKmh_;
    double maxUserWeightKg_;
    double currentSpeedKmh_;
};
