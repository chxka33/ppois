#pragma once

#include <string>

#include "Employee.h"
#include "MedicalCertificate.h"

class Member;

// Врач клуба: проводит медосмотр и выдаёт допуски.
class MedicalOfficer : public Employee {
public:
    MedicalOfficer(std::string employeeId, std::string firstName, std::string lastName, const Date& birthDate,
                   std::string phone, std::string email, double baseSalary, const Date& hireDate,
                   std::string licenseNumber);

    std::string getPosition() const override;

    const std::string& getLicenseNumber() const;
    int getExaminationsCount() const;

    MedicalCertificate examine(Member& member, const Date& today, int validityDays, bool allowsHighIntensity);
    void addContraindication(Member& member, const std::string& contraindication);

private:
    std::string licenseNumber_;
    int examinationsCount_;
};
