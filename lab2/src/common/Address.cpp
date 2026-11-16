#include "common/Address.h"

#include <algorithm>
#include <cctype>

#include "exceptions/InvalidDataException.h"

Address::Address(std::string city, std::string street, std::string building, std::string postalCode)
    : city_(std::move(city)), street_(std::move(street)), building_(std::move(building)), postalCode_(std::move(postalCode)) {
    if (city_.empty() || street_.empty() || building_.empty() || !isValidPostalCode(postalCode_)) {
        throw InvalidDataException("Invalid address data");
    }
}

const std::string& Address::getCity() const { return city_; }
const std::string& Address::getStreet() const { return street_; }
const std::string& Address::getBuilding() const { return building_; }
const std::string& Address::getPostalCode() const { return postalCode_; }

bool Address::isSameCity(const Address& other) const {
    return city_ == other.city_;
}

bool Address::isValidPostalCode(const std::string& postalCode) {
    if (postalCode.empty() || postalCode.length() < 5 || postalCode.length() > 6) {
        return false;
    }
    return std::all_of(postalCode.begin(), postalCode.end(), [](unsigned char c) {
        return std::isdigit(c);
    });
}

std::string Address::toString() const {
    return postalCode_ + ", " + city_ + ", " + street_ + ", " + building_;
}
