#include "Booking.h"

#include "Facility.h"
#include "InvalidDataException.h"
#include "Member.h"

Booking::Booking(int id, Member* member, Facility* facility, const TimeSlot& slot, double cost)
    : id_(id), member_(member), facility_(facility), slot_(slot),
      status_(BookingStatus::Pending), cost_(cost) {
    if (cost_ < 0.0) {
        throw InvalidDataException("Booking cost cannot be negative");
    }
}

int Booking::getId() const { return id_; }
Member* Booking::getMember() const { return member_; }
Facility* Booking::getFacility() const { return facility_; }
const TimeSlot& Booking::getSlot() const { return slot_; }
BookingStatus Booking::getStatus() const { return status_; }
double Booking::getCost() const { return cost_; }

void Booking::confirm() {
    status_ = BookingStatus::Confirmed;
}

void Booking::cancel() {
    status_ = BookingStatus::Cancelled;
}

bool Booking::isActive() const {
    return status_ == BookingStatus::Confirmed || status_ == BookingStatus::Pending;
}
