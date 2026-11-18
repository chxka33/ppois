#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "club/NotificationService.h"
#include "club/ReportGenerator.h"
#include "club/Review.h"
#include "club/SportsClub.h"
#include "exceptions/EmployeeNotFoundException.h"
#include "exceptions/FacilityNotFoundException.h"
#include "exceptions/InvalidDataException.h"
#include "exceptions/MemberNotFoundException.h"
#include "exceptions/SportsClubException.h"
#include "facilities/Gym.h"
#include "facilities/Treadmill.h"
#include "finance/Invoice.h"
#include "members/LoyaltyProgram.h"
#include "members/MedicalCertificate.h"
#include "members/Member.h"
#include "members/MembershipPlan.h"
#include "staff/Coach.h"
#include "training/GroupClass.h"
#include "training/Workout.h"

TEST_CASE("Review, NotificationService, ReportGenerator and SportsClub", "[club][SportsClub]") {
    Date birth(15, 5, 1995);
    Date today(15, 5, 2026);
    Coach coach("C1", "Alex", "S", birth, "+375291112233", "c@b.c", 1000.0, today, "Fit", 5);

    REQUIRE_THROWS_AS(Review("M1", &coach, 0, "Bad", today), InvalidDataException);
    Review rev("M1", &coach, 5, "Great coach!", today);
    REQUIRE(rev.getAuthorId() == "M1");
    REQUIRE(rev.getCoach() == &coach);
    REQUIRE(rev.getRating() == 5);
    REQUIRE(rev.getComment() == "Great coach!");
    REQUIRE(rev.getDate() == today);
    REQUIRE(rev.isPositive());
    REQUIRE_FALSE(rev.isPublished());
    rev.publish();
    REQUIRE(rev.isPublished());

    NotificationService notifier(35);
    REQUIRE(notifier.getReminderDays() == 35);
    Member mem("M1", "Ivan", "P", birth, "+375291112233", "ivan@b.c");
    REQUIRE_FALSE(notifier.notifyMembershipExpiry(mem, today));

    MembershipPlan plan("Basic", 100.0, 1, AccessLevel::Basic, 0, false, false);
    mem.deposit(500.0);
    mem.purchaseMembership(plan, today);
    REQUIRE(notifier.notifyMembershipExpiry(mem, today));
    NotificationService shortNotifier(5);
    REQUIRE_FALSE(shortNotifier.notifyMembershipExpiry(mem, today));

    mem.getHealthRecord().addCertificate(MedicalCertificate("MC1", today, today.addDays(30), "Doc", true));
    Workout workout("Yoga", Intensity::Low, 60, 0.1);
    TimeSlot slot(today, 600, 60);
    GroupClass gc("YogaClass", workout, &coach, nullptr, slot, 10);
    gc.enroll(mem, today);
    REQUIRE(notifier.notifyClassCancelled(gc) == 1);

    Invoice inv("INV1", &mem, today.addDays(-10), today.addDays(-1));
    REQUIRE(notifier.notifyOverdueInvoice(inv, today));
    Invoice futureInv("INV2", &mem, today, today.addDays(5));
    REQUIRE_FALSE(notifier.notifyOverdueInvoice(futureInv, today));

    REQUIRE(notifier.sendBirthdayGreeting(mem, today));
    REQUIRE_FALSE(notifier.sendBirthdayGreeting(mem, today.addDays(1)));
    REQUIRE(notifier.getSentMessages().size() == 4);
    notifier.clearLog();
    REQUIRE(notifier.getSentMessages().empty());

    Address addr("Minsk", "Street", "1", "220000");
    LoyaltyProgram loyalty(10, 1.0, 0.5);
    REQUIRE_THROWS_AS(SportsClub("", addr, 1000.0, loyalty), InvalidDataException);
    SportsClub club("Olympus", addr, 1000.0, loyalty);
    REQUIRE(club.getName() == "Olympus");
    REQUIRE(club.getAddress().getCity() == "Minsk");
    REQUIRE(club.getBookingSystem().getBookings().empty());
    REQUIRE(club.getEmployees().empty());
    REQUIRE(club.getFacilities().empty());
    REQUIRE(club.getLoyaltyProgram().getPointsPerVisit() == 10);
    REQUIRE(club.getFinance().getBalance() == Catch::Approx(1000.0));

    REQUIRE_THROWS_AS(club.registerMember(nullptr), InvalidDataException);
    REQUIRE_THROWS_AS(club.hireEmployee(nullptr), InvalidDataException);
    REQUIRE_THROWS_AS(club.addFacility(nullptr), InvalidDataException);

    auto memPtr = std::make_unique<Member>("M1", "Ivan", "P", birth, "+375291112233", "i@b.c");
    memPtr->deposit(500.0);
    club.registerMember(std::move(memPtr));
    club.addPlan(plan);
    REQUIRE(club.getPlans().size() == 1);
    REQUIRE_THROWS_AS(club.findPlan("NONE"), SportsClubException);
    club.sellMembership("M1", "Basic", today);
    REQUIRE(club.countActiveMembers(today) == 1);

    auto empPtr = std::make_unique<Coach>("E1", "Alex", "S", birth, "+375291112233", "c@b.c", 1000.0, today, "Fit", 5);
    club.hireEmployee(std::move(empPtr));
    REQUIRE(club.findEmployee("E1").isActive());
    club.fireEmployee("E1");
    REQUIRE_FALSE(club.findEmployee("E1").isActive());
    REQUIRE_THROWS_AS(club.findEmployee("NONE"), EmployeeNotFoundException);

    auto gymPtr = std::make_unique<Gym>("G1", "MainGym", 20, AccessLevel::Basic, 150.0);
    gymPtr->addEquipment(std::make_unique<Treadmill>("TR1", "Run", today, 50.0, 20.0, 120.0));
    Gym& gymRef = static_cast<Gym&>(club.addFacility(std::move(gymPtr)));
    REQUIRE(club.findFacility("G1").getName() == "MainGym");
    REQUIRE_THROWS_AS(club.findFacility("NONE"), FacilityNotFoundException);

    club.getSchedule().addGroupClass(gc);
    ReportGenerator rep("Olympus");
    REQUIRE(rep.getClubName() == "Olympus");
    const SportsClub& constClub = club;
    REQUIRE_FALSE(rep.buildMembersReport(constClub, today).empty());
    REQUIRE_FALSE(rep.buildFinanceReport(constClub.getFinance(), today, today).empty());
    REQUIRE_FALSE(rep.buildEquipmentReport(gymRef).empty());
    REQUIRE_FALSE(rep.buildScheduleReport(club.getSchedule(), today).empty());
    REQUIRE(rep.getGeneratedCount() == 4);

    club.removeMember("M1");
    REQUIRE_THROWS_AS(club.findMember("M1"), MemberNotFoundException);
    REQUIRE_THROWS_AS(club.removeMember("M1"), MemberNotFoundException);
}
