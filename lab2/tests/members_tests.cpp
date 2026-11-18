#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "exceptions/AccessDeniedException.h"
#include "exceptions/InsufficientFundsException.h"
#include "exceptions/InvalidDataException.h"
#include "exceptions/InvalidDateException.h"
#include "exceptions/InvalidPinException.h"
#include "exceptions/MedicalClearanceException.h"
#include "exceptions/MembershipExpiredException.h"
#include "exceptions/MembershipFrozenException.h"
#include "facilities/Gym.h"
#include "facilities/Sauna.h"
#include "facilities/SwimmingPool.h"
#include "facilities/TennisCourt.h"
#include "finance/Discount.h"
#include "finance/Invoice.h"
#include "finance/InvoiceItem.h"
#include "members/AccessCard.h"
#include "members/BodyMetrics.h"
#include "members/GuestPass.h"
#include "members/HealthRecord.h"
#include "members/LoyaltyProgram.h"
#include "members/MedicalCertificate.h"
#include "members/Member.h"
#include "members/Membership.h"
#include "members/MembershipPlan.h"
#include "members/ProgressTracker.h"
#include "members/Visit.h"

TEST_CASE("AccessCard PIN verification, blocking and changePin", "[members][AccessCard]") {
    Date issueDate(1, 1, 2026);
    REQUIRE_THROWS_AS(AccessCard("", "M1", issueDate, "1234"), InvalidDataException);
    REQUIRE_THROWS_AS(AccessCard("C1", "M1", issueDate, "123"), InvalidPinException);
    REQUIRE_THROWS_AS(AccessCard("C1", "M1", issueDate, "12a4"), InvalidPinException);

    AccessCard card("C1", "M1", issueDate, "1234");
    REQUIRE(card.getCardNumber() == "C1");
    REQUIRE(card.getOwnerId() == "M1");
    REQUIRE(card.getIssueDate() == issueDate);
    REQUIRE_FALSE(card.isBlocked());
    REQUIRE(card.getFailedAttempts() == 0);

    REQUIRE(card.verifyPin("1234"));
    REQUIRE_FALSE(card.verifyPin("0000"));
    REQUIRE_FALSE(card.verifyPin("0000"));
    REQUIRE_FALSE(card.verifyPin("0000"));
    REQUIRE(card.isBlocked());
    REQUIRE_FALSE(card.verifyPin("1234"));
    REQUIRE_THROWS_AS(card.changePin("1234", "5678"), AccessDeniedException);

    card.unblock();
    REQUIRE_FALSE(card.isBlocked());
    REQUIRE_THROWS_AS(card.changePin("1234", "bad"), InvalidPinException);
    card.changePin("1234", "5678");
    REQUIRE(card.verifyPin("5678"));

    REQUIRE_THROWS_AS(card.changePin("0000", "9999"), InvalidPinException);
    REQUIRE_THROWS_AS(card.changePin("0000", "9999"), InvalidPinException);
    REQUIRE_THROWS_AS(card.changePin("0000", "9999"), InvalidPinException);
    REQUIRE(card.isBlocked());
    card.unblock();
    card.block();
    REQUIRE(card.isBlocked());
}

