#pragma once

#include <string>

#include "Facility.h"

class TennisCourt : public Facility {
public:
    TennisCourt(std::string id, std::string name, AccessLevel requiredAccessLevel, CourtSurface surface,
                bool indoor, double hourlyRate);

    FacilityType getType() const override;

    CourtSurface getSurface() const;
    bool isIndoor() const;
    double getHourlyRate() const;

    void setHourlyRate(double hourlyRate);
    double calculateRentCost(double hours, bool isMember) const;
    bool isPlayableInWeather(bool isRaining) const;

private:
    CourtSurface surface_;
    bool indoor_;
    double hourlyRate_;
};
