#pragma once

#include <string>

#include "Date.h"

class Facility;

// Посещение клиентом объекта клуба.
class Visit {
public:
    Visit(std::string memberId, Facility* facility, const Date& date, int entryMinute);

    const std::string& getMemberId() const;
    Facility* getFacility() const;
    const Date& getDate() const;
    int getEntryMinute() const;
    int getExitMinute() const;

    void close(int exitMinute);
    bool isOpen() const;
    int getDurationMinutes() const;

private:
    std::string memberId_;
    Facility* facility_;
    Date date_;
    int entryMinute_;
    int exitMinute_;
    bool closed_;
};
