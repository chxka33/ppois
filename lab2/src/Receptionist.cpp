#include "Receptionist.h"

#include "AccessDeniedException.h"
#include "Facility.h"
#include "InvalidPinException.h"
#include "LoyaltyProgram.h"
#include "Member.h"

Receptionist::Receptionist(std::string employeeId, std::string firstName, std::string lastName,
                           const Date& birthDate, std::string phone, std::string email,
                           double baseSalary, const Date& hireDate, int deskNumber)
    : Employee(std::move(employeeId), std::move(firstName), std::move(lastName), birthDate,
               std::move(phone), std::move(email), baseSalary, hireDate),
      deskNumber_(deskNumber), checkInsCount_(0), issuedPassesCount_(0) {}

std::string Receptionist::getPosition() const {
    return "Receptionist";
}

int Receptionist::getDeskNumber() const {
    return deskNumber_;
}

int Receptionist::getCheckInsCount() const {
    return checkInsCount_;
}

int Receptionist::getIssuedPassesCount() const {
    return issuedPassesCount_;
}

Visit Receptionist::checkIn(Member& member, Facility& facility, const std::string& pin, const Date& today, int minute) {
    AccessCard* card = member.getAccessCard();
    if (!card) {
        throw AccessDeniedException("Member has no access card");
    }
    if (!card->verifyPin(pin)) {
        throw InvalidPinException("Incorrect card PIN");
    }
    if (!member.canAccessFacility(facility, today)) {
        throw AccessDeniedException("Member cannot access facility");
    }
    facility.enter(member, today);
    checkInsCount_++;
    return Visit(member.getMemberId(), &facility, today, minute);
}

void Receptionist::checkOut(Member& member, Visit& visit, int minute, const LoyaltyProgram& loyaltyProgram) {
    visit.close(minute);
    if (visit.getFacility()) {
        visit.getFacility()->leave(member.getMemberId());
    }
    member.registerVisit(visit, loyaltyProgram);
}

GuestPass Receptionist::issueGuestPass(const std::string& guestName, const Date& validDate, double price) {
    issuedPassesCount_++;
    std::string code = "GP-" + std::to_string(issuedPassesCount_);
    return GuestPass(code, guestName, validDate, price);
}

void Receptionist::admitGuest(GuestPass& pass, Facility& facility, const Date& today) {
    pass.use(today);
    facility.enterGuest();
}
