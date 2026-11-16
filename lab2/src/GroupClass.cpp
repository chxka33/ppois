#include "GroupClass.h"

#include <algorithm>

#include "AccessDeniedException.h"
#include "ClassFullException.h"
#include "Coach.h"
#include "Facility.h"
#include "InvalidDataException.h"
#include "MedicalClearanceException.h"
#include "Member.h"

GroupClass::GroupClass(std::string name, const Workout& workout, Coach* coach,
                       Facility* facility, const TimeSlot& slot, int maxParticipants)
    : name_(std::move(name)), workout_(workout), coach_(coach), facility_(facility),
      slot_(slot), maxParticipants_(maxParticipants), cancelled_(false) {
    if (name_.empty() || maxParticipants_ <= 0) {
        throw InvalidDataException("Invalid group class parameters");
    }
}

const std::string& GroupClass::getName() const { return name_; }
const Workout& GroupClass::getWorkout() const { return workout_; }
Coach* GroupClass::getCoach() const { return coach_; }
Facility* GroupClass::getFacility() const { return facility_; }
const TimeSlot& GroupClass::getSlot() const { return slot_; }
int GroupClass::getMaxParticipants() const { return maxParticipants_; }
const std::vector<Member*>& GroupClass::getParticipants() const { return participants_; }
bool GroupClass::isCancelled() const { return cancelled_; }

void GroupClass::enroll(Member& member, const Date& today) {
    if (cancelled_) {
        throw AccessDeniedException("Group class is cancelled");
    }
    if (isFull()) {
        throw ClassFullException("Class is full: " + name_);
    }
    if (isEnrolled(member.getMemberId())) {
        return;
    }
    if (!member.hasActiveMembership(today)) {
        throw AccessDeniedException("Member has no active membership");
    }
    if (!member.getHealthRecord().canPerform(workout_.getIntensity(), today)) {
        throw MedicalClearanceException("Member lacks clearance for " + name_);
    }
    participants_.push_back(&member);
}

void GroupClass::unenroll(const std::string& memberId) {
    participants_.erase(
        std::remove_if(participants_.begin(), participants_.end(),
                       [&memberId](Member* m) { return m && m->getMemberId() == memberId; }),
        participants_.end());
}

bool GroupClass::isEnrolled(const std::string& memberId) const {
    for (const auto* m : participants_) {
        if (m && m->getMemberId() == memberId) {
            return true;
        }
    }
    return false;
}

bool GroupClass::isFull() const {
    return static_cast<int>(participants_.size()) >= maxParticipants_;
}

int GroupClass::getFreeSpots() const {
    return maxParticipants_ - static_cast<int>(participants_.size());
}

void GroupClass::cancel() {
    cancelled_ = true;
}
