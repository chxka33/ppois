#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "common/Address.h"
#include "common/Date.h"
#include "common/Enums.h"
#include "common/Person.h"
#include "common/TimeSlot.h"
#include "exceptions/AccessDeniedException.h"
#include "exceptions/BookingConflictException.h"
#include "exceptions/ClassFullException.h"
#include "exceptions/CoachUnavailableException.h"
#include "exceptions/EmployeeNotFoundException.h"
#include "exceptions/EquipmentBrokenException.h"
#include "exceptions/FacilityFullException.h"
#include "exceptions/FacilityNotFoundException.h"
#include "exceptions/InsufficientFundsException.h"
#include "exceptions/InvalidDataException.h"
#include "exceptions/InvalidDateException.h"
#include "exceptions/InvalidPinException.h"
#include "exceptions/MedicalClearanceException.h"
#include "exceptions/MemberNotFoundException.h"
#include "exceptions/MembershipExpiredException.h"
#include "exceptions/MembershipFrozenException.h"
#include "exceptions/OutOfStockException.h"
#include "exceptions/PaymentException.h"
#include "exceptions/SportsClubException.h"

TEST_CASE("Date operations and validation", "[common][Date]") {
    Date defaultDate;
    REQUIRE(defaultDate.getDay() == 1);
    REQUIRE(defaultDate.getMonth() == 1);
    REQUIRE(defaultDate.getYear() == 2000);
    REQUIRE(defaultDate.toString() == "2000-01-01");

    Date leapDay(29, 2, 2024);
    REQUIRE(Date::isLeapYear(2024));
    REQUIRE(Date::isLeapYear(2000));
    REQUIRE_FALSE(Date::isLeapYear(1900));
    REQUIRE_FALSE(Date::isLeapYear(2023));

    REQUIRE(Date::daysInMonth(0, 2024) == 0);
    REQUIRE(Date::daysInMonth(13, 2024) == 0);
    REQUIRE(Date::daysInMonth(2, 2024) == 29);
    REQUIRE(Date::daysInMonth(2, 2023) == 28);
    REQUIRE(Date::daysInMonth(4, 2024) == 30);
    REQUIRE(Date::daysInMonth(6, 2024) == 30);
    REQUIRE(Date::daysInMonth(9, 2024) == 30);
    REQUIRE(Date::daysInMonth(11, 2024) == 30);
    REQUIRE(Date::daysInMonth(12, 2024) == 31);

    REQUIRE_FALSE(Date::isValid(1, 1, 1899));
    REQUIRE_FALSE(Date::isValid(1, 1, 2101));
    REQUIRE_FALSE(Date::isValid(1, 0, 2024));
    REQUIRE_FALSE(Date::isValid(32, 1, 2024));
    REQUIRE_THROWS_AS(Date(29, 2, 2023), InvalidDateException);

    Date d1(15, 3, 2025);
    Date d2 = d1.addDays(20);
    REQUIRE(d1.daysUntil(d2) == 20);
    REQUIRE(d1 < d2);
    REQUIRE(d1 <= d2);
    REQUIRE(d1 <= d1);
    REQUIRE(d1 == d1);
    REQUIRE(d1 != d2);
    REQUIRE(Date(10, 1, 2024) < Date(10, 1, 2025));
    REQUIRE(Date(10, 1, 2025) < Date(10, 2, 2025));

    Date birth(20, 5, 2000);
    REQUIRE(birth.yearsUntil(Date(19, 5, 2020)) == 19);
    REQUIRE(birth.yearsUntil(Date(20, 5, 2020)) == 20);
    REQUIRE(birth.yearsUntil(Date(10, 4, 2020)) == 19);
}

TEST_CASE("Address validation and formatting", "[common][Address]") {
    Address addr1("Minsk", "Pobediteley", "10", "220004");
    Address addr2("Minsk", "Lenina", "5", "220030");
    Address addr3("Brest", "Sovetskaya", "1", "224000");

    REQUIRE(addr1.getCity() == "Minsk");
    REQUIRE(addr1.getStreet() == "Pobediteley");
    REQUIRE(addr1.getBuilding() == "10");
    REQUIRE(addr1.getPostalCode() == "220004");
    REQUIRE(addr1.isSameCity(addr2));
    REQUIRE_FALSE(addr1.isSameCity(addr3));
    REQUIRE(addr1.toString() == "220004, Minsk, Pobediteley, 10");

    REQUIRE_FALSE(Address::isValidPostalCode(""));
    REQUIRE_FALSE(Address::isValidPostalCode("1234"));
    REQUIRE_FALSE(Address::isValidPostalCode("1234567"));
    REQUIRE_FALSE(Address::isValidPostalCode("12A45"));
    REQUIRE_THROWS_AS(Address("", "St", "1", "220004"), InvalidDataException);
    REQUIRE_THROWS_AS(Address("Minsk", "St", "1", "bad"), InvalidDataException);
}

