#include "finance/Discount.h"

#include "exceptions/InvalidDataException.h"

Discount::Discount(std::string code, double percent, const Date& validUntil, int usageLimit)
    : code_(std::move(code)), percent_(percent), validUntil_(validUntil), usageLimit_(usageLimit), usedCount_(0) {
    if (code_.empty() || percent_ <= 0.0 || percent_ > 100.0 || usageLimit_ <= 0) {
        throw InvalidDataException("Invalid discount parameters");
    }
}

const std::string& Discount::getCode() const { return code_; }
double Discount::getPercent() const { return percent_; }
const Date& Discount::getValidUntil() const { return validUntil_; }
int Discount::getUsageLimit() const { return usageLimit_; }
int Discount::getUsedCount() const { return usedCount_; }

bool Discount::isValid(const Date& today) const {
    return today <= validUntil_ && usedCount_ < usageLimit_;
}

double Discount::apply(double price, const Date& today) {
    if (!isValid(today)) {
        throw InvalidDataException("Discount is expired or usage limit reached");
    }
    usedCount_++;
    return previewPrice(price);
}

double Discount::previewPrice(double price) const {
    if (price < 0.0) return 0.0;
    return price * (1.0 - percent_ / 100.0);
}

int Discount::getRemainingUses() const {
    return usageLimit_ - usedCount_;
}
