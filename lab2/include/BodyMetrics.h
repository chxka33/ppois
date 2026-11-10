#pragma once

#include <string>

#include "Date.h"

// Антропометрический замер клиента.
class BodyMetrics {
public:
    BodyMetrics(const Date& date, double weightKg, double heightCm, double bodyFatPercent);

    const Date& getDate() const;
    double getWeightKg() const;
    double getHeightCm() const;
    double getBodyFatPercent() const;

    double calculateBmi() const;
    std::string getBmiCategory() const;
    double calculateLeanMassKg() const;

private:
    Date date_;
    double weightKg_;
    double heightCm_;
    double bodyFatPercent_;
};
