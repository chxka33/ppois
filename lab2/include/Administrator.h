#pragma once

#include <memory>
#include <string>

#include "Discount.h"
#include "Employee.h"

class SportsClub;
class Member;
class AccessCard;

// Администратор клуба: регистрирует клиентов, управляет пропусками и скидками.
class Administrator : public Employee {
public:
    Administrator(std::string employeeId, std::string firstName, std::string lastName, const Date& birthDate,
                  std::string phone, std::string email, double baseSalary, const Date& hireDate, SportsClub* club);

    std::string getPosition() const override;

    SportsClub* getClub() const;
    int getRegisteredMembersCount() const;

    Member& registerMember(std::unique_ptr<Member> member);
    void blockAccessCard(AccessCard& card);
    void unblockAccessCard(AccessCard& card);
    Discount createDiscount(const std::string& code, double percent, const Date& validUntil, int usageLimit);

private:
    SportsClub* club_;
    int registeredMembersCount_;
};
