#pragma once

#include <string>

class Address {
public:
    Address(std::string city, std::string street, std::string building, std::string postalCode);

    const std::string& getCity() const;
    const std::string& getStreet() const;
    const std::string& getBuilding() const;
    const std::string& getPostalCode() const;

    bool isSameCity(const Address& other) const;
    static bool isValidPostalCode(const std::string& postalCode);
    std::string toString() const;

private:
    std::string city_;
    std::string street_;
    std::string building_;
    std::string postalCode_;
};
