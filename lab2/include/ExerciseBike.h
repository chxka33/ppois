#pragma once

#include <string>

#include "Equipment.h"

class ExerciseBike : public Equipment {
public:
    ExerciseBike(std::string inventoryNumber, std::string name, const Date& purchaseDate,
                 double maintenanceIntervalHours, int resistanceLevels);

    std::string getCategory() const override;

    int getResistanceLevels() const;
    int getCurrentLevel() const;

    void increaseResistance();
    void decreaseResistance();
    double estimateCaloriesPerMinute(double userWeightKg) const;

private:
    int resistanceLevels_;
    int currentLevel_;
};
