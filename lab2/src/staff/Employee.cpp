#include "staff/Employee.h"

#include "exceptions/InvalidDataException.h"

Employee::Employee(std::string employeeId, std::string firstName, std::string lastName,
                   const Date& birthDate, std::string phone, std::string email,
                   double baseSalary, const Date& hireDate)
    : Person(std::move(firstName), std::move(lastName), birthDate, std::move(phone), std::move(email)),
      employeeId_(std::move(employeeId)), baseSalary_(baseSalary), hireDate_(hireDate),
      active_(true), workedHours_(0.0) {
    if (employeeId_.empty()) {
        throw InvalidDataException("Employee ID cannot be empty");
    }
    if (baseSalary_ < 0.0) {
        throw InvalidDataException("Base salary cannot be negative");
    }
}

const std::string& Employee::getEmployeeId() const { return employeeId_; }
double Employee::getBaseSalary() const { return baseSalary_; }
const Date& Employee::getHireDate() const { return hireDate_; }
bool Employee::isActive() const { return active_; }
double Employee::getWorkedHours() const { return workedHours_; }

double Employee::calculateSalary() const {
    if (!active_) {
        return 0.0;
    }
    if (workedHours_ == 0.0) {
        return baseSalary_;
    }
    double hourlyRate = baseSalary_ / 160.0;
    if (workedHours_ <= 160.0) {
        return hourlyRate * workedHours_;
    }
    double overtimeHours = workedHours_ - 160.0;
    return baseSalary_ + overtimeHours * hourlyRate * 1.5;
}

int Employee::getExperienceYears(const Date& today) const {
    return hireDate_.yearsUntil(today);
}

void Employee::logWorkHours(double hours) {
    if (hours <= 0.0) {
        throw InvalidDataException("Work hours must be positive");
    }
    workedHours_ += hours;
}

void Employee::resetWorkHours() {
    workedHours_ = 0.0;
}

void Employee::raiseSalary(double percent) {
    if (percent <= 0.0) {
        throw InvalidDataException("Raise percent must be positive");
    }
    baseSalary_ *= (1.0 + percent / 100.0);
}

void Employee::terminate() {
    active_ = false;
}
