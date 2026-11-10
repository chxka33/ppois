#pragma once

#include <vector>

#include "BodyMetrics.h"

// История замеров и цель клиента по весу.
class ProgressTracker {
public:
    ProgressTracker();
    explicit ProgressTracker(double targetWeightKg);

    void addMeasurement(const BodyMetrics& metrics);
    const BodyMetrics& getLatest() const;
    const std::vector<BodyMetrics>& getHistory() const;
    double getWeightChange() const;
    bool isGoalReached() const;
    void setTargetWeight(double targetWeightKg);
    double getTargetWeight() const;

private:
    std::vector<BodyMetrics> history_;
    double targetWeightKg_;
};
