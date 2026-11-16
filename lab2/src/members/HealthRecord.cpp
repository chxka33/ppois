#include "members/HealthRecord.h"

#include <algorithm>

#include "exceptions/MedicalClearanceException.h"

void HealthRecord::addCertificate(const MedicalCertificate& certificate) {
    certificates_.push_back(certificate);
}

void HealthRecord::addContraindication(const std::string& contraindication) {
    if (!hasContraindication(contraindication)) {
        contraindications_.push_back(contraindication);
    }
}

bool HealthRecord::hasContraindication(const std::string& contraindication) const {
    return std::find(contraindications_.begin(), contraindications_.end(), contraindication) != contraindications_.end();
}

bool HealthRecord::hasValidCertificate(const Date& today) const {
    for (const auto& cert : certificates_) {
        if (cert.isValid(today)) {
            return true;
        }
    }
    return false;
}

bool HealthRecord::canPerform(Intensity intensity, const Date& today) const {
    for (const auto& cert : certificates_) {
        if (cert.permits(intensity, today)) {
            return true;
        }
    }
    return false;
}

const MedicalCertificate& HealthRecord::getLatestCertificate() const {
    if (certificates_.empty()) {
        throw MedicalClearanceException("No medical certificates found in health record");
    }
    const MedicalCertificate* latest = &certificates_[0];
    for (const auto& cert : certificates_) {
        if (latest->getIssueDate() < cert.getIssueDate()) {
            latest = &cert;
        }
    }
    return *latest;
}

const std::vector<std::string>& HealthRecord::getContraindications() const {
    return contraindications_;
}
