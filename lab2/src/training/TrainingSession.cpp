#include "training/TrainingSession.h"

#include "staff/Coach.h"
#include "facilities/Facility.h"
#include "exceptions/InvalidDataException.h"
#include "members/Member.h"
#include "training/TrainingProgram.h"

TrainingSession::TrainingSession(Coach* coach, Member* member, Facility* facility,
                                 const TimeSlot& slot, TrainingProgram* program, double hourlyPrice)
    : coach_(coach), member_(member), facility_(facility), slot_(slot),
      program_(program), status_(SessionStatus::Planned), hourlyPrice_(hourlyPrice) {
    if (hourlyPrice_ < 0.0) {
        throw InvalidDataException("Hourly price cannot be negative");
    }
}

Coach* TrainingSession::getCoach() const { return coach_; }
Member* TrainingSession::getMember() const { return member_; }
Facility* TrainingSession::getFacility() const { return facility_; }
const TimeSlot& TrainingSession::getSlot() const { return slot_; }
TrainingProgram* TrainingSession::getProgram() const { return program_; }
SessionStatus TrainingSession::getStatus() const { return status_; }

double TrainingSession::calculateCost() const {
    return hourlyPrice_ * slot_.getDurationHours();
}

void TrainingSession::start() {
    if (status_ == SessionStatus::Planned) {
        status_ = SessionStatus::InProgress;
    }
}

void TrainingSession::complete() {
    if (status_ == SessionStatus::InProgress || status_ == SessionStatus::Planned) {
        status_ = SessionStatus::Completed;
    }
}

void TrainingSession::cancel() {
    if (status_ != SessionStatus::Completed) {
        status_ = SessionStatus::Cancelled;
    }
}
