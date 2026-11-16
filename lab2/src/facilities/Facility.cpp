#include "facilities/Facility.h"

#include <algorithm>

#include "exceptions/AccessDeniedException.h"
#include "exceptions/FacilityFullException.h"
#include "exceptions/InvalidDataException.h"
#include "members/Member.h"

Facility::Facility(std::string id, std::string name, int capacity, AccessLevel requiredAccessLevel)
    : id_(std::move(id)), name_(std::move(name)), capacity_(capacity), occupancy_(0),
      open_(true), requiredAccessLevel_(requiredAccessLevel), lastCleaned_(Date()) {
    if (id_.empty() || name_.empty() || capacity_ <= 0) {
        throw InvalidDataException("Invalid facility parameters");
    }
}

const std::string& Facility::getId() const { return id_; }
const std::string& Facility::getName() const { return name_; }
int Facility::getCapacity() const { return capacity_; }
int Facility::getOccupancy() const { return occupancy_; }
bool Facility::isOpen() const { return open_; }
AccessLevel Facility::getRequiredAccessLevel() const { return requiredAccessLevel_; }
const Date& Facility::getLastCleaned() const { return lastCleaned_; }

void Facility::enter(const Member& member, const Date& today) {
    if (!open_) {
        throw AccessDeniedException("Facility is closed");
    }
    if (!hasFreeSpace()) {
        throw FacilityFullException("Facility is at full capacity");
    }
    if (!member.canAccessFacility(*this, today)) {
        throw AccessDeniedException("Member does not have access to this facility");
    }
    visitorIds_.push_back(member.getMemberId());
    occupancy_ = static_cast<int>(visitorIds_.size());
}

void Facility::enterGuest() {
    if (!open_) {
        throw AccessDeniedException("Facility is closed");
    }
    if (!hasFreeSpace()) {
        throw FacilityFullException("Facility is at full capacity");
    }
    visitorIds_.push_back("GUEST_" + std::to_string(occupancy_ + 1));
    occupancy_ = static_cast<int>(visitorIds_.size());
}

void Facility::leave(const std::string& visitorId) {
    auto it = std::find(visitorIds_.begin(), visitorIds_.end(), visitorId);
    if (it != visitorIds_.end()) {
        visitorIds_.erase(it);
    } else if (!visitorIds_.empty()) {
        visitorIds_.pop_back();
    }
    occupancy_ = static_cast<int>(visitorIds_.size());
}

bool Facility::hasFreeSpace() const {
    return occupancy_ < capacity_;
}

bool Facility::isInside(const std::string& visitorId) const {
    return std::find(visitorIds_.begin(), visitorIds_.end(), visitorId) != visitorIds_.end();
}

double Facility::getLoadPercent() const {
    if (capacity_ <= 0) return 0.0;
    return (static_cast<double>(occupancy_) / capacity_) * 100.0;
}

void Facility::open() {
    open_ = true;
}

void Facility::close() {
    open_ = false;
}

void Facility::markCleaned(const Date& date) {
    lastCleaned_ = date;
}
