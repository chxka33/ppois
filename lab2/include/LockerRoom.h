#pragma once

#include <string>
#include <vector>

#include "Facility.h"
#include "Locker.h"

class LockerRoom : public Facility {
public:
    LockerRoom(std::string id, std::string name, int capacity, AccessLevel requiredAccessLevel, int lockerCount);

    FacilityType getType() const override;

    const std::vector<Locker>& getLockers() const;

    int assignLocker(const std::string& ownerId, const std::string& code);
    void releaseLocker(int number);
    int countFreeLockers() const;
    Locker& findLockerByOwner(const std::string& ownerId);

private:
    std::vector<Locker> lockers_;
};
