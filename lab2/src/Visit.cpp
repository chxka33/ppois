#include "Visit.h"

#include "InvalidDataException.h"

Visit::Visit(std::string memberId, Facility* facility, const Date& date, int entryMinute)
    : memberId_(std::move(memberId)), facility_(facility), date_(date),
      entryMinute_(entryMinute), exitMinute_(0), closed_(false) {
    if (memberId_.empty() || entryMinute_ < 0) {
        throw InvalidDataException("Invalid visit parameters");
    }
}

const std::string& Visit::getMemberId() const { return memberId_; }
Facility* Visit::getFacility() const { return facility_; }
const Date& Visit::getDate() const { return date_; }
int Visit::getEntryMinute() const { return entryMinute_; }
int Visit::getExitMinute() const { return exitMinute_; }

void Visit::close(int exitMinute) {
    if (exitMinute < entryMinute_) {
        throw InvalidDataException("Exit minute cannot be earlier than entry minute");
    }
    exitMinute_ = exitMinute;
    closed_ = true;
}

bool Visit::isOpen() const { return !closed_; }

int Visit::getDurationMinutes() const {
    return closed_ ? (exitMinute_ - entryMinute_) : 0;
}
