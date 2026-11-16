#pragma once

#include <string>

// Календарная дата (день, месяц, год).
class Date {
public:
    Date();
    Date(int day, int month, int year);

    int getDay() const;
    int getMonth() const;
    int getYear() const;

    static bool isLeapYear(int year);
    static int daysInMonth(int month, int year);
    static bool isValid(int day, int month, int year);

    Date addDays(int days) const;
    int toDayNumber() const;
    int daysUntil(const Date& other) const;
    int yearsUntil(const Date& other) const;
    std::string toString() const;

    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;
    bool operator<(const Date& other) const;
    bool operator<=(const Date& other) const;

private:
    int day_;
    int month_;
    int year_;
};
