#include "Person.h"

#include <algorithm>
#include <cctype>

#include "InvalidDataException.h"

Person::Person(std::string firstName, std::string lastName, const Date& birthDate, std::string phone, std::string email)
    : firstName_(std::move(firstName)), lastName_(std::move(lastName)), birthDate_(birthDate),
      phone_(std::move(phone)), email_(std::move(email)) {
    if (firstName_.empty() || lastName_.empty()) {
        throw InvalidDataException("Name fields cannot be empty");
    }
    if (!isValidPhone(phone_)) {
        throw InvalidDataException("Invalid phone number");
    }
    if (!isValidEmail(email_)) {
        throw InvalidDataException("Invalid email");
    }
}

const std::string& Person::getFirstName() const { return firstName_; }
const std::string& Person::getLastName() const { return lastName_; }
const Date& Person::getBirthDate() const { return birthDate_; }
const std::string& Person::getPhone() const { return phone_; }
const std::string& Person::getEmail() const { return email_; }

std::string Person::getFullName() const {
    return firstName_ + " " + lastName_;
}

int Person::getAge(const Date& today) const {
    return birthDate_.yearsUntil(today);
}

bool Person::isAdult(const Date& today) const {
    return getAge(today) >= 18;
}

bool Person::isBirthday(const Date& today) const {
    return today.getDay() == birthDate_.getDay() && today.getMonth() == birthDate_.getMonth();
}

void Person::updateContacts(const std::string& phone, const std::string& email) {
    if (!isValidPhone(phone)) {
        throw InvalidDataException("Invalid phone number");
    }
    if (!isValidEmail(email)) {
        throw InvalidDataException("Invalid email");
    }
    phone_ = phone;
    email_ = email;
}

bool Person::isValidEmail(const std::string& email) {
    if (email.empty()) return false;
    auto at = email.find('@');
    if (at == std::string::npos || at == 0) return false;
    auto dot = email.find('.', at + 1);
    if (dot == std::string::npos || dot == at + 1 || dot == email.length() - 1) return false;
    return true;
}

bool Person::isValidPhone(const std::string& phone) {
    if (phone.empty()) return false;
    size_t start = (phone[0] == '+') ? 1 : 0;
    if (start == phone.length()) return false;
    int digits = 0;
    for (size_t i = start; i < phone.length(); ++i) {
        if (std::isdigit(static_cast<unsigned char>(phone[i]))) {
            ++digits;
        } else if (phone[i] != '-' && phone[i] != ' ' && phone[i] != '(' && phone[i] != ')') {
            return false;
        }
    }
    return digits >= 7;
}
