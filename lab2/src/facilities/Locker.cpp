#include "facilities/Locker.h"

#include "exceptions/AccessDeniedException.h"

Locker::Locker(int number) : number_(number), occupied_(false) {}

int Locker::getNumber() const { return number_; }
bool Locker::isOccupied() const { return occupied_; }
const std::string& Locker::getOwnerId() const { return ownerId_; }

void Locker::occupy(const std::string& ownerId, const std::string& code) {
    if (occupied_) {
        throw AccessDeniedException("Locker is already occupied");
    }
    ownerId_ = ownerId;
    code_ = code;
    occupied_ = true;
}

void Locker::release() {
    occupied_ = false;
    ownerId_.clear();
    code_.clear();
}

bool Locker::unlock(const std::string& code) const {
    return occupied_ && code_ == code;
}
