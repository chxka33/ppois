#pragma once

#include <string>

#include "Date.h"

// Временной интервал в пределах одного дня.
class TimeSlot {
public:
    TimeSlot(const Date& date, int startMinute, int durationMinutes);

    const Date& getDate() const;
    int getStartMinute() const;
    int getDurationMinutes() const;
    int getEndMinute() const;

    bool overlaps(const TimeSlot& other) const;
    bool contains(int minute) const;
    double getDurationHours() const;
    std::string toString() const;

private:
    Date date_;
    int startMinute_;
    int durationMinutes_;
};
