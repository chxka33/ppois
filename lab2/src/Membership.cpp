#include "Membership.h"

#include "AccessDeniedException.h"
#include "MembershipExpiredException.h"
#include "MembershipFrozenException.h"

Membership::Membership(const MembershipPlan& plan, const Date& startDate)
    : plan_(plan), startDate_(startDate),
      endDate_(startDate.addDays(plan.getDurationMonths() * 30)),
      frozen_(false), freezeStart_(startDate), frozenDaysUsed_(0), visitsThisWeek_(0) {}

const MembershipPlan& Membership::getPlan() const { return plan_; }
const Date& Membership::getStartDate() const { return startDate_; }
const Date& Membership::getEndDate() const { return endDate_; }
bool Membership::isFrozen() const { return frozen_; }
int Membership::getVisitsThisWeek() const { return visitsThisWeek_; }

bool Membership::isActive(const Date& today) const {
    return !frozen_ && startDate_ <= today && today <= endDate_;
}

bool Membership::canVisit(const Date& today) const {
    if (!isActive(today)) {
        return false;
    }
    if (!plan_.isUnlimited() && visitsThisWeek_ >= plan_.getMaxVisitsPerWeek()) {
        return false;
    }
    return true;
}

void Membership::registerVisit(const Date& today) {
    if (frozen_) {
        throw MembershipFrozenException("Membership is currently frozen");
    }
    if (today < startDate_ || endDate_ < today) {
        throw MembershipExpiredException("Membership is expired or not yet active");
    }
    if (!plan_.isUnlimited() && visitsThisWeek_ >= plan_.getMaxVisitsPerWeek()) {
        throw AccessDeniedException("Weekly visits limit exceeded");
    }
    visitsThisWeek_++;
}

void Membership::resetWeeklyVisits() {
    visitsThisWeek_ = 0;
}

void Membership::freeze(const Date& today) {
    if (frozen_) {
        throw MembershipFrozenException("Membership is already frozen");
    }
    if (today < startDate_ || endDate_ < today) {
        throw MembershipExpiredException("Cannot freeze inactive membership");
    }
    frozen_ = true;
    freezeStart_ = today;
}

void Membership::unfreeze(const Date& today) {
    if (!frozen_) {
        return;
    }
    int days = freezeStart_.daysUntil(today);
    if (days < 0) days = 0;
    frozenDaysUsed_ += days;
    endDate_ = endDate_.addDays(days);
    frozen_ = false;
}

void Membership::extend(int days) {
    if (days > 0) {
        endDate_ = endDate_.addDays(days);
    }
}

int Membership::daysRemaining(const Date& today) const {
    if (endDate_ < today) {
        return 0;
    }
    return today.daysUntil(endDate_);
}
