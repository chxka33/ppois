#pragma once

#include <string>

#include "Enums.h"

class Discount;
class Date;

// Тарифный план абонемента.
class MembershipPlan {
public:
    MembershipPlan(std::string name, double monthlyPrice, int durationMonths, AccessLevel accessLevel,
                   int maxVisitsPerWeek, bool includesPool, bool includesSauna);

    const std::string& getName() const;
    double getMonthlyPrice() const;
    int getDurationMonths() const;
    AccessLevel getAccessLevel() const;
    int getMaxVisitsPerWeek() const;

    double calculateTotalPrice() const;
    double calculateTotalPrice(Discount& discount, const Date& today) const;
    bool allowsAccessLevel(AccessLevel required) const;
    bool includesFacility(FacilityType type) const;
    bool isUnlimited() const;

private:
    std::string name_;
    double monthlyPrice_;
    int durationMonths_;
    AccessLevel accessLevel_;
    int maxVisitsPerWeek_;
    bool includesPool_;
    bool includesSauna_;
};
