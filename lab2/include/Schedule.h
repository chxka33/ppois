#pragma once

#include <vector>

#include "TimeSlot.h"

class GroupClass;
class TrainingSession;
class Facility;
class Coach;

// Расписание групповых и персональных занятий.
class Schedule {
public:
    Schedule() = default;

    const std::vector<GroupClass*>& getGroupClasses() const;
    const std::vector<TrainingSession*>& getSessions() const;

    void addGroupClass(GroupClass& groupClass);
    void removeGroupClass(const GroupClass& groupClass);
    void addSession(TrainingSession& session);
    bool hasConflict(const Facility& facility, const TimeSlot& slot) const;
    bool isCoachBusy(const Coach& coach, const TimeSlot& slot) const;
    std::vector<GroupClass*> getClassesOn(const Date& date) const;
    int getCoachLoadMinutes(const Coach& coach, const Date& date) const;

private:
    std::vector<GroupClass*> groupClasses_;
    std::vector<TrainingSession*> sessions_;
};
