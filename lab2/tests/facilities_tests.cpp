#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "exceptions/AccessDeniedException.h"
#include "exceptions/BookingConflictException.h"
#include "exceptions/EquipmentBrokenException.h"
#include "exceptions/FacilityFullException.h"
#include "exceptions/InvalidDataException.h"
#include "exceptions/SportsClubException.h"
#include "facilities/ExerciseBike.h"
#include "facilities/Gym.h"
#include "facilities/Locker.h"
#include "facilities/LockerRoom.h"
#include "facilities/MaintenanceRecord.h"
#include "facilities/Sauna.h"
#include "facilities/SwimmingPool.h"
#include "facilities/TennisCourt.h"
#include "facilities/Treadmill.h"
#include "members/Member.h"
#include "members/MembershipPlan.h"

TEST_CASE("Equipment, Treadmill, ExerciseBike and MaintenanceRecord", "[facilities][Equipment]") {
    Date purchase(1, 1, 2024);
    Date today(1, 6, 2026);

    REQUIRE_THROWS_AS(Treadmill("", "Run", purchase, 100.0, 20.0, 150.0), InvalidDataException);
    REQUIRE_THROWS_AS(Treadmill("TR1", "Run", purchase, 100.0, 0.0, 150.0), InvalidDataException);
    Treadmill tm("TR1", "ProRun", purchase, 50.0, 20.0, 130.0);

    REQUIRE(tm.getInventoryNumber() == "TR1");
    REQUIRE(tm.getName() == "ProRun");
    REQUIRE(tm.getPurchaseDate() == purchase);
    REQUIRE(tm.getCondition() == EquipmentCondition::New);
    REQUIRE(tm.getCategory() == "Treadmill");
    REQUIRE(tm.getMaxSpeedKmh() == Catch::Approx(20.0));
    REQUIRE(tm.getMaxUserWeightKg() == Catch::Approx(130.0));
    REQUIRE(tm.getAgeYears(today) == 2);
    REQUIRE(tm.isSuitableFor(100.0));
    REQUIRE_FALSE(tm.isSuitableFor(140.0));

    REQUIRE_THROWS_AS(tm.use(-1.0), InvalidDataException);
    tm.use(10.0);
    REQUIRE(tm.getCondition() == EquipmentCondition::Good);
    REQUIRE(tm.getUsageHours() == Catch::Approx(10.0));
    REQUIRE(tm.getHoursSinceMaintenance() == Catch::Approx(10.0));
    tm.use(45.0);
    REQUIRE(tm.getCondition() == EquipmentCondition::Worn);
    REQUIRE(tm.needsMaintenance());

    REQUIRE_THROWS_AS(tm.setSpeed(-1.0), InvalidDataException);
    REQUIRE_THROWS_AS(tm.setSpeed(25.0), InvalidDataException);
    tm.setSpeed(12.5);
    REQUIRE(tm.getCurrentSpeedKmh() == Catch::Approx(12.5));
    tm.stop();
    REQUIRE(tm.getCurrentSpeedKmh() == Catch::Approx(0.0));

    tm.markBroken();
    REQUIRE_THROWS_AS(tm.use(1.0), EquipmentBrokenException);
    REQUIRE_THROWS_AS(tm.setSpeed(5.0), EquipmentBrokenException);
    tm.repair();
    REQUIRE_FALSE(tm.needsMaintenance());

    REQUIRE_THROWS_AS(ExerciseBike("BK1", "Bike", purchase, 50.0, 0), InvalidDataException);
    ExerciseBike bike("BK1", "Bike", purchase, 50.0, 3);
    REQUIRE(bike.getCategory() == "ExerciseBike");
    REQUIRE(bike.getResistanceLevels() == 3);
    REQUIRE(bike.getCurrentLevel() == 1);
    bike.decreaseResistance();
    REQUIRE(bike.getCurrentLevel() == 1);
    bike.increaseResistance();
    bike.increaseResistance();
    bike.increaseResistance();
    REQUIRE(bike.getCurrentLevel() == 3);
    REQUIRE(bike.estimateCaloriesPerMinute(0.0) == Catch::Approx(0.0));
    REQUIRE(bike.estimateCaloriesPerMinute(70.0) > 0.0);

    REQUIRE_THROWS_AS(MaintenanceRecord(&tm, "T1", today, -5.0, "Bad"), InvalidDataException);
    MaintenanceRecord rec(&tm, "T1", today, 40.0, "Oil");
    MaintenanceRecord recNull(nullptr, "T1", today, 10.0, "Check");
    REQUIRE(rec.getEquipment() == &tm);
    REQUIRE(rec.getTechnicianId() == "T1");
    REQUIRE(rec.getDate() == today);
    REQUIRE(rec.getDescription() == "Oil");
    REQUIRE(rec.isWithinPeriod(purchase, today));
    REQUIRE_FALSE(rec.toString().empty());
    REQUIRE_FALSE(recNull.toString().empty());
}

