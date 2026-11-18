#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "exceptions/AccessDeniedException.h"
#include "exceptions/BookingConflictException.h"
#include "exceptions/ClassFullException.h"
#include "exceptions/CoachUnavailableException.h"
#include "exceptions/InvalidDataException.h"
#include "exceptions/MedicalClearanceException.h"
#include "exceptions/SportsClubException.h"
#include "facilities/Gym.h"
#include "facilities/TennisCourt.h"
#include "finance/FinanceDepartment.h"
#include "members/MedicalCertificate.h"
#include "members/Member.h"
#include "members/MembershipPlan.h"
#include "staff/Coach.h"
#include "training/Booking.h"
#include "training/BookingSystem.h"
#include "training/Exercise.h"
#include "training/GroupClass.h"
#include "training/Match.h"
#include "training/Schedule.h"
#include "training/Team.h"
#include "training/Tournament.h"
#include "training/TrainingProgram.h"
#include "training/TrainingSession.h"
#include "training/Workout.h"

TEST_CASE("Exercise, Workout, TrainingProgram and TrainingSession", "[training][TrainingProgram]") {
    Date today(1, 6, 2026);
    REQUIRE_THROWS_AS(Exercise("", "Chest", 3, 10, 50.0), InvalidDataException);
    Exercise ex("Bench Press", "Chest", 4, 10, 60.0);
    REQUIRE(ex.getName() == "Bench Press");
    REQUIRE(ex.getMuscleGroup() == "Chest");
    REQUIRE(ex.getSets() == 4);
    REQUIRE(ex.getReps() == 10);
    REQUIRE(ex.getWeightKg() == Catch::Approx(60.0));
    REQUIRE(ex.calculateVolume() == Catch::Approx(2400.0));
    REQUIRE(ex.estimateDurationMinutes() == 6);
    REQUIRE_THROWS_AS(ex.increaseWeight(-100.0), InvalidDataException);
    ex.increaseWeight(10.0);
    REQUIRE_THROWS_AS(ex.scale(0.0), InvalidDataException);
    ex.scale(2.0);
    REQUIRE(ex.getWeightKg() == Catch::Approx(140.0));

    REQUIRE_THROWS_AS(Workout("", Intensity::Low, 60, 0.1), InvalidDataException);
    Workout w("HIIT", Intensity::High, 45, 0.15);
    REQUIRE(w.getName() == "HIIT");
    REQUIRE(w.getIntensity() == Intensity::High);
    REQUIRE(w.getDurationMinutes() == 45);
    REQUIRE(w.getCaloriesPerMinutePerKg() == Catch::Approx(0.15));
    REQUIRE(w.estimateCalories(0.0) == Catch::Approx(0.0));
    REQUIRE(w.estimateCalories(80.0) > 0.0);
    REQUIRE(w.requiresHighIntensityClearance());

    REQUIRE_THROWS_AS(TrainingProgram("", "Goal", 4, nullptr), InvalidDataException);
    TrainingProgram prog("Strength", "Gain", 8, nullptr);
    REQUIRE(prog.getName() == "Strength");
    REQUIRE(prog.getGoal() == "Gain");
    REQUIRE(prog.getWeeks() == 8);
    prog.addExercise(ex);
    REQUIRE(prog.getExercises().size() == 1);
    REQUIRE(prog.calculateTotalVolume() > 0.0);
    REQUIRE(prog.estimateSessionDurationMinutes() == 6);
    prog.progressAll(5.0);
    prog.removeExercise("Bench Press");
    REQUIRE(prog.getExercises().empty());

    TimeSlot slot(today, 600, 60);
    REQUIRE_THROWS_AS(TrainingSession(nullptr, nullptr, nullptr, slot, &prog, -10.0), InvalidDataException);
    TrainingSession ts(nullptr, nullptr, nullptr, slot, &prog, 50.0);
    REQUIRE(ts.getCoach() == nullptr);
    REQUIRE(ts.getMember() == nullptr);
    REQUIRE(ts.getFacility() == nullptr);
    REQUIRE(ts.getProgram() == &prog);
    REQUIRE(ts.calculateCost() == Catch::Approx(50.0));
    ts.start();
    REQUIRE(ts.getStatus() == SessionStatus::InProgress);
    ts.complete();
    REQUIRE(ts.getStatus() == SessionStatus::Completed);
    ts.cancel();
    REQUIRE(ts.getStatus() == SessionStatus::Completed);

    TrainingSession ts2(nullptr, nullptr, nullptr, slot, &prog, 50.0);
    ts2.cancel();
    REQUIRE(ts2.getStatus() == SessionStatus::Cancelled);
}