TEST_CASE("TimeSlot overlap, containment and formatting", "[common][TimeSlot]") {
    Date date(17, 11, 2026);
    TimeSlot slot1(date, 600, 60);
    TimeSlot slot2(date, 630, 60);
    TimeSlot slot3(date, 660, 30);
    TimeSlot diffDay(Date(18, 11, 2026), 600, 60);

    REQUIRE(slot1.getDate() == date);
    REQUIRE(slot1.getStartMinute() == 600);
    REQUIRE(slot1.getDurationMinutes() == 60);
    REQUIRE(slot1.getEndMinute() == 660);
    REQUIRE(slot1.getDurationHours() == Catch::Approx(1.0));
    REQUIRE(slot1.toString() == "2026-11-17 10:00-11:00");

    REQUIRE(slot1.overlaps(slot2));
    REQUIRE_FALSE(slot1.overlaps(slot3));
    REQUIRE_FALSE(slot1.overlaps(diffDay));
    REQUIRE(slot1.contains(600));
    REQUIRE(slot1.contains(659));
    REQUIRE_FALSE(slot1.contains(660));

    REQUIRE_THROWS_AS(TimeSlot(date, -1, 60), InvalidDataException);
    REQUIRE_THROWS_AS(TimeSlot(date, 600, 0), InvalidDataException);
    REQUIRE_THROWS_AS(TimeSlot(date, 1400, 60), InvalidDataException);
}

TEST_CASE("Person validation and age/birthday logic", "[common][Person]") {
    Date birth(15, 5, 2005);
    Date today(15, 5, 2025);
    Person person("Ivan", "Petrov", birth, "+375 (29) 123-45-67", "ivan@mail.by");

    REQUIRE(person.getFirstName() == "Ivan");
    REQUIRE(person.getLastName() == "Petrov");
    REQUIRE(person.getBirthDate() == birth);
    REQUIRE(person.getPhone() == "+375 (29) 123-45-67");
    REQUIRE(person.getEmail() == "ivan@mail.by");
    REQUIRE(person.getFullName() == "Ivan Petrov");
    REQUIRE(person.getAge(today) == 20);
    REQUIRE(person.isAdult(today));
    REQUIRE(person.isBirthday(today));
    REQUIRE_FALSE(person.isBirthday(Date(16, 5, 2025)));

    person.updateContacts("80297654321", "new@domain.org");
    REQUIRE(person.getPhone() == "80297654321");
    REQUIRE(person.getEmail() == "new@domain.org");

    REQUIRE_THROWS_AS(person.updateContacts("123", "new@domain.org"), InvalidDataException);
    REQUIRE_THROWS_AS(person.updateContacts("80297654321", "bad_email"), InvalidDataException);

    REQUIRE_FALSE(Person::isValidEmail(""));
    REQUIRE_FALSE(Person::isValidEmail("@mail.ru"));
    REQUIRE_FALSE(Person::isValidEmail("user@.ru"));
    REQUIRE_FALSE(Person::isValidEmail("user@mail."));
    REQUIRE_FALSE(Person::isValidPhone(""));
    REQUIRE_FALSE(Person::isValidPhone("+"));
    REQUIRE_FALSE(Person::isValidPhone("+37529abc1234"));
    REQUIRE_THROWS_AS(Person("", "Petrov", birth, "+375291234567", "a@b.c"), InvalidDataException);
    REQUIRE_THROWS_AS(Person("Ivan", "Petrov", birth, "123", "a@b.c"), InvalidDataException);
    REQUIRE_THROWS_AS(Person("Ivan", "Petrov", birth, "+375291234567", "bad"), InvalidDataException);

    SportsClubException baseEx("msg");
    REQUIRE(std::string(baseEx.what()) == "msg");
}
