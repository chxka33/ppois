#include "staff/Administrator.h"

#include "members/AccessCard.h"
#include "members/Member.h"
#include "club/SportsClub.h"
#include "exceptions/SportsClubException.h"

Administrator::Administrator(std::string employeeId, std::string firstName, std::string lastName,
                             const Date& birthDate, std::string phone, std::string email,
                             double baseSalary, const Date& hireDate, SportsClub* club)
    : Employee(std::move(employeeId), std::move(firstName), std::move(lastName), birthDate,
               std::move(phone), std::move(email), baseSalary, hireDate),
      club_(club), registeredMembersCount_(0) {}

std::string Administrator::getPosition() const {
    return "Administrator";
}

SportsClub* Administrator::getClub() const {
    return club_;
}

int Administrator::getRegisteredMembersCount() const {
    return registeredMembersCount_;
}

Member& Administrator::registerMember(std::unique_ptr<Member> member) {
    if (!club_) {
        throw SportsClubException("SportsClub instance is null");
    }
    registeredMembersCount_++;
    return club_->registerMember(std::move(member));
}

void Administrator::blockAccessCard(AccessCard& card) {
    card.block();
}

void Administrator::unblockAccessCard(AccessCard& card) {
    card.unblock();
}

Discount Administrator::createDiscount(const std::string& code, double percent, const Date& validUntil, int usageLimit) {
    return Discount(code, percent, validUntil, usageLimit);
}
