#pragma once

#include "Date.h"
#include "MembershipPlan.h"

// Купленный клиентом абонемент.
class Membership {
public:
    Membership(const MembershipPlan& plan, const Date& startDate);

    const MembershipPlan& getPlan() const;
    const Date& getStartDate() const;
    const Date& getEndDate() const;
    bool isFrozen() const;
    int getVisitsThisWeek() const;

    bool isActive(const Date& today) const;
    bool canVisit(const Date& today) const;
    void registerVisit(const Date& today);
    void resetWeeklyVisits();
    void freeze(const Date& today);
    void unfreeze(const Date& today);
    void extend(int days);
    int daysRemaining(const Date& today) const;

private:
    MembershipPlan plan_;
    Date startDate_;
    Date endDate_;
    bool frozen_;
    Date freezeStart_;
    int frozenDaysUsed_;
    int visitsThisWeek_;
};
