#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Booking.h"

class TennisCourt;

class BookingSystem {
public:
    BookingSystem();

    const std::vector<std::unique_ptr<Booking>>& getBookings() const;

    Booking& createBooking(Member& member, Facility& facility, const TimeSlot& slot, double cost);
    Booking& bookCourt(Member& member, TennisCourt& court, const TimeSlot& slot);
    void cancelBooking(int bookingId);
    bool isSlotFree(const Facility& facility, const TimeSlot& slot) const;
    Booking& findBooking(int bookingId);
    std::vector<Booking*> getBookingsForMember(const std::string& memberId) const;

private:
    std::vector<std::unique_ptr<Booking>> bookings_;
    int nextId_;
};