TEST_CASE("Facility, Gym, SwimmingPool, TennisCourt, Sauna, Locker and LockerRoom", "[facilities]") {
    Date today(1, 6, 2026);
    REQUIRE_THROWS_AS(Gym("", "Gym", 10, AccessLevel::Basic, 100.0), InvalidDataException);
    REQUIRE_THROWS_AS(Gym("G1", "Gym", 10, AccessLevel::Basic, 0.0), InvalidDataException);

    Gym gym("G1", "Main Gym", 1, AccessLevel::Basic, 200.0);
    REQUIRE(gym.getType() == FacilityType::Gym);
    REQUIRE(gym.getAreaSquareMeters() == Catch::Approx(200.0));
    REQUIRE(gym.getRequiredAccessLevel() == AccessLevel::Basic);
    REQUIRE(gym.getLoadPercent() == Catch::Approx(0.0));

    REQUIRE_THROWS_AS(gym.addEquipment(nullptr), InvalidDataException);
    gym.addEquipment(std::make_unique<Treadmill>("TR1", "Run", today, 50.0, 20.0, 120.0));
    REQUIRE(gym.findEquipment("TR1").getName() == "Run");
    REQUIRE_THROWS_AS(gym.findEquipment("NONE"), SportsClubException);
    REQUIRE(gym.countWorkingEquipment() == 1);
    gym.findEquipment("TR1").markBroken();
    REQUIRE(gym.getEquipmentNeedingMaintenance().size() == 1);
    gym.removeEquipment("TR1");
    REQUIRE(gym.getEquipment().empty());

    Member m1("M1", "Ivan", "P", Date(1, 1, 2000), "+375291112233", "i@b.c");
    MembershipPlan plan("Basic", 100.0, 1, AccessLevel::Basic, 0, false, false);
    m1.deposit(200.0);
    m1.purchaseMembership(plan, today);

    gym.close();
    REQUIRE_THROWS_AS(gym.enter(m1, today), AccessDeniedException);
    REQUIRE_THROWS_AS(gym.enterGuest(), AccessDeniedException);
    gym.open();
    gym.enter(m1, today);
    REQUIRE(gym.isInside("M1"));
    REQUIRE(gym.getLoadPercent() == Catch::Approx(100.0));
    REQUIRE_THROWS_AS(gym.enterGuest(), FacilityFullException);
    REQUIRE_THROWS_AS(gym.enter(m1, today), FacilityFullException);
    gym.leave("UNKNOWN");
    REQUIRE(gym.getOccupancy() == 0);

    REQUIRE_THROWS_AS(SwimmingPool("P1", "Pool", 10, AccessLevel::Basic, 0, 2.0, 27.0), InvalidDataException);
    SwimmingPool pool("P1", "Pool", 10, AccessLevel::Basic, 1, 2.0, 27.0);
    REQUIRE(pool.getType() == FacilityType::SwimmingPool);
    REQUIRE(pool.getLaneCount() == 1);
    REQUIRE(pool.getDepthMeters() == Catch::Approx(2.0));
    REQUIRE(pool.isTemperatureSafe());
    pool.setWaterTemperature(20.0);
    REQUIRE(pool.getWaterTemperature() == Catch::Approx(20.0));
    REQUIRE_FALSE(pool.isTemperatureSafe());
    pool.reserveLane();
    REQUIRE(pool.getReservedLanes() == 1);
    REQUIRE_THROWS_AS(pool.reserveLane(), BookingConflictException);
    pool.releaseLane();
    REQUIRE(pool.getFreeLanes() == 1);

    REQUIRE_THROWS_AS(TennisCourt("C1", "Court", AccessLevel::Basic, CourtSurface::Clay, false, -1.0), InvalidDataException);
    TennisCourt court("C1", "Court", AccessLevel::Basic, CourtSurface::Clay, false, 50.0);
    REQUIRE(court.getType() == FacilityType::TennisCourt);
    REQUIRE(court.getSurface() == CourtSurface::Clay);
    REQUIRE_FALSE(court.isIndoor());
    REQUIRE_THROWS_AS(court.setHourlyRate(-5.0), InvalidDataException);
    court.setHourlyRate(40.0);
    REQUIRE(court.calculateRentCost(0.0, true) == Catch::Approx(0.0));
    REQUIRE(court.calculateRentCost(2.0, false) == Catch::Approx(80.0));
    REQUIRE(court.calculateRentCost(2.0, true) == Catch::Approx(64.0));
    REQUIRE_FALSE(court.isPlayableInWeather(true));
    REQUIRE(court.isPlayableInWeather(false));

    REQUIRE_THROWS_AS(Sauna("S1", "Sauna", 5, AccessLevel::Basic, 80.0, 0), InvalidDataException);
    Sauna sauna("S1", "Sauna", 5, AccessLevel::Basic, 85.0, 30);
    REQUIRE(sauna.getType() == FacilityType::Sauna);
    REQUIRE(sauna.getMaxSessionMinutes() == 30);
    REQUIRE(sauna.isTemperatureSafe());
    sauna.setTemperature(120.0);
    REQUIRE(sauna.getTemperature() == Catch::Approx(120.0));
    REQUIRE_FALSE(sauna.isTemperatureSafe());
    REQUIRE(sauna.isSessionAllowed(20));
    REQUIRE_FALSE(sauna.isSessionAllowed(40));

    REQUIRE_THROWS_AS(LockerRoom("LR1", "LR", 10, AccessLevel::Basic, 0), InvalidDataException);
    LockerRoom lr("LR1", "LR", 10, AccessLevel::Basic, 1);
    REQUIRE(lr.getType() == FacilityType::LockerRoom);
    REQUIRE(lr.getLockers().size() == 1);
    int num = lr.assignLocker("M1", "1111");
    REQUIRE(num == 1);
    REQUIRE(lr.countFreeLockers() == 0);
    REQUIRE_THROWS_AS(lr.assignLocker("M2", "2222"), FacilityFullException);
    Locker& lk = lr.findLockerByOwner("M1");
    REQUIRE(lk.unlock("1111"));
    REQUIRE_THROWS_AS(lk.occupy("M2", "2222"), AccessDeniedException);
    lr.releaseLocker(1);
    REQUIRE_THROWS_AS(lr.findLockerByOwner("M1"), SportsClubException);
}
