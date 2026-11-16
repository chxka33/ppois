#include "LockerRoom.h"

#include "FacilityFullException.h"
#include "InvalidDataException.h"
#include "SportsClubException.h"

LockerRoom::LockerRoom(std::string id, std::string name, int capacity,
                       AccessLevel requiredAccessLevel, int lockerCount)
    : Facility(std::move(id), std::move(name), capacity, requiredAccessLevel) {
    if (lockerCount <= 0) {
        throw InvalidDataException("Locker count must be positive");
    }
    for (int i = 1; i <= lockerCount; ++i) {
        lockers_.emplace_back(i);
    }
}

FacilityType LockerRoom::getType() const {
    return FacilityType::LockerRoom;
}

const std::vector<Locker>& LockerRoom::getLockers() const {
    return lockers_;
}

int LockerRoom::assignLocker(const std::string& ownerId, const std::string& code) {
    for (auto& locker : lockers_) {
        if (!locker.isOccupied()) {
            locker.occupy(ownerId, code);
            return locker.getNumber();
        }
    }
    throw FacilityFullException("No free lockers available");
}

void LockerRoom::releaseLocker(int number) {
    for (auto& locker : lockers_) {
        if (locker.getNumber() == number) {
            locker.release();
            return;
        }
    }
}

int LockerRoom::countFreeLockers() const {
    int count = 0;
    for (const auto& locker : lockers_) {
        if (!locker.isOccupied()) {
            count++;
        }
    }
    return count;
}

Locker& LockerRoom::findLockerByOwner(const std::string& ownerId) {
    for (auto& locker : lockers_) {
        if (locker.isOccupied() && locker.getOwnerId() == ownerId) {
            return locker;
        }
    }
    throw SportsClubException("No locker found for owner: " + ownerId);
}
