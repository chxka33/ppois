#pragma once

#include <string>

#include "Date.h"

// Скидочный промокод.
class Discount {
public:
    Discount(std::string code, double percent, const Date& validUntil, int usageLimit);

    const std::string& getCode() const;
    double getPercent() const;
    const Date& getValidUntil() const;
    int getUsageLimit() const;
    int getUsedCount() const;

    bool isValid(const Date& today) const;
    double apply(double price, const Date& today);
    double previewPrice(double price) const;
    int getRemainingUses() const;

private:
    std::string code_;
    double percent_;
    Date validUntil_;
    int usageLimit_;
    int usedCount_;
};
