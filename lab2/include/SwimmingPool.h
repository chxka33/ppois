#pragma once

#include <string>

#include "Facility.h"

class SwimmingPool : public Facility {
public:
    SwimmingPool(std::string id, std::string name, int capacity, AccessLevel requiredAccessLevel, int laneCount,
                 double depthMeters, double waterTemperature);

    FacilityType getType() const override;

    int getLaneCount() const;
    int getReservedLanes() const;
    double getDepthMeters() const;
    double getWaterTemperature() const;

    void setWaterTemperature(double temperature);
    bool isTemperatureSafe() const;
    void reserveLane();
    void releaseLane();
    int getFreeLanes() const;

private:
    int laneCount_;
    int reservedLanes_;
    double depthMeters_;
    double waterTemperature_;
};
