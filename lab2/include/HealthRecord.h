#pragma once

#include <string>
#include <vector>

#include "Enums.h"
#include "MedicalCertificate.h"

// Медицинская карта клиента.
class HealthRecord {
public:
    HealthRecord() = default;

    void addCertificate(const MedicalCertificate& certificate);
    void addContraindication(const std::string& contraindication);
    bool hasContraindication(const std::string& contraindication) const;
    bool hasValidCertificate(const Date& today) const;
    bool canPerform(Intensity intensity, const Date& today) const;
    const MedicalCertificate& getLatestCertificate() const;
    const std::vector<std::string>& getContraindications() const;

private:
    std::vector<MedicalCertificate> certificates_;
    std::vector<std::string> contraindications_;
};
