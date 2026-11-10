#pragma once

#include <string>
#include <vector>

#include "TimeSlot.h"
#include "Workout.h"

class Coach;
class Member;
class Facility;

// Групповое занятие по расписанию.
class GroupClass {
public:
    GroupClass(std::string name, const Workout& workout, Coach* coach, Facility* facility, const TimeSlot& slot,
               int maxParticipants);

    const std::string& getName() const;
    const Workout& getWorkout() const;
    Coach* getCoach() const;
    Facility* getFacility() const;
    const TimeSlot& getSlot() const;
    int getMaxParticipants() const;
    const std::vector<Member*>& getParticipants() const;
    bool isCancelled() const;

    void enroll(Member& member, const Date& today);
    void unenroll(const std::string& memberId);
    bool isEnrolled(const std::string& memberId) const;
    bool isFull() const;
    int getFreeSpots() const;
    void cancel();

private:
    std::string name_;
    Workout workout_;
    Coach* coach_;
    Facility* facility_;
    TimeSlot slot_;
    int maxParticipants_;
    std::vector<Member*> participants_;
    bool cancelled_;
};
