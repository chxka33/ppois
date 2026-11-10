#pragma once

#include <string>
#include <vector>

#include "Date.h"
#include "Enums.h"

class Member;

// Абстрактный объект клуба (зал, бассейн, корт и т. д.).
class Facility {
public:
    Facility(std::string id, std::string name, int capacity, AccessLevel requiredAccessLevel);
    virtual ~Facility() = default;

    const std::string& getId() const;
    const std::string& getName() const;
    int getCapacity() const;
    int getOccupancy() const;
    bool isOpen() const;
    AccessLevel getRequiredAccessLevel() const;
    const Date& getLastCleaned() const;

    virtual FacilityType getType() const = 0;

    void enter(const Member& member, const Date& today);
    void enterGuest();
    void leave(const std::string& visitorId);
    bool hasFreeSpace() const;
    bool isInside(const std::string& visitorId) const;
    double getLoadPercent() const;
    void open();
    void close();
    void markCleaned(const Date& date);

protected:
    std::string id_;
    std::string name_;
    int capacity_;
    int occupancy_;
    bool open_;
    AccessLevel requiredAccessLevel_;
    Date lastCleaned_;
    std::vector<std::string> visitorIds_;
};
