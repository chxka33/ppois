#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Address.h"
#include "BookingSystem.h"
#include "Employee.h"
#include "Facility.h"
#include "FinanceDepartment.h"
#include "LoyaltyProgram.h"
#include "Member.h"
#include "MembershipPlan.h"
#include "Schedule.h"

// Корневой класс предметной области: спортивный клуб.
class SportsClub {
public:
    SportsClub(std::string name, const Address& address, double initialBalance,
               const LoyaltyProgram& loyaltyProgram);

    const std::string& getName() const;
    const Address& getAddress() const;
    const std::vector<std::unique_ptr<Member>>& getMembers() const;
    const std::vector<std::unique_ptr<Employee>>& getEmployees() const;
    const std::vector<std::unique_ptr<Facility>>& getFacilities() const;
    const std::vector<MembershipPlan>& getPlans() const;
    Schedule& getSchedule();
    BookingSystem& getBookingSystem();
    FinanceDepartment& getFinance();
    const FinanceDepartment& getFinance() const;
    const LoyaltyProgram& getLoyaltyProgram() const;

    Member& registerMember(std::unique_ptr<Member> member);
    void removeMember(const std::string& memberId);
    Member& findMember(const std::string& memberId);
    Employee& hireEmployee(std::unique_ptr<Employee> employee);
    void fireEmployee(const std::string& employeeId);
    Employee& findEmployee(const std::string& employeeId);
    Facility& addFacility(std::unique_ptr<Facility> facility);
    Facility& findFacility(const std::string& facilityId);
    void addPlan(const MembershipPlan& plan);
    const MembershipPlan& findPlan(const std::string& planName) const;
    void sellMembership(const std::string& memberId, const std::string& planName, const Date& today);
    int countActiveMembers(const Date& today) const;

private:
    std::string name_;
    Address address_;
    std::vector<std::unique_ptr<Member>> members_;
    std::vector<std::unique_ptr<Employee>> employees_;
    std::vector<std::unique_ptr<Facility>> facilities_;
    std::vector<MembershipPlan> plans_;
    Schedule schedule_;
    BookingSystem bookingSystem_;
    FinanceDepartment finance_;
    LoyaltyProgram loyaltyProgram_;
};