TEST_CASE("GroupClass, Schedule, Booking and BookingSystem", "[training][Schedule][Booking]") {
    Date today(1, 6, 2026);
    Date birth(1, 1, 1995);
    Workout w("Yoga", Intensity::Low, 60, 0.08);
    Coach c1("C1", "Alex", "S", birth, "+375291112233", "c1@b.c", 1500.0, today, "Yoga", 10);
    Coach c2("C2", "Oleg", "K", birth, "+375291112234", "c2@b.c", 1500.0, today, "Box", 10);
    Gym g1("G1", "Gym1", 20, AccessLevel::Basic, 100.0);
    Gym g2("G2", "Gym2", 20, AccessLevel::Basic, 100.0);
    TimeSlot slot(today, 600, 60);

    REQUIRE_THROWS_AS(GroupClass("", w, &c1, &g1, slot, 10), InvalidDataException);
    GroupClass gc("Morning Yoga", w, &c1, &g1, slot, 2);
    REQUIRE(gc.getWorkout().getName() == "Yoga");
    REQUIRE(gc.getCoach() == &c1);
    REQUIRE(gc.getFacility() == &g1);
    REQUIRE(gc.getMaxParticipants() == 2);

    Member m1("M1", "Ivan", "P", birth, "+375291112233", "m1@b.c");
    Member m2("M2", "Petr", "S", birth, "+375291112234", "m2@b.c");
    Member m3("M3", "Oleg", "K", birth, "+375291112235", "m3@b.c");
    REQUIRE_THROWS_AS(gc.enroll(m1, today), AccessDeniedException);

    MembershipPlan plan("Basic", 100.0, 1, AccessLevel::Basic, 0, false, false);
    m1.deposit(200.0);
    m1.purchaseMembership(plan, today);
    m2.deposit(200.0);
    m2.purchaseMembership(plan, today);
    m2.getHealthRecord().addCertificate(MedicalCertificate("MC2", today, today.addDays(30), "Doc", true));
    REQUIRE_THROWS_AS(gc.enroll(m1, today), MedicalClearanceException);

    m1.getHealthRecord().addCertificate(MedicalCertificate("MC1", today, today.addDays(30), "Doc", true));
    gc.enroll(m1, today);
    gc.enroll(m1, today);
    gc.enroll(m2, today);
    REQUIRE(gc.isFull());
    REQUIRE(gc.getFreeSpots() == 0);
    REQUIRE(gc.getParticipants().size() == 2);
    REQUIRE_THROWS_AS(gc.enroll(m3, today), ClassFullException);
    gc.unenroll("M1");
    REQUIRE(gc.getFreeSpots() == 1);
    gc.cancel();
    REQUIRE(gc.isCancelled());
    REQUIRE_THROWS_AS(gc.enroll(m1, today), AccessDeniedException);

    GroupClass gcActive("Box", w, &c1, &g1, slot, 5);
    Schedule sched;
    sched.addGroupClass(gcActive);
    REQUIRE(sched.getGroupClasses().size() == 1);

    GroupClass conflictFac("C_Fac", w, &c2, &g1, slot, 5);
    GroupClass conflictCoach("C_Coach", w, &c1, &g2, slot, 5);
    REQUIRE_THROWS_AS(sched.addGroupClass(conflictFac), BookingConflictException);
    REQUIRE_THROWS_AS(sched.addGroupClass(conflictCoach), CoachUnavailableException);

    TimeSlot slot2(today, 720, 60);
    TrainingSession sess(&c1, &m1, &g1, slot2, nullptr, 40.0);
    sched.addSession(sess);
    REQUIRE(sched.getSessions().size() == 1);
    TrainingSession sessConflictFac(&c2, &m1, &g1, slot2, nullptr, 40.0);
    TrainingSession sessConflictCoach(&c1, &m1, &g2, slot2, nullptr, 40.0);
    REQUIRE_THROWS_AS(sched.addSession(sessConflictFac), BookingConflictException);
    REQUIRE_THROWS_AS(sched.addSession(sessConflictCoach), CoachUnavailableException);

    REQUIRE(sched.getClassesOn(today).size() == 1);
    REQUIRE(sched.getCoachLoadMinutes(c1, today) == 120);
    sched.removeGroupClass(gcActive);
    REQUIRE(sched.getGroupClasses().empty());

    REQUIRE_THROWS_AS(Booking(1, &m1, &g1, slot, -5.0), InvalidDataException);
    TennisCourt court("TC1", "Court", AccessLevel::Basic, CourtSurface::Hard, true, 50.0);
    BookingSystem bsys;
    Booking& b1 = bsys.bookCourt(m1, court, slot);
    REQUIRE(b1.getStatus() == BookingStatus::Confirmed);
    REQUIRE(bsys.getBookings().size() == 1);
    REQUIRE(bsys.getBookingsForMember("M1").size() == 1);
    REQUIRE_THROWS_AS(bsys.bookCourt(m1, court, slot), BookingConflictException);
    bsys.cancelBooking(b1.getId());
    REQUIRE(b1.getStatus() == BookingStatus::Cancelled);
    REQUIRE_THROWS_AS(bsys.findBooking(999), SportsClubException);
}

