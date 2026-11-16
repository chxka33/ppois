#include "facilities/TennisCourt.h"

#include "exceptions/InvalidDataException.h"

TennisCourt::TennisCourt(std::string id, std::string name, AccessLevel requiredAccessLevel,
                         CourtSurface surface, bool indoor, double hourlyRate)
    : Facility(std::move(id), std::move(name), 4, requiredAccessLevel),
      surface_(surface), indoor_(indoor), hourlyRate_(hourlyRate) {
    if (hourlyRate_ < 0.0) {
        throw InvalidDataException("Hourly rate cannot be negative");
    }
}

FacilityType TennisCourt::getType() const {
    return FacilityType::TennisCourt;
}

CourtSurface TennisCourt::getSurface() const { return surface_; }
bool TennisCourt::isIndoor() const { return indoor_; }
double TennisCourt::getHourlyRate() const { return hourlyRate_; }

void TennisCourt::setHourlyRate(double hourlyRate) {
    if (hourlyRate < 0.0) {
        throw InvalidDataException("Hourly rate cannot be negative");
    }
    hourlyRate_ = hourlyRate;
}

double TennisCourt::calculateRentCost(double hours, bool isMember) const {
    if (hours <= 0.0) return 0.0;
    double cost = hourlyRate_ * hours;
    if (isMember) {
        cost *= (1.0 - 0.2);
    }
    return cost;
}

bool TennisCourt::isPlayableInWeather(bool isRaining) const {
    if (indoor_) {
        return true;
    }
    return !isRaining;
}
