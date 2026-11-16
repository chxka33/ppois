#include "members/MembershipPlan.h"

#include "common/Date.h"
#include "finance/Discount.h"
#include "exceptions/InvalidDataException.h"

MembershipPlan::MembershipPlan(std::string name, double monthlyPrice, int durationMonths,
                               AccessLevel accessLevel, int maxVisitsPerWeek,
                               bool includesPool, bool includesSauna)
    : name_(std::move(name)), monthlyPrice_(monthlyPrice), durationMonths_(durationMonths),
      accessLevel_(accessLevel), maxVisitsPerWeek_(maxVisitsPerWeek),
      includesPool_(includesPool), includesSauna_(includesSauna) {
    if (name_.empty() || monthlyPrice_ < 0.0 || durationMonths_ <= 0 || maxVisitsPerWeek_ < 0) {
        throw InvalidDataException("Invalid membership plan data");
    }
}

const std::string& MembershipPlan::getName() const { return name_; }
double MembershipPlan::getMonthlyPrice() const { return monthlyPrice_; }
int MembershipPlan::getDurationMonths() const { return durationMonths_; }
AccessLevel MembershipPlan::getAccessLevel() const { return accessLevel_; }
int MembershipPlan::getMaxVisitsPerWeek() const { return maxVisitsPerWeek_; }

double MembershipPlan::calculateTotalPrice() const {
    return monthlyPrice_ * durationMonths_;
}

double MembershipPlan::calculateTotalPrice(Discount& discount, const Date& today) const {
    double base = calculateTotalPrice();
    return discount.apply(base, today);
}

bool MembershipPlan::allowsAccessLevel(AccessLevel required) const {
    return static_cast<int>(accessLevel_) >= static_cast<int>(required);
}

bool MembershipPlan::includesFacility(FacilityType type) const {
    switch (type) {
        case FacilityType::Gym:
        case FacilityType::LockerRoom:
            return true;
        case FacilityType::SwimmingPool:
            return includesPool_;
        case FacilityType::Sauna:
            return includesSauna_;
        case FacilityType::TennisCourt:
            return accessLevel_ >= AccessLevel::Premium;
    }
    return false;
}

bool MembershipPlan::isUnlimited() const {
    return maxVisitsPerWeek_ == 0;
}
