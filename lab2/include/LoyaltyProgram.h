#pragma once

#include "Enums.h"

// Правила начисления и списания бонусных баллов.
class LoyaltyProgram {
public:
    LoyaltyProgram(int pointsPerVisit, double pointsPerCurrencyUnit, double pointValue);

    int getPointsPerVisit() const;
    double getPointsPerCurrencyUnit() const;
    double getPointValue() const;

    int calculateVisitPoints(int visits) const;
    int calculatePurchasePoints(double amount) const;
    double convertPointsToMoney(int points) const;
    LoyaltyTier getTier(int totalPoints) const;
    double getTierDiscountPercent(LoyaltyTier tier) const;

private:
    int pointsPerVisit_;
    double pointsPerCurrencyUnit_;
    double pointValue_;
};
