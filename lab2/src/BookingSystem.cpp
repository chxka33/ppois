#include "BookingSystem.h"

#include "BookingConflictException.h"
#include "Member.h"
#include "SportsClubException.h"
#include "TennisCourt.h"

BookingSystem::BookingSystem() : nextId_(1) {}

const std::vector<std::unique_ptr<Booking>>& BookingSystem::getBookings() const {
    return bookings_;
}

Booking& BookingSystem::createBooking(Member& member, Facility& facility, const TimeSlot& slot, double cost) {
    if (!isSlotFree(facility, slot)) {
        throw BookingConflictException("Slot is not free for facility " + facility.getId());
    }
    int id = nextId_++;
    auto booking = std::make_unique<Booking>(id, &member, &facility, slot, cost);
    booking->confirm();
    bookings_.push_back(std::move(booking));
    return *bookings_.back();
}

Booking& BookingSystem::bookCourt(Member& member, TennisCourt& court, const TimeSlot& slot) {
    double cost = court.calculateRentCost(slot.getDurationHours(), true);
    return createBooking(member, court, slot, cost);
}

void BookingSystem::cancelBooking(int bookingId) {
    Booking& b = findBooking(bookingId);
    b.cancel();
}

bool BookingSystem::isSlotFree(const Facility& facility, const TimeSlot& slot) const {
    for (const auto& b : bookings_) {
        if (b && b->isActive() && b->getFacility() &&
            b->getFacility()->getId() == facility.getId() &&
            b->getSlot().overlaps(slot)) {
            return false;
        }
    }
    return true;
}

Booking& BookingSystem::findBooking(int bookingId) {
    for (auto& b : bookings_) {
        if (b && b->getId() == bookingId) {
            return *b;
        }
    }
    throw SportsClubException("Booking not found: " + std::to_string(bookingId));
}

std::vector<Booking*> BookingSystem::getBookingsForMember(const std::string& memberId) const {
    std::vector<Booking*> result;
    for (const auto& b : bookings_) {
        if (b && b->getMember() && b->getMember()->getMemberId() == memberId) {
            result.push_back(b.get());
        }
    }
    return result;
}
