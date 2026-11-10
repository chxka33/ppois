#pragma once

#include <string>

#include "Employee.h"
#include "GuestPass.h"
#include "Visit.h"

class Member;
class Facility;
class LoyaltyProgram;

// Сотрудник ресепшена: пропускает клиентов и гостей.
class Receptionist : public Employee {
public:
    Receptionist(std::string employeeId, std::string firstName, std::string lastName, const Date& birthDate,
                 std::string phone, std::string email, double baseSalary, const Date& hireDate, int deskNumber);

    std::string getPosition() const override;

    int getDeskNumber() const;
    int getCheckInsCount() const;
    int getIssuedPassesCount() const;

    Visit checkIn(Member& member, Facility& facility, const std::string& pin, const Date& today, int minute);
    void checkOut(Member& member, Visit& visit, int minute, const LoyaltyProgram& loyaltyProgram);
    GuestPass issueGuestPass(const std::string& guestName, const Date& validDate, double price);
    void admitGuest(GuestPass& pass, Facility& facility, const Date& today);

private:
    int deskNumber_;
    int checkInsCount_;
    int issuedPassesCount_;
};
