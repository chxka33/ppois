#pragma once

#include <memory>
#include <string>
#include <vector>

#include "AccessCard.h"
#include "HealthRecord.h"
#include "Membership.h"
#include "Person.h"
#include "ProgressTracker.h"
#include "Visit.h"

class Facility;
class Invoice;
class LoyaltyProgram;

// Клиент спортивного клуба.
class Member : public Person {
public:
    Member(std::string memberId, std::string firstName, std::string lastName, const Date& birthDate, std::string phone,
           std::string email);

    const std::string& getMemberId() const;
    double getBalance() const;
    int getBonusPoints() const;
    Membership* getMembership() const;
    AccessCard* getAccessCard() const;
    HealthRecord& getHealthRecord();
    ProgressTracker& getProgressTracker();
    const std::vector<Visit>& getVisits() const;

    void deposit(double amount);
    void withdraw(double amount);
    void payInvoice(Invoice& invoice, const Date& today);
    void purchaseMembership(const MembershipPlan& plan, const Date& startDate);
    void issueAccessCard(std::unique_ptr<AccessCard> card);
    bool hasActiveMembership(const Date& today) const;
    bool canAccessFacility(const Facility& facility, const Date& today) const;
    void registerVisit(const Visit& visit, const LoyaltyProgram& loyaltyProgram);
    void addBonusPoints(int points);
    double redeemBonusPoints(int points, const LoyaltyProgram& loyaltyProgram);
    int countVisitsBetween(const Date& from, const Date& to) const;

private:
    std::string memberId_;
    double balance_;
    int bonusPoints_;
    std::unique_ptr<Membership> membership_;
    std::unique_ptr<AccessCard> accessCard_;
    HealthRecord healthRecord_;
    ProgressTracker progressTracker_;
    std::vector<Visit> visits_;
};
