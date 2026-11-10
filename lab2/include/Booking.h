#pragma once

#include "Enums.h"
#include "TimeSlot.h"

class Member;
class Facility;

// Бронирование объекта клуба (корта, дорожки бассейна).
class Booking {
public:
    Booking(int id, Member* member, Facility* facility, const TimeSlot& slot, double cost);

    int getId() const;
    Member* getMember() const;
    Facility* getFacility() const;
    const TimeSlot& getSlot() const;
    BookingStatus getStatus() const;
    double getCost() const;

    void confirm();
    void cancel();
    bool isActive() const;

private:
    int id_;
    Member* member_;
    Facility* facility_;
    TimeSlot slot_;
    BookingStatus status_;
    double cost_;
};
