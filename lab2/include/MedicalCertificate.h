#pragma once

#include <string>

#include "Date.h"
#include "Enums.h"

// Медицинская справка о допуске к тренировкам.
class MedicalCertificate {
public:
    MedicalCertificate(std::string number, const Date& issueDate, const Date& validUntil, std::string doctorName,
                       bool allowsHighIntensity);

    const std::string& getNumber() const;
    const Date& getIssueDate() const;
    const Date& getValidUntil() const;
    const std::string& getDoctorName() const;
    bool allowsHighIntensity() const;

    bool isValid(const Date& today) const;
    bool permits(Intensity intensity, const Date& today) const;
    int daysUntilExpiry(const Date& today) const;

private:
    std::string number_;
    Date issueDate_;
    Date validUntil_;
    std::string doctorName_;
    bool allowsHighIntensity_;
};
