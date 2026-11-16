#include "members/LoyaltyProgram.h"

#include "exceptions/InvalidDataException.h"

LoyaltyProgram::LoyaltyProgram(int pointsPerVisit, double pointsPerCurrencyUnit, double pointValue)
    : pointsPerVisit_(pointsPerVisit), pointsPerCurrencyUnit_(pointsPerCurrencyUnit), pointValue_(pointValue) {
    if (pointsPerVisit_ < 0 || pointsPerCurrencyUnit_ < 0.0 || pointValue_ < 0.0) {
        throw InvalidDataException("Loyalty program parameters must be non-negative");
    }
}

int LoyaltyProgram::getPointsPerVisit() const { return pointsPerVisit_; }
double LoyaltyProgram::getPointsPerCurrencyUnit() const { return pointsPerCurrencyUnit_; }
double LoyaltyProgram::getPointValue() const { return pointValue_; }

int LoyaltyProgram::calculateVisitPoints(int visits) const {
    return visits > 0 ? visits * pointsPerVisit_ : 0;
}

int LoyaltyProgram::calculatePurchasePoints(double amount) const {
    return amount > 0.0 ? static_cast<int>(amount * pointsPerCurrencyUnit_) : 0;
}

double LoyaltyProgram::convertPointsToMoney(int points) const {
    return points > 0 ? points * pointValue_ : 0.0;
}

LoyaltyTier LoyaltyProgram::getTier(int totalPoints) const {
    if (totalPoints >= 5000) {
        return LoyaltyTier::Platinum;
    } else if (totalPoints >= 1500) {
        return LoyaltyTier::Gold;
    } else if (totalPoints >= 500) {
        return LoyaltyTier::Silver;
    } else {
        return LoyaltyTier::Bronze;
    }
}

double LoyaltyProgram::getTierDiscountPercent(LoyaltyTier tier) const {
    switch (tier) {
        case LoyaltyTier::Bronze:
            return 0.0;
        case LoyaltyTier::Silver:
            return 5.0;
        case LoyaltyTier::Gold:
            return 10.0;
        case LoyaltyTier::Platinum:
            return 15.0;
    }
    return 0.0;
}
