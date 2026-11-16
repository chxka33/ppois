#include "Sauna.h"

#include "InvalidDataException.h"

Sauna::Sauna(std::string id, std::string name, int capacity, AccessLevel requiredAccessLevel,
             double temperature, int maxSessionMinutes)
    : Facility(std::move(id), std::move(name), capacity, requiredAccessLevel),
      temperature_(temperature), maxSessionMinutes_(maxSessionMinutes) {
    if (maxSessionMinutes_ <= 0) {
        throw InvalidDataException("Max session minutes must be positive");
    }
}

FacilityType Sauna::getType() const {
    return FacilityType::Sauna;
}

double Sauna::getTemperature() const { return temperature_; }
int Sauna::getMaxSessionMinutes() const { return maxSessionMinutes_; }

void Sauna::setTemperature(double temperature) {
    temperature_ = temperature;
}

bool Sauna::isTemperatureSafe() const {
    return temperature_ >= 60.0 &&
           temperature_ <= 110.0;
}

bool Sauna::isSessionAllowed(int minutes) const {
    return minutes > 0 && minutes <= maxSessionMinutes_;
}
