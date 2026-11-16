#include "members/BodyMetrics.h"

#include "exceptions/InvalidDataException.h"

BodyMetrics::BodyMetrics(const Date& date, double weightKg, double heightCm, double bodyFatPercent)
    : date_(date), weightKg_(weightKg), heightCm_(heightCm), bodyFatPercent_(bodyFatPercent) {
    if (weightKg <= 0.0 || heightCm <= 0.0 || bodyFatPercent < 0.0 || bodyFatPercent > 100.0) {
        throw InvalidDataException("Invalid body metrics values");
    }
}

const Date& BodyMetrics::getDate() const { return date_; }
double BodyMetrics::getWeightKg() const { return weightKg_; }
double BodyMetrics::getHeightCm() const { return heightCm_; }
double BodyMetrics::getBodyFatPercent() const { return bodyFatPercent_; }

double BodyMetrics::calculateBmi() const {
    double heightM = heightCm_ / 100.0;
    return weightKg_ / (heightM * heightM);
}

std::string BodyMetrics::getBmiCategory() const {
    double bmi = calculateBmi();
    if (bmi < 18.5) {
        return "Underweight";
    } else if (bmi < 25.0) {
        return "Normal";
    } else if (bmi < 30.0) {
        return "Overweight";
    } else {
        return "Obese";
    }
}

double BodyMetrics::calculateLeanMassKg() const {
    return weightKg_ * (1.0 - bodyFatPercent_ / 100.0);
}
