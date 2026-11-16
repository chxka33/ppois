#include "Coach.h"

#include <algorithm>

#include "CoachUnavailableException.h"
#include "InvalidDataException.h"
#include "Member.h"

Coach::Coach(std::string employeeId, std::string firstName, std::string lastName,
             const Date& birthDate, std::string phone, std::string email,
             double baseSalary, const Date& hireDate, std::string specialization, int maxClients)
    : Employee(std::move(employeeId), std::move(firstName), std::move(lastName), birthDate,
               std::move(phone), std::move(email), baseSalary, hireDate),
      specialization_(std::move(specialization)), ratingSum_(0.0), ratingsCount_(0),
      maxClients_(maxClients) {
    if (maxClients_ < 0) {
        throw InvalidDataException("Max clients cannot be negative");
    }
}

std::string Coach::getPosition() const {
    return "Coach";
}

double Coach::calculateSalary() const {
    if (!active_) return 0.0;
    return Employee::calculateSalary() + clients_.size() * 50.0;
}

const std::string& Coach::getSpecialization() const {
    return specialization_;
}

double Coach::getRating() const {
    return ratingsCount_ > 0 ? (ratingSum_ / ratingsCount_) : 0.0;
}

int Coach::getMaxClients() const {
    return maxClients_;
}

const std::vector<Member*>& Coach::getClients() const {
    return clients_;
}

const std::vector<std::string>& Coach::getCertificates() const {
    return certificates_;
}

void Coach::assignClient(Member& member) {
    if (hasClient(member.getMemberId())) {
        return;
    }
    if (!isAvailable()) {
        throw CoachUnavailableException("Coach has reached maximum clients capacity");
    }
    clients_.push_back(&member);
}

void Coach::removeClient(const std::string& memberId) {
    clients_.erase(
        std::remove_if(clients_.begin(), clients_.end(),
                       [&memberId](Member* m) { return m && m->getMemberId() == memberId; }),
        clients_.end());
}

bool Coach::hasClient(const std::string& memberId) const {
    for (const auto* m : clients_) {
        if (m && m->getMemberId() == memberId) {
            return true;
        }
    }
    return false;
}

bool Coach::isAvailable() const {
    return static_cast<int>(clients_.size()) < maxClients_;
}

void Coach::addRating(int rating) {
    if (rating < 1 || rating > 5) {
        throw InvalidDataException("Rating must be between 1 and 5");
    }
    ratingSum_ += rating;
    ratingsCount_++;
}

void Coach::addCertificate(const std::string& certificate) {
    if (!certificate.empty()) {
        certificates_.push_back(certificate);
    }
}

TrainingProgram Coach::createProgram(const std::string& name, const std::string& goal, int weeks) {
    return TrainingProgram(name, goal, weeks, this);
}
