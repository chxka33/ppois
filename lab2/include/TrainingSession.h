#pragma once

#include "Enums.h"
#include "TimeSlot.h"

class Coach;
class Member;
class Facility;
class TrainingProgram;

// Персональная тренировка клиента с тренером.
class TrainingSession {
public:
    TrainingSession(Coach* coach, Member* member, Facility* facility, const TimeSlot& slot,
                    TrainingProgram* program, double hourlyPrice);

    Coach* getCoach() const;
    Member* getMember() const;
    Facility* getFacility() const;
    const TimeSlot& getSlot() const;
    TrainingProgram* getProgram() const;
    SessionStatus getStatus() const;

    double calculateCost() const;
    void start();
    void complete();
    void cancel();

private:
    Coach* coach_;
    Member* member_;
    Facility* facility_;
    TimeSlot slot_;
    TrainingProgram* program_;
    SessionStatus status_;
    double hourlyPrice_;
};
