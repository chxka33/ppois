#include "common/Date.h"

#include <iomanip>
#include <sstream>

#include "exceptions/InvalidDateException.h"

Date::Date() : day_(1), month_(1), year_(2000) {}

Date::Date(int day, int month, int year) : day_(day), month_(month), year_(year) {
    if (!isValid(day, month, year)) {
        throw InvalidDateException(std::to_string(day) + "." + std::to_string(month) + "." + std::to_string(year));
    }
}

int Date::getDay() const { return day_; }
int Date::getMonth() const { return month_; }
int Date::getYear() const { return year_; }

bool Date::isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int Date::daysInMonth(int month, int year) {
    if (month < 1 || month > 12) {
        return 0;
    }
    switch (month) {
        case 2:
            return isLeapYear(year) ? 29 : 28;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        default:
            return 31;
    }
}

bool Date::isValid(int day, int month, int year) {
    if (year < 1900 || year > 2100) {
        return false;
    }
    if (month < 1 || month > 12) {
        return false;
    }
    return day >= 1 && day <= daysInMonth(month, year);
}

int Date::toDayNumber() const {
    int y = year_;
    int m = month_;
    int d = day_;
    y -= (m <= 2 ? 1 : 0);
    const int era = (y >= 0 ? y : y - 399) / 400;
    const unsigned yoe = static_cast<unsigned>(y - era * 400);
    const unsigned doy = (153 * (m > 2 ? m - 3 : m + 9) + 2) / 5 + d - 1;
    const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + static_cast<int>(doe) - 719468;
}

static Date fromDayNumber(int n) {
    n += 719468;
    const int era = (n >= 0 ? n : n - 146096) / 146097;
    const unsigned doe = static_cast<unsigned>(n - era * 146097);
    const unsigned yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    const int y = static_cast<int>(yoe) + era * 400;
    const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    const unsigned mp = (5 * doy + 2) / 153;
    const unsigned d = doy - (153 * mp + 2) / 5 + 1;
    const unsigned m = mp < 10 ? mp + 3 : mp - 9;
    return Date(static_cast<int>(d), static_cast<int>(m), y + (m <= 2 ? 1 : 0));
}

Date Date::addDays(int days) const {
    return fromDayNumber(toDayNumber() + days);
}

int Date::daysUntil(const Date& other) const {
    return other.toDayNumber() - toDayNumber();
}

int Date::yearsUntil(const Date& other) const {
    int years = other.year_ - year_;
    if (other.month_ < month_ || (other.month_ == month_ && other.day_ < day_)) {
        --years;
    }
    return years;
}

std::string Date::toString() const {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(4) << year_ << "-"
        << std::setw(2) << month_ << "-"
        << std::setw(2) << day_;
    return oss.str();
}

bool Date::operator==(const Date& other) const {
    return day_ == other.day_ && month_ == other.month_ && year_ == other.year_;
}

bool Date::operator!=(const Date& other) const {
    return !(*this == other);
}

bool Date::operator<(const Date& other) const {
    if (year_ != other.year_) return year_ < other.year_;
    if (month_ != other.month_) return month_ < other.month_;
    return day_ < other.day_;
}

bool Date::operator<=(const Date& other) const {
    return *this < other || *this == other;
}
