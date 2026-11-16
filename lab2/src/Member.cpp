#include "Member.h"

#include "Facility.h"
#include "Invoice.h"
#include "LoyaltyProgram.h"
#include "Payment.h"
#include "InsufficientFundsException.h"
#include "InvalidDataException.h"

Member::Member(std::string memberId, std::string firstName, std::string lastName,
               const Date& birthDate, std::string phone, std::string email)
    : Person(std::move(firstName), std::move(lastName), birthDate, std::move(phone), std::move(email)),
      memberId_(std::move(memberId)), balance_(0.0), bonusPoints_(0) {
    if (memberId_.empty()) {
        throw InvalidDataException("Member ID cannot be empty");
    }
}

const std::string& Member::getMemberId() const { return memberId_; }
double Member::getBalance() const { return balance_; }
int Member::getBonusPoints() const { return bonusPoints_; }
Membership* Member::getMembership() const { return membership_.get(); }
AccessCard* Member::getAccessCard() const { return accessCard_.get(); }
HealthRecord& Member::getHealthRecord() { return healthRecord_; }
ProgressTracker& Member::getProgressTracker() { return progressTracker_; }
const std::vector<Visit>& Member::getVisits() const { return visits_; }

void Member::deposit(double amount) {
    if (amount <= 0.0) {
        throw InvalidDataException("Deposit amount must be positive");
    }
    balance_ += amount;
}

void Member::withdraw(double amount) {
    if (amount <= 0.0) {
        throw InvalidDataException("Withdrawal amount must be positive");
    }
    if (balance_ < amount) {
        throw InsufficientFundsException(amount, balance_);
    }
    balance_ -= amount;
}

void Member::payInvoice(Invoice& invoice, const Date& today) {
    double total = invoice.calculateTotal();
    withdraw(total);
    Payment payment(invoice.getNumber() + "_pay", memberId_, total, today, PaymentMethod::Balance);
    payment.process();
    invoice.markPaid(payment);
}

void Member::purchaseMembership(const MembershipPlan& plan, const Date& startDate) {
    double total = plan.calculateTotalPrice();
    withdraw(total);
    membership_ = std::make_unique<Membership>(plan, startDate);
}

void Member::issueAccessCard(std::unique_ptr<AccessCard> card) {
    accessCard_ = std::move(card);
}

bool Member::hasActiveMembership(const Date& today) const {
    return membership_ && membership_->isActive(today);
}

bool Member::canAccessFacility(const Facility& facility, const Date& today) const {
    if (!hasActiveMembership(today)) {
        return false;
    }
    if (!membership_->canVisit(today)) {
        return false;
    }
    if (!membership_->getPlan().allowsAccessLevel(facility.getRequiredAccessLevel())) {
        return false;
    }
    if (!membership_->getPlan().includesFacility(facility.getType())) {
        return false;
    }
    return true;
}

void Member::registerVisit(const Visit& visit, const LoyaltyProgram& loyaltyProgram) {
    visits_.push_back(visit);
    if (membership_) {
        membership_->registerVisit(visit.getDate());
    }
    addBonusPoints(loyaltyProgram.getPointsPerVisit());
}

void Member::addBonusPoints(int points) {
    if (points > 0) {
        bonusPoints_ += points;
    }
}

double Member::redeemBonusPoints(int points, const LoyaltyProgram& loyaltyProgram) {
    if (points <= 0 || bonusPoints_ < points) {
        throw InvalidDataException("Insufficient bonus points");
    }
    bonusPoints_ -= points;
    double money = loyaltyProgram.convertPointsToMoney(points);
    balance_ += money;
    return money;
}

int Member::countVisitsBetween(const Date& from, const Date& to) const {
    int count = 0;
    for (const auto& v : visits_) {
        if (from <= v.getDate() && v.getDate() <= to) {
            count++;
        }
    }
    return count;
}
