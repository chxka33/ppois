#pragma once

#include <string>

#include "Date.h"

// Базовый класс для всех людей в клубе (клиенты и сотрудники).
class Person {
public:
    Person(std::string firstName, std::string lastName, const Date& birthDate, std::string phone, std::string email);
    virtual ~Person() = default;

    const std::string& getFirstName() const;
    const std::string& getLastName() const;
    const Date& getBirthDate() const;
    const std::string& getPhone() const;
    const std::string& getEmail() const;

    std::string getFullName() const;
    int getAge(const Date& today) const;
    bool isAdult(const Date& today) const;
    bool isBirthday(const Date& today) const;
    void updateContacts(const std::string& phone, const std::string& email);

    static bool isValidEmail(const std::string& email);
    static bool isValidPhone(const std::string& phone);

protected:
    std::string firstName_;
    std::string lastName_;
    Date birthDate_;
    std::string phone_;
    std::string email_;
};