TEST_CASE("Team, Match and Tournament", "[training][Tournament]") {
    Date today(1, 6, 2026);
    Date birth(1, 1, 1995);
    REQUIRE_THROWS_AS(Team("", "Football", nullptr, 5), InvalidDataException);
    Team t1("Titans", "Football", nullptr, 1);
    Team t2("Spartans", "Football", nullptr, 2);
    Team t3("Olympians", "Football", nullptr, 2);

    REQUIRE(t1.getName() == "Titans");
    REQUIRE(t1.getSport() == "Football");
    REQUIRE(t1.getCoach() == nullptr);
    REQUIRE(t1.calculateWinRate() == Catch::Approx(0.0));

    Member m1("M1", "Ivan", "P", birth, "+375291112233", "m1@b.c");
    Member m2("M2", "Petr", "S", birth, "+375291112234", "m2@b.c");
    t1.addPlayer(m1);
    t1.addPlayer(m1);
    REQUIRE(t1.getPlayers().size() == 1);
    REQUIRE_THROWS_AS(t1.addPlayer(m2), SportsClubException);
    t1.removePlayer("M1");
    REQUIRE(t1.getPlayers().empty());

    Match drawMatch(&t1, &t2, today);
    REQUIRE(drawMatch.getHomeTeam() == &t1);
    REQUIRE(drawMatch.getAwayTeam() == &t2);
    REQUIRE(drawMatch.getDate() == today);
    REQUIRE(drawMatch.getWinner() == nullptr);
    REQUIRE_THROWS_AS(drawMatch.finish(-1, 0), InvalidDataException);
    drawMatch.finish(1, 1);
    REQUIRE(drawMatch.getHomeScore() == 1);
    REQUIRE(drawMatch.getAwayScore() == 1);
    REQUIRE(drawMatch.isDraw());
    REQUIRE(drawMatch.getWinner() == nullptr);
    REQUIRE(t1.getDraws() == 1);

    REQUIRE_THROWS_AS(Tournament("", "Football", today, 4, 10.0, 100.0), InvalidDataException);
    Tournament tourn("Cup", "Football", today, 2, 50.0, 500.0);
    REQUIRE(tourn.getName() == "Cup");
    REQUIRE(tourn.getSport() == "Football");
    REQUIRE(tourn.getStartDate() == today);
    REQUIRE(tourn.getEntryFee() == Catch::Approx(50.0));
    REQUIRE(tourn.getPrizeFund() == Catch::Approx(500.0));
    REQUIRE_FALSE(tourn.isCompleted());
    REQUIRE(tourn.getLeader() == nullptr);

    FinanceDepartment fin(1000.0);
    tourn.registerTeam(t1, fin);
    REQUIRE_THROWS_AS(tourn.registerTeam(t1, fin), SportsClubException);
    tourn.registerTeam(t2, fin);
    REQUIRE(tourn.getTeams().size() == 2);
    REQUIRE_THROWS_AS(tourn.registerTeam(t3, fin), SportsClubException);

    tourn.generateMatches();
    REQUIRE(tourn.getMatches().size() == 1);
    REQUIRE_FALSE(tourn.isCompleted());
    tourn.getMatches()[0].finish(0, 2);
    REQUIRE(tourn.getMatches()[0].getWinner() == &t2);
    REQUIRE(tourn.isCompleted());
    REQUIRE(tourn.getLeader() == &t2);
    REQUIRE(t2.calculateWinRate() > 0.0);
    REQUIRE(t1.getLosses() == 1);

    Match mHomeWin(&t1, &t2, today);
    mHomeWin.finish(3, 0);
    REQUIRE(mHomeWin.getWinner() == &t1);
    REQUIRE(tourn.getLeader() != nullptr);
}
