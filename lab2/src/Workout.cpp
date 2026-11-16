#include "Workout.h"

#include "InvalidDataException.h"

Workout::Workout(std::string name, Intensity intensity, int durationMinutes, double caloriesPerMinutePerKg)
    : name_(std::move(name)), intensity_(intensity),
      durationMinutes_(durationMinutes), caloriesPerMinutePerKg_(caloriesPerMinutePerKg) {
    if (name_.empty() || durationMinutes_ <= 0 || caloriesPerMinutePerKg_ < 0.0) {
        throw InvalidDataException("Invalid workout parameters");
    }
}

const std::string& Workout::getName() const { return name_; }
Intensity Workout::getIntensity() const { return intensity_; }
int Workout::getDurationMinutes() const { return durationMinutes_; }
double Workout::getCaloriesPerMinutePerKg() const { return caloriesPerMinutePerKg_; }

double Workout::estimateCalories(double userWeightKg) const {
    if (userWeightKg <= 0.0) return 0.0;
    return caloriesPerMinutePerKg_ * userWeightKg * durationMinutes_;
}

bool Workout::requiresHighIntensityClearance() const {
    return intensity_ == Intensity::High;
}
