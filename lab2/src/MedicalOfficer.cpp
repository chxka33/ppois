#include "MedicalOfficer.h"

#include "InvalidDataException.h"
#include "Member.h"

MedicalOfficer::MedicalOfficer(std::string employeeId, std::string firstName, std::string lastName,
                               const Date& birthDate, std::string phone, std::string email,
                               double baseSalary, const Date& hireDate, std::string licenseNumber)
    : Employee(std::move(employeeId), std::move(firstName), std::move(lastName), birthDate,
               std::move(phone), std::move(email), baseSalary, hireDate),
      licenseNumber_(std::move(licenseNumber)), examinationsCount_(0) {
    if (licenseNumber_.empty()) {
        throw InvalidDataException("License number cannot be empty");
    }
}

std::string MedicalOfficer::getPosition() const {
    return "MedicalOfficer";
}

const std::string& MedicalOfficer::getLicenseNumber() const {
    return licenseNumber_;
}

int MedicalOfficer::getExaminationsCount() const {
    return examinationsCount_;
}

MedicalCertificate MedicalOfficer::examine(Member& member, const Date& today, int validityDays, bool allowsHighIntensity) {
    if (validityDays <= 0) {
        throw InvalidDataException("Validity days must be positive");
    }
    examinationsCount_++;
    std::string certNumber = "MC-" + employeeId_ + "-" + std::to_string(examinationsCount_);
    Date validUntil = today.addDays(validityDays);
    MedicalCertificate cert(certNumber, today, validUntil, getFullName(), allowsHighIntensity);
    member.getHealthRecord().addCertificate(cert);
    return cert;
}

void MedicalOfficer::addContraindication(Member& member, const std::string& contraindication) {
    member.getHealthRecord().addContraindication(contraindication);
}