TEST_CASE("BodyMetrics and ProgressTracker calculations", "[members][BodyMetrics][ProgressTracker]") {
    Date d1(1, 1, 2026);
    REQUIRE_THROWS_AS(BodyMetrics(d1, 0.0, 180.0, 15.0), InvalidDataException);
    REQUIRE_THROWS_AS(BodyMetrics(d1, 80.0, 180.0, 105.0), InvalidDataException);

    BodyMetrics underweight(d1, 50.0, 180.0, 10.0);
    BodyMetrics normal(d1, 70.0, 180.0, 15.0);
    BodyMetrics overweight(d1, 85.0, 180.0, 20.0);
    BodyMetrics obese(d1, 105.0, 180.0, 30.0);

    REQUIRE(normal.getDate() == d1);
    REQUIRE(normal.getWeightKg() == Catch::Approx(70.0));
    REQUIRE(normal.getHeightCm() == Catch::Approx(180.0));
    REQUIRE(normal.getBodyFatPercent() == Catch::Approx(15.0));
    REQUIRE(underweight.getBmiCategory() == "Underweight");
    REQUIRE(normal.getBmiCategory() == "Normal");
    REQUIRE(overweight.getBmiCategory() == "Overweight");
    REQUIRE(obese.getBmiCategory() == "Obese");
    REQUIRE(overweight.calculateLeanMassKg() == Catch::Approx(68.0));

    ProgressTracker emptyTracker;
    REQUIRE_THROWS_AS(emptyTracker.getLatest(), InvalidDataException);
    REQUIRE(emptyTracker.getWeightChange() == Catch::Approx(0.0));
    REQUIRE_FALSE(emptyTracker.isGoalReached());
    REQUIRE_THROWS_AS(ProgressTracker(-5.0), InvalidDataException);
    REQUIRE_THROWS_AS(emptyTracker.setTargetWeight(-1.0), InvalidDataException);

    ProgressTracker lossTracker(75.0);
    REQUIRE(lossTracker.getTargetWeight() == Catch::Approx(75.0));
    lossTracker.addMeasurement(overweight);
    lossTracker.addMeasurement(normal);
    REQUIRE(lossTracker.getHistory().size() == 2);
    REQUIRE(lossTracker.getLatest().getWeightKg() == Catch::Approx(70.0));
    REQUIRE(lossTracker.getWeightChange() == Catch::Approx(-15.0));
    REQUIRE(lossTracker.isGoalReached());

    lossTracker.setTargetWeight(90.0);
    ProgressTracker gainTracker(75.0);
    gainTracker.addMeasurement(normal);
    gainTracker.addMeasurement(overweight);
    REQUIRE(gainTracker.isGoalReached());
}

TEST_CASE("GuestPass, MedicalCertificate and HealthRecord", "[members][HealthRecord]") {
    Date d1(10, 5, 2026);
    Date d2(20, 5, 2026);
    REQUIRE_THROWS_AS(GuestPass("", "Guest", d1, 10.0), InvalidDataException);
    GuestPass pass("GP-1", "Alex", d1, 15.0);
    REQUIRE(pass.getCode() == "GP-1");
    REQUIRE(pass.getGuestName() == "Alex");
    REQUIRE(pass.getValidDate() == d1);
    REQUIRE(pass.getPrice() == Catch::Approx(15.0));
    REQUIRE_FALSE(pass.isUsed());
    REQUIRE(pass.isValidOn(d1));
    REQUIRE_THROWS_AS(pass.use(d2), InvalidDateException);
    pass.use(d1);
    REQUIRE(pass.isUsed());
    REQUIRE_THROWS_AS(pass.use(d1), AccessDeniedException);

    REQUIRE_THROWS_AS(MedicalCertificate("", d1, d2, "Doc", true), InvalidDataException);
    REQUIRE_THROWS_AS(MedicalCertificate("MC1", d2, d1, "Doc", true), InvalidDateException);
    MedicalCertificate lowCert("MC1", d1, d2, "Dr. Smith", false);
    MedicalCertificate highCert("MC2", Date(12, 5, 2026), d2, "Dr. Jones", true);
    REQUIRE(lowCert.getNumber() == "MC1");
    REQUIRE(lowCert.getIssueDate() == d1);
    REQUIRE(lowCert.getValidUntil() == d2);
    REQUIRE(lowCert.getDoctorName() == "Dr. Smith");
    REQUIRE_FALSE(lowCert.allowsHighIntensity());
    REQUIRE(lowCert.daysUntilExpiry(d1) == 10);
    REQUIRE_FALSE(lowCert.permits(Intensity::High, d1));
    REQUIRE(lowCert.permits(Intensity::Medium, d1));
    REQUIRE_FALSE(lowCert.permits(Intensity::Low, Date(25, 5, 2026)));

    HealthRecord record;
    REQUIRE_THROWS_AS(record.getLatestCertificate(), MedicalClearanceException);
    REQUIRE_FALSE(record.hasValidCertificate(d1));
    REQUIRE_FALSE(record.canPerform(Intensity::Low, d1));

    record.addContraindication("Back pain");
    record.addContraindication("Back pain");
    REQUIRE(record.hasContraindication("Back pain"));
    REQUIRE(record.getContraindications().size() == 1);

    record.addCertificate(lowCert);
    record.addCertificate(highCert);
    REQUIRE(record.hasValidCertificate(Date(15, 5, 2026)));
    REQUIRE(record.canPerform(Intensity::High, Date(15, 5, 2026)));
    REQUIRE(record.getLatestCertificate().getNumber() == "MC2");
}

