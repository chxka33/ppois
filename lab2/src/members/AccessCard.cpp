#include "members/AccessCard.h"

#include <algorithm>
#include <cctype>
#include <functional>

#include "exceptions/AccessDeniedException.h"
#include "exceptions/InvalidDataException.h"
#include "exceptions/InvalidPinException.h"

AccessCard::AccessCard(std::string cardNumber, std::string ownerId, const Date& issueDate, const std::string& pin)
    : cardNumber_(std::move(cardNumber)), ownerId_(std::move(ownerId)), issueDate_(issueDate),
      blocked_(false), pinHash_(0), failedAttempts_(0) {
    if (cardNumber_.empty() || ownerId_.empty()) {
        throw InvalidDataException("Card number and owner ID cannot be empty");
    }
    if (!isValidPin(pin)) {
        throw InvalidPinException("PIN must be at least 4 digits");
    }
    pinHash_ = hashPin(pin);
}

const std::string& AccessCard::getCardNumber() const { return cardNumber_; }
const std::string& AccessCard::getOwnerId() const { return ownerId_; }
const Date& AccessCard::getIssueDate() const { return issueDate_; }
bool AccessCard::isBlocked() const { return blocked_; }
int AccessCard::getFailedAttempts() const { return failedAttempts_; }

bool AccessCard::verifyPin(const std::string& pin) {
    if (blocked_) {
        return false;
    }
    if (hashPin(pin) == pinHash_) {
        failedAttempts_ = 0;
        return true;
    }
    failedAttempts_++;
    if (failedAttempts_ >= 3) {
        blocked_ = true;
    }
    return false;
}

void AccessCard::changePin(const std::string& oldPin, const std::string& newPin) {
    if (blocked_) {
        throw AccessDeniedException("Card is blocked");
    }
    if (hashPin(oldPin) != pinHash_) {
        failedAttempts_++;
        if (failedAttempts_ >= 3) {
            blocked_ = true;
        }
        throw InvalidPinException("Incorrect old PIN");
    }
    if (!isValidPin(newPin)) {
        throw InvalidPinException("Invalid new PIN");
    }
    pinHash_ = hashPin(newPin);
    failedAttempts_ = 0;
}

void AccessCard::block() {
    blocked_ = true;
}

void AccessCard::unblock() {
    blocked_ = false;
    failedAttempts_ = 0;
}

std::size_t AccessCard::hashPin(const std::string& pin) {
    return std::hash<std::string>{}(pin);
}

bool AccessCard::isValidPin(const std::string& pin) {
    if (pin.length() < static_cast<size_t>(4)) {
        return false;
    }
    return std::all_of(pin.begin(), pin.end(), [](unsigned char c) {
        return std::isdigit(c);
    });
}
