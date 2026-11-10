#pragma once

#include <string>

#include "Enums.h"

// Тип тренировки (йога, кроссфит, аквааэробика...).
class Workout {
public:
    Workout(std::string name, Intensity intensity, int durationMinutes, double caloriesPerMinutePerKg);

    const std::string& getName() const;
    Intensity getIntensity() const;
    int getDurationMinutes() const;
    double getCaloriesPerMinutePerKg() const;

    double estimateCalories(double userWeightKg) const;
    bool requiresHighIntensityClearance() const;

private:
    std::string name_;
    Intensity intensity_;
    int durationMinutes_;
    double caloriesPerMinutePerKg_;
};
