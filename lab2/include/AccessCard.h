#pragma once

#include <cstddef>
#include <string>

#include "Date.h"

// Пропуск клиента с PIN-кодом.
class AccessCard {
public:
    AccessCard(std::string cardNumber, std::string ownerId, const Date& issueDate, const std::string& pin);

    const std::string& getCardNumber() const;
    const std::string& getOwnerId() const;
    const Date& getIssueDate() const;
    bool isBlocked() const;
    int getFailedAttempts() const;

    bool verifyPin(const std::string& pin);
    void changePin(const std::string& oldPin, const std::string& newPin);
    void block();
    void unblock();

private:
    static std::size_t hashPin(const std::string& pin);
    static bool isValidPin(const std::string& pin);

    std::string cardNumber_;
    std::string ownerId_;
    Date issueDate_;
    bool blocked_;
    std::size_t pinHash_;
    int failedAttempts_;
};
