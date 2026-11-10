#pragma once

#include <string>
#include <vector>

#include "Employee.h"
#include "TrainingProgram.h"

class Member;

// Тренер: ведёт персональных клиентов и групповые занятия.
class Coach : public Employee {
public:
    Coach(std::string employeeId, std::string firstName, std::string lastName, const Date& birthDate,
          std::string phone, std::string email, double baseSalary, const Date& hireDate, std::string specialization,
          int maxClients);

    std::string getPosition() const override;
    double calculateSalary() const override;

    const std::string& getSpecialization() const;
    double getRating() const;
    int getMaxClients() const;
    const std::vector<Member*>& getClients() const;
    const std::vector<std::string>& getCertificates() const;

    void assignClient(Member& member);
    void removeClient(const std::string& memberId);
    bool hasClient(const std::string& memberId) const;
    bool isAvailable() const;
    void addRating(int rating);
    void addCertificate(const std::string& certificate);
    TrainingProgram createProgram(const std::string& name, const std::string& goal, int weeks);

private:
    std::string specialization_;
    double ratingSum_;
    int ratingsCount_;
    int maxClients_;
    std::vector<Member*> clients_;
    std::vector<std::string> certificates_;
};
