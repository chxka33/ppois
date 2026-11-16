#include "members/MedicalCertificate.h"

#include "exceptions/InvalidDataException.h"
#include "exceptions/InvalidDateException.h"

MedicalCertificate::MedicalCertificate(std::string number, const Date& issueDate, const Date& validUntil,
                                       std::string doctorName, bool allowsHighIntensity)
    : number_(std::move(number)), issueDate_(issueDate), validUntil_(validUntil),
      doctorName_(std::move(doctorName)), allowsHighIntensity_(allowsHighIntensity) {
    if (number_.empty() || doctorName_.empty()) {
        throw InvalidDataException("Certificate fields cannot be empty");
    }
    if (validUntil_ < issueDate_) {
        throw InvalidDateException("Certificate validUntil cannot be earlier than issueDate");
    }
}

const std::string& MedicalCertificate::getNumber() const { return number_; }
const Date& MedicalCertificate::getIssueDate() const { return issueDate_; }
const Date& MedicalCertificate::getValidUntil() const { return validUntil_; }
const std::string& MedicalCertificate::getDoctorName() const { return doctorName_; }
bool MedicalCertificate::allowsHighIntensity() const { return allowsHighIntensity_; }

bool MedicalCertificate::isValid(const Date& today) const {
    return issueDate_ <= today && today <= validUntil_;
}

bool MedicalCertificate::permits(Intensity intensity, const Date& today) const {
    if (!isValid(today)) {
        return false;
    }
    if (intensity == Intensity::High) {
        return allowsHighIntensity_;
    }
    return true;
}

int MedicalCertificate::daysUntilExpiry(const Date& today) const {
    return today.daysUntil(validUntil_);
}
