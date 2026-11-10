#pragma once

#include <string>

#include "Facility.h"

class Sauna : public Facility {
public:
    Sauna(std::string id, std::string name, int capacity, AccessLevel requiredAccessLevel, double temperature,
          int maxSessionMinutes);

    FacilityType getType() const override;

    double getTemperature() const;
    int getMaxSessionMinutes() const;

    void setTemperature(double temperature);
    bool isTemperatureSafe() const;
    bool isSessionAllowed(int minutes) const;

private:
    double temperature_;
    int maxSessionMinutes_;
};
