#include "facilities/SwimmingPool.h"

#include "exceptions/BookingConflictException.h"
#include "exceptions/InvalidDataException.h"

SwimmingPool::SwimmingPool(std::string id, std::string name, int capacity,
                           AccessLevel requiredAccessLevel, int laneCount,
                           double depthMeters, double waterTemperature)
    : Facility(std::move(id), std::move(name), capacity, requiredAccessLevel),
      laneCount_(laneCount), reservedLanes_(0), depthMeters_(depthMeters),
      waterTemperature_(waterTemperature) {
    if (laneCount_ <= 0 || depthMeters_ <= 0.0) {
        throw InvalidDataException("Invalid swimming pool parameters");
    }
}

FacilityType SwimmingPool::getType() const {
    return FacilityType::SwimmingPool;
}

int SwimmingPool::getLaneCount() const { return laneCount_; }
int SwimmingPool::getReservedLanes() const { return reservedLanes_; }
double SwimmingPool::getDepthMeters() const { return depthMeters_; }
double SwimmingPool::getWaterTemperature() const { return waterTemperature_; }

void SwimmingPool::setWaterTemperature(double temperature) {
    waterTemperature_ = temperature;
}

bool SwimmingPool::isTemperatureSafe() const {
    return waterTemperature_ >= 24.0 &&
           waterTemperature_ <= 30.0;
}

void SwimmingPool::reserveLane() {
    if (getFreeLanes() <= 0) {
        throw BookingConflictException("No free lanes available in pool");
    }
    reservedLanes_++;
}

void SwimmingPool::releaseLane() {
    if (reservedLanes_ > 0) {
        reservedLanes_--;
    }
}

int SwimmingPool::getFreeLanes() const {
    return laneCount_ - reservedLanes_;
}
