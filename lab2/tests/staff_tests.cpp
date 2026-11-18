#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "club/SportsClub.h"
#include "exceptions/AccessDeniedException.h"
#include "exceptions/CoachUnavailableException.h"
#include "exceptions/InvalidDataException.h"
#include "exceptions/InvalidPinException.h"
#include "exceptions/SportsClubException.h"
#include "facilities/Gym.h"
#include "facilities/Treadmill.h"
#include "members/AccessCard.h"
#include "members/LoyaltyProgram.h"
#include "members/Member.h"
#include "members/MembershipPlan.h"
#include "staff/Administrator.h"
#include "staff/Cleaner.h"
#include "staff/Coach.h"
#include "staff/MedicalOfficer.h"
#include "staff/Receptionist.h"
#include "staff/Technician.h"

TEST_CASE("Employee base methods and Coach logic", "[staff][Employee][Coach]") {
    Date birth(10, 5, 1990);
    Date hire(1, 1, 2020);
    Date today(1, 6, 2026);

    REQUIRE_THROWS_AS(Coach("", "A", "B", birth, "+375291234567", "a@b.c", 1000.0, hire, "Box", 5), InvalidDataException);
    REQUIRE_THROWS_AS(Coach("E1", "A", "B", birth, "+375291234567", "a@b.c", -10.0, hire, "Box", 5), InvalidDataException);
    REQUIRE_THROWS_AS(Coach("E1", "A", "B", birth, "+375291234567", "a@b.c", 1000.0, hire, "Box", -1), InvalidDataException);

    Coach coach("E1", "Alex", "Smirnov", birth, "+375291234567", "a@b.c", 1600.0, hire, "CrossFit", 1);
    REQUIRE(coach.getEmployeeId() == "E1");
    REQUIRE(coach.getBaseSalary() == Catch::Approx(1600.0));
    REQUIRE(coach.getHireDate() == hire);
    REQUIRE(coach.isActive());
    REQUIRE(coach.getWorkedHours() == Catch::Approx(0.0));
    REQUIRE(coach.getPosition() == "Coach");
    REQUIRE(coach.getSpecialization() == "CrossFit");
    REQUIRE(coach.getMaxClients() == 1);
    REQUIRE(coach.getExperienceYears(today) == 6);
    REQUIRE(coach.getRating() == Catch::Approx(0.0));

    REQUIRE_THROWS_AS(coach.logWorkHours(0.0), InvalidDataException);
    coach.logWorkHours(80.0);
    REQUIRE(coach.Employee::calculateSalary() == Catch::Approx(800.0));
    coach.logWorkHours(100.0);
    REQUIRE(coach.Employee::calculateSalary() == Catch::Approx(1600.0 + 20.0 * 10.0 * 1.5));
    coach.resetWorkHours();
    REQUIRE(coach.getWorkedHours() == Catch::Approx(0.0));

    REQUIRE_THROWS_AS(coach.raiseSalary(0.0), InvalidDataException);
    coach.raiseSalary(10.0);
    REQUIRE(coach.getBaseSalary() == Catch::Approx(1760.0));

    Member m1("M1", "Ivan", "P", birth, "+375291112233", "i@b.c");
    Member m2("M2", "Oleg", "K", birth, "+375291112234", "o@b.c");
    coach.assignClient(m1);
    coach.assignClient(m1);
    REQUIRE(coach.getClients().size() == 1);
    REQUIRE_THROWS_AS(coach.assignClient(m2), CoachUnavailableException);
    REQUIRE(coach.calculateSalary() == Catch::Approx(1760.0 + 50.0));
    coach.removeClient("M1");
    REQUIRE(coach.getClients().empty());

    REQUIRE_THROWS_AS(coach.addRating(0), InvalidDataException);
    REQUIRE_THROWS_AS(coach.addRating(6), InvalidDataException);
    coach.addRating(5);
    coach.addRating(3);
    REQUIRE(coach.getRating() == Catch::Approx(4.0));

    coach.addCertificate("");
    coach.addCertificate("PRO-1");
    REQUIRE(coach.getCertificates().size() == 1);
    auto prog = coach.createProgram("Strength", "Mass", 8);
    REQUIRE(prog.getAuthor() == &coach);

    coach.terminate();
    REQUIRE_FALSE(coach.isActive());
    REQUIRE(coach.Employee::calculateSalary() == Catch::Approx(0.0));
    REQUIRE(coach.calculateSalary() == Catch::Approx(0.0));
}

