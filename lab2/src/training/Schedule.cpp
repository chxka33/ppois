#include "training/Schedule.h"

#include <algorithm>

#include "exceptions/BookingConflictException.h"
#include "staff/Coach.h"
#include "exceptions/CoachUnavailableException.h"
#include "facilities/Facility.h"
#include "training/GroupClass.h"
#include "training/TrainingSession.h"

const std::vector<GroupClass*>& Schedule::getGroupClasses() const {
    return groupClasses_;
}

const std::vector<TrainingSession*>& Schedule::getSessions() const {
    return sessions_;
}

void Schedule::addGroupClass(GroupClass& groupClass) {
    if (groupClass.getFacility() && hasConflict(*groupClass.getFacility(), groupClass.getSlot())) {
        throw BookingConflictException("Facility is already booked for this slot");
    }
    if (groupClass.getCoach() && isCoachBusy(*groupClass.getCoach(), groupClass.getSlot())) {
        throw CoachUnavailableException("Coach is already busy for this slot");
    }
    groupClasses_.push_back(&groupClass);
}

void Schedule::removeGroupClass(const GroupClass& groupClass) {
    groupClasses_.erase(
        std::remove(groupClasses_.begin(), groupClasses_.end(), &groupClass),
        groupClasses_.end());
}

void Schedule::addSession(TrainingSession& session) {
    if (session.getFacility() && hasConflict(*session.getFacility(), session.getSlot())) {
        throw BookingConflictException("Facility is already booked for this slot");
    }
    if (session.getCoach() && isCoachBusy(*session.getCoach(), session.getSlot())) {
        throw CoachUnavailableException("Coach is already busy for this slot");
    }
    sessions_.push_back(&session);
}

bool Schedule::hasConflict(const Facility& facility, const TimeSlot& slot) const {
    for (const auto* gc : groupClasses_) {
        if (gc && !gc->isCancelled() && gc->getFacility() &&
            gc->getFacility()->getId() == facility.getId() && gc->getSlot().overlaps(slot)) {
            return true;
        }
    }
    for (const auto* s : sessions_) {
        if (s && s->getStatus() != SessionStatus::Cancelled && s->getFacility() &&
            s->getFacility()->getId() == facility.getId() && s->getSlot().overlaps(slot)) {
            return true;
        }
    }
    return false;
}

bool Schedule::isCoachBusy(const Coach& coach, const TimeSlot& slot) const {
    for (const auto* gc : groupClasses_) {
        if (gc && !gc->isCancelled() && gc->getCoach() &&
            gc->getCoach()->getEmployeeId() == coach.getEmployeeId() &&
            gc->getSlot().overlaps(slot)) {
            return true;
        }
    }
    for (const auto* s : sessions_) {
        if (s && s->getStatus() != SessionStatus::Cancelled && s->getCoach() &&
            s->getCoach()->getEmployeeId() == coach.getEmployeeId() &&
            s->getSlot().overlaps(slot)) {
            return true;
        }
    }
    return false;
}

std::vector<GroupClass*> Schedule::getClassesOn(const Date& date) const {
    std::vector<GroupClass*> result;
    for (auto* gc : groupClasses_) {
        if (gc && !gc->isCancelled() && gc->getSlot().getDate() == date) {
            result.push_back(gc);
        }
    }
    return result;
}

int Schedule::getCoachLoadMinutes(const Coach& coach, const Date& date) const {
    int total = 0;
    for (const auto* gc : groupClasses_) {
        if (gc && !gc->isCancelled() && gc->getCoach() &&
            gc->getCoach()->getEmployeeId() == coach.getEmployeeId() &&
            gc->getSlot().getDate() == date) {
            total += gc->getSlot().getDurationMinutes();
        }
    }
    for (const auto* s : sessions_) {
        if (s && s->getStatus() != SessionStatus::Cancelled && s->getCoach() &&
            s->getCoach()->getEmployeeId() == coach.getEmployeeId() &&
            s->getSlot().getDate() == date) {
            total += s->getSlot().getDurationMinutes();
        }
    }
    return total;
}
