#include "ProgressTracker.h"

#include "InvalidDataException.h"

ProgressTracker::ProgressTracker() : targetWeightKg_(0.0) {}

ProgressTracker::ProgressTracker(double targetWeightKg) : targetWeightKg_(targetWeightKg) {
    if (targetWeightKg < 0.0) {
        throw InvalidDataException("Target weight cannot be negative");
    }
}

void ProgressTracker::addMeasurement(const BodyMetrics& metrics) {
    history_.push_back(metrics);
}

const BodyMetrics& ProgressTracker::getLatest() const {
    if (history_.empty()) {
        throw InvalidDataException("No measurements recorded");
    }
    return history_.back();
}

const std::vector<BodyMetrics>& ProgressTracker::getHistory() const {
    return history_;
}

double ProgressTracker::getWeightChange() const {
    if (history_.size() < 2) {
        return 0.0;
    }
    return history_.back().getWeightKg() - history_.front().getWeightKg();
}

bool ProgressTracker::isGoalReached() const {
    if (history_.empty() || targetWeightKg_ <= 0.0) {
        return false;
    }
    double initial = history_.front().getWeightKg();
    double current = history_.back().getWeightKg();
    if (initial >= targetWeightKg_) {
        return current <= targetWeightKg_;
    } else {
        return current >= targetWeightKg_;
    }
}

void ProgressTracker::setTargetWeight(double targetWeightKg) {
    if (targetWeightKg < 0.0) {
        throw InvalidDataException("Target weight cannot be negative");
    }
    targetWeightKg_ = targetWeightKg;
}

double ProgressTracker::getTargetWeight() const {
    return targetWeightKg_;
}