TEST_CASE("Administrator, Cleaner, MedicalOfficer, Receptionist and Technician", "[staff]") {
    Date birth(10, 5, 1990);
    Date today(1, 6, 2026);
    Address addr("Minsk", "Main", "1", "220000");
    LoyaltyProgram loyalty(10, 1.0, 0.5);
    SportsClub club("Club", addr, 1000.0, loyalty);

    Administrator nullAdmin("A0", "Ann", "B", birth, "+375291112233", "a@b.c", 1000.0, today, nullptr);
    REQUIRE(nullAdmin.getPosition() == "Administrator");
    REQUIRE(nullAdmin.getClub() == nullptr);
    REQUIRE_THROWS_AS(nullAdmin.registerMember(nullptr), SportsClubException);

    Administrator admin("A1", "Ann", "B", birth, "+375291112233", "a@b.c", 1000.0, today, &club);
    auto memPtr = std::make_unique<Member>("M1", "Ivan", "P", birth, "+375291112233", "i@b.c");
    Member& mem = admin.registerMember(std::move(memPtr));
    REQUIRE(admin.getRegisteredMembersCount() == 1);

    AccessCard card("C1", "M1", today, "1234");
    admin.blockAccessCard(card);
    REQUIRE(card.isBlocked());
    admin.unblockAccessCard(card);
    REQUIRE_FALSE(card.isBlocked());
    Discount disc = admin.createDiscount("SALE", 15.0, today.addDays(10), 5);
    REQUIRE(disc.getPercent() == Catch::Approx(15.0));

    Cleaner cleaner("CL1", "Bob", "C", birth, "+375291112233", "c@b.c", 800.0, today);
    Gym gym("G1", "Gym", 10, AccessLevel::Basic, 100.0);
    REQUIRE(cleaner.getPosition() == "Cleaner");
    cleaner.assignFacility(gym);
    cleaner.assignFacility(gym);
    REQUIRE(cleaner.getAssignedFacilities().size() == 1);
    cleaner.cleanFacility(gym, today);
    REQUIRE(cleaner.getCleanedCount() == 1);
    REQUIRE(gym.getLastCleaned() == today);

    REQUIRE_THROWS_AS(MedicalOfficer("D0", "D", "V", birth, "+375291112233", "d@b.c", 1500.0, today, ""), InvalidDataException);
    MedicalOfficer doc("D1", "Doc", "V", birth, "+375291112233", "d@b.c", 1500.0, today, "LIC-1");
    REQUIRE(doc.getPosition() == "MedicalOfficer");
    REQUIRE(doc.getLicenseNumber() == "LIC-1");
    REQUIRE_THROWS_AS(doc.examine(mem, today, 0, true), InvalidDataException);
    doc.examine(mem, today, 30, true);
    doc.addContraindication(mem, "Asthma");
    REQUIRE(doc.getExaminationsCount() == 1);
    REQUIRE(mem.getHealthRecord().hasContraindication("Asthma"));

    Receptionist rec("R1", "Eva", "R", birth, "+375291112233", "r@b.c", 900.0, today, 2);
    REQUIRE(rec.getPosition() == "Receptionist");
    REQUIRE(rec.getDeskNumber() == 2);
    REQUIRE_THROWS_AS(rec.checkIn(mem, gym, "1234", today, 600), AccessDeniedException);
    mem.issueAccessCard(std::make_unique<AccessCard>("C1", "M1", today, "1234"));
    REQUIRE_THROWS_AS(rec.checkIn(mem, gym, "0000", today, 600), InvalidPinException);
    REQUIRE_THROWS_AS(rec.checkIn(mem, gym, "1234", today, 600), AccessDeniedException);

    MembershipPlan plan("Basic", 100.0, 1, AccessLevel::Basic, 0, false, false);
    mem.deposit(200.0);
    mem.purchaseMembership(plan, today);
    Visit visit = rec.checkIn(mem, gym, "1234", today, 600);
    REQUIRE(rec.getCheckInsCount() == 1);
    rec.checkOut(mem, visit, 660, loyalty);
    REQUIRE(gym.getOccupancy() == 0);

    GuestPass gpass = rec.issueGuestPass("Guest", today, 20.0);
    REQUIRE(rec.getIssuedPassesCount() == 1);
    rec.admitGuest(gpass, gym, today);
    REQUIRE(gym.getOccupancy() == 1);

    REQUIRE_THROWS_AS(Technician("T0", "T", "M", birth, "+375291112233", "t@b.c", 1100.0, today, ""), InvalidDataException);
    Technician tech("T1", "Tom", "M", birth, "+375291112233", "t@b.c", 1100.0, today, "Master");
    REQUIRE(tech.getPosition() == "Technician");
    REQUIRE(tech.getQualification() == "Master");
    Treadmill tm("TR1", "Run", today, 10.0, 20.0, 120.0);
    tm.markBroken();
    REQUIRE(tech.inspect(tm));
    auto mrec = tech.repair(tm, today, 50.0, "Fix belt");
    REQUIRE(tech.getRepairsCount() == 1);
    REQUIRE(mrec.getCost() == Catch::Approx(50.0));
}
