#include "SportsClub.h"

#include <algorithm>

#include "EmployeeNotFoundException.h"
#include "FacilityNotFoundException.h"
#include "InvalidDataException.h"
#include "MemberNotFoundException.h"
#include "Payment.h"
#include "SportsClubException.h"

SportsClub::SportsClub(std::string name, const Address& address, double initialBalance,
                       const LoyaltyProgram& loyaltyProgram)
    : name_(std::move(name)), address_(address),
      finance_(initialBalance), loyaltyProgram_(loyaltyProgram) {
    if (name_.empty()) {
        throw InvalidDataException("Club name cannot be empty");
    }
}

const std::string& SportsClub::getName() const { return name_; }
const Address& SportsClub::getAddress() const { return address_; }
const std::vector<std::unique_ptr<Member>>& SportsClub::getMembers() const { return members_; }
const std::vector<std::unique_ptr<Employee>>& SportsClub::getEmployees() const { return employees_; }
const std::vector<std::unique_ptr<Facility>>& SportsClub::getFacilities() const { return facilities_; }
const std::vector<MembershipPlan>& SportsClub::getPlans() const { return plans_; }
Schedule& SportsClub::getSchedule() { return schedule_; }
BookingSystem& SportsClub::getBookingSystem() { return bookingSystem_; }
FinanceDepartment& SportsClub::getFinance() { return finance_; }
const FinanceDepartment& SportsClub::getFinance() const { return finance_; }
const LoyaltyProgram& SportsClub::getLoyaltyProgram() const { return loyaltyProgram_; }

Member& SportsClub::registerMember(std::unique_ptr<Member> member) {
    if (!member) {
        throw InvalidDataException("Member is null");
    }
    members_.push_back(std::move(member));
    return *members_.back();
}

void SportsClub::removeMember(const std::string& memberId) {
    auto it = std::find_if(members_.begin(), members_.end(),
                           [&memberId](const std::unique_ptr<Member>& m) {
                               return m && m->getMemberId() == memberId;
                           });
    if (it == members_.end()) {
        throw MemberNotFoundException("Member not found: " + memberId);
    }
    members_.erase(it);
}

Member& SportsClub::findMember(const std::string& memberId) {
    for (auto& m : members_) {
        if (m && m->getMemberId() == memberId) {
            return *m;
        }
    }
    throw MemberNotFoundException("Member not found: " + memberId);
}

Employee& SportsClub::hireEmployee(std::unique_ptr<Employee> employee) {
    if (!employee) {
        throw InvalidDataException("Employee is null");
    }
    employees_.push_back(std::move(employee));
    return *employees_.back();
}

void SportsClub::fireEmployee(const std::string& employeeId) {
    Employee& emp = findEmployee(employeeId);
    emp.terminate();
}

Employee& SportsClub::findEmployee(const std::string& employeeId) {
    for (auto& emp : employees_) {
        if (emp && emp->getEmployeeId() == employeeId) {
            return *emp;
        }
    }
    throw EmployeeNotFoundException("Employee not found: " + employeeId);
}

Facility& SportsClub::addFacility(std::unique_ptr<Facility> facility) {
    if (!facility) {
        throw InvalidDataException("Facility is null");
    }
    facilities_.push_back(std::move(facility));
    return *facilities_.back();
}

Facility& SportsClub::findFacility(const std::string& facilityId) {
    for (auto& f : facilities_) {
        if (f && f->getId() == facilityId) {
            return *f;
        }
    }
    throw FacilityNotFoundException("Facility not found: " + facilityId);
}

void SportsClub::addPlan(const MembershipPlan& plan) {
    plans_.push_back(plan);
}

const MembershipPlan& SportsClub::findPlan(const std::string& planName) const {
    for (const auto& p : plans_) {
        if (p.getName() == planName) {
            return p;
        }
    }
    throw SportsClubException("Membership plan not found: " + planName);
}

void SportsClub::sellMembership(const std::string& memberId, const std::string& planName, const Date& today) {
    Member& member = findMember(memberId);
    const MembershipPlan& plan = findPlan(planName);
    double cost = plan.calculateTotalPrice();
    member.purchaseMembership(plan, today);
    Payment payment("MEMBERSHIP_" + memberId + "_" + today.toString(), memberId, cost, today, PaymentMethod::Balance);
    payment.process();
    finance_.registerPayment(payment);
}

int SportsClub::countActiveMembers(const Date& today) const {
    int count = 0;
    for (const auto& m : members_) {
        if (m && m->hasActiveMembership(today)) {
            count++;
        }
    }
    return count;
}