TEST_CASE("LoyaltyProgram, MembershipPlan, Membership, Visit and Member", "[members][Member]") {
    REQUIRE_THROWS_AS(LoyaltyProgram(-1, 1.0, 0.5), InvalidDataException);
    LoyaltyProgram loyalty(10, 2.0, 0.5);
    REQUIRE(loyalty.getPointsPerVisit() == 10);
    REQUIRE(loyalty.getPointsPerCurrencyUnit() == Catch::Approx(2.0));
    REQUIRE(loyalty.getPointValue() == Catch::Approx(0.5));
    REQUIRE(loyalty.calculateVisitPoints(3) == 30);
    REQUIRE(loyalty.calculateVisitPoints(-1) == 0);
    REQUIRE(loyalty.calculatePurchasePoints(50.0) == 100);
    REQUIRE(loyalty.calculatePurchasePoints(-5.0) == 0);
    REQUIRE(loyalty.convertPointsToMoney(100) == Catch::Approx(50.0));
    REQUIRE(loyalty.convertPointsToMoney(0) == Catch::Approx(0.0));
    REQUIRE(loyalty.getTier(100) == LoyaltyTier::Bronze);
    REQUIRE(loyalty.getTier(500) == LoyaltyTier::Silver);
    REQUIRE(loyalty.getTier(1500) == LoyaltyTier::Gold);
    REQUIRE(loyalty.getTier(5000) == LoyaltyTier::Platinum);
    REQUIRE(loyalty.getTierDiscountPercent(LoyaltyTier::Bronze) == Catch::Approx(0.0));
    REQUIRE(loyalty.getTierDiscountPercent(LoyaltyTier::Silver) == Catch::Approx(5.0));
    REQUIRE(loyalty.getTierDiscountPercent(LoyaltyTier::Gold) == Catch::Approx(10.0));
    REQUIRE(loyalty.getTierDiscountPercent(LoyaltyTier::Platinum) == Catch::Approx(15.0));

    Date today(1, 6, 2026);
    REQUIRE_THROWS_AS(MembershipPlan("", 100.0, 1, AccessLevel::Basic, 2, false, false), InvalidDataException);
    MembershipPlan limitedPlan("Basic", 100.0, 1, AccessLevel::Basic, 1, false, false);
    MembershipPlan premiumPlan("Premium", 200.0, 2, AccessLevel::Premium, 0, true, true);
    Discount disc("PROMO10", 10.0, Date(30, 6, 2026), 5);
    REQUIRE(limitedPlan.getName() == "Basic");
    REQUIRE(limitedPlan.getMonthlyPrice() == Catch::Approx(100.0));
    REQUIRE(limitedPlan.getDurationMonths() == 1);
    REQUIRE(limitedPlan.getAccessLevel() == AccessLevel::Basic);
    REQUIRE(limitedPlan.getMaxVisitsPerWeek() == 1);
    REQUIRE(limitedPlan.calculateTotalPrice(disc, today) == Catch::Approx(90.0));
    REQUIRE(limitedPlan.includesFacility(FacilityType::Gym));
    REQUIRE_FALSE(limitedPlan.includesFacility(FacilityType::SwimmingPool));
    REQUIRE_FALSE(limitedPlan.includesFacility(FacilityType::Sauna));
    REQUIRE_FALSE(limitedPlan.includesFacility(FacilityType::TennisCourt));
    REQUIRE(premiumPlan.includesFacility(FacilityType::TennisCourt));

    Membership mship(limitedPlan, today);
    REQUIRE(mship.getPlan().getName() == "Basic");
    REQUIRE(mship.getStartDate() == today);
    REQUIRE(mship.getEndDate() == today.addDays(30));
    REQUIRE(mship.daysRemaining(today) == 30);
    REQUIRE(mship.daysRemaining(today.addDays(40)) == 0);
    mship.extend(5);
    mship.registerVisit(today);
    REQUIRE(mship.getVisitsThisWeek() == 1);
    REQUIRE_FALSE(mship.canVisit(today));
    REQUIRE_THROWS_AS(mship.registerVisit(today), AccessDeniedException);
    mship.resetWeeklyVisits();
    REQUIRE(mship.canVisit(today));

    REQUIRE_THROWS_AS(mship.registerVisit(today.addDays(100)), MembershipExpiredException);
    REQUIRE_THROWS_AS(mship.freeze(today.addDays(100)), MembershipExpiredException);
    mship.freeze(today);
    REQUIRE(mship.isFrozen());
    REQUIRE_FALSE(mship.canVisit(today));
    REQUIRE_THROWS_AS(mship.freeze(today), MembershipFrozenException);
    REQUIRE_THROWS_AS(mship.registerVisit(today), MembershipFrozenException);
    mship.unfreeze(today.addDays(5));
    mship.unfreeze(today.addDays(5));
    REQUIRE_FALSE(mship.isFrozen());

    REQUIRE_THROWS_AS(Visit("", nullptr, today, 100), InvalidDataException);
    Visit visit("M1", nullptr, today, 600);
    REQUIRE(visit.getMemberId() == "M1");
    REQUIRE(visit.getFacility() == nullptr);
    REQUIRE(visit.getEntryMinute() == 600);
    REQUIRE(visit.isOpen());
    REQUIRE(visit.getDurationMinutes() == 0);
    REQUIRE_THROWS_AS(visit.close(500), InvalidDataException);
    visit.close(660);
    REQUIRE(visit.getExitMinute() == 660);
    REQUIRE(visit.getDurationMinutes() == 60);

    REQUIRE_THROWS_AS(Member("", "A", "B", Date(1, 1, 2000), "+375291234567", "a@b.c"), InvalidDataException);
    Member member("M1", "Ivan", "Petrov", Date(1, 1, 2000), "+375291234567", "a@b.c");
    Gym gym("G1", "Gym", 10, AccessLevel::Basic, 100.0);
    SwimmingPool pool("P1", "Pool", 10, AccessLevel::Basic, 4, 2.0, 27.0);
    TennisCourt vipCourt("C1", "Court", AccessLevel::Vip, CourtSurface::Hard, true, 30.0);

    REQUIRE_FALSE(member.canAccessFacility(gym, today));
    REQUIRE_THROWS_AS(member.deposit(0.0), InvalidDataException);
    REQUIRE_THROWS_AS(member.withdraw(0.0), InvalidDataException);
    REQUIRE_THROWS_AS(member.withdraw(50.0), InsufficientFundsException);

    member.deposit(500.0);
    member.purchaseMembership(limitedPlan, today);
    REQUIRE(member.getMembership() != nullptr);
    REQUIRE(member.canAccessFacility(gym, today));
    REQUIRE_FALSE(member.canAccessFacility(pool, today));
    REQUIRE_FALSE(member.canAccessFacility(vipCourt, today));

    member.registerVisit(visit, loyalty);
    REQUIRE_FALSE(member.canAccessFacility(gym, today));
    REQUIRE(member.getVisits().size() == 1);
    REQUIRE(member.countVisitsBetween(today, today) == 1);
    REQUIRE(member.getBonusPoints() == 10);
    REQUIRE_THROWS_AS(member.redeemBonusPoints(100, loyalty), InvalidDataException);
    REQUIRE(member.redeemBonusPoints(10, loyalty) == Catch::Approx(5.0));

    Invoice inv("INV-1", &member, today, today.addDays(7));
    inv.addItem(InvoiceItem("PT", 1, 50.0));
    member.payInvoice(inv, today);
    REQUIRE(inv.isPaid());
    REQUIRE(member.getProgressTracker().getTargetWeight() == Catch::Approx(0.0));
}
