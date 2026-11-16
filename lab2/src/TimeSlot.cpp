#include "TimeSlot.h"

#include <iomanip>
#include <sstream>

#include "InvalidDataException.h"

TimeSlot::TimeSlot(const Date& date, int startMinute, int durationMinutes)
    : date_(date), startMinute_(startMinute), durationMinutes_(durationMinutes) {
    if (startMinute < 0 || durationMinutes <= 0 || startMinute + durationMinutes > 1440) {
        throw InvalidDataException("Invalid time slot bounds");
    }
}

const Date& TimeSlot::getDate() const { return date_; }
int TimeSlot::getStartMinute() const { return startMinute_; }
int TimeSlot::getDurationMinutes() const { return durationMinutes_; }
int TimeSlot::getEndMinute() const { return startMinute_ + durationMinutes_; }

bool TimeSlot::overlaps(const TimeSlot& other) const {
    if (date_ != other.date_) return false;
    return startMinute_ < other.getEndMinute() && other.startMinute_ < getEndMinute();
}

bool TimeSlot::contains(int minute) const {
    return minute >= startMinute_ && minute < getEndMinute();
}

double TimeSlot::getDurationHours() const {
    return static_cast<double>(durationMinutes_) / 60;
}

std::string TimeSlot::toString() const {
    std::ostringstream oss;
    int sh = startMinute_ / 60;
    int sm = startMinute_ % 60;
    int eh = getEndMinute() / 60;
    int em = getEndMinute() % 60;
    oss << date_.toString() << " "
        << std::setfill('0') << std::setw(2) << sh << ":" << std::setw(2) << sm << "-"
        << std::setw(2) << eh << ":" << std::setw(2) << em;
    return oss.str();
}
