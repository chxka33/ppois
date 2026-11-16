#include "ReportGenerator.h"

#include <sstream>

#include "Date.h"
#include "FinanceDepartment.h"
#include "GroupClass.h"
#include "Gym.h"
#include "Schedule.h"
#include "SportsClub.h"

ReportGenerator::ReportGenerator(std::string clubName)
    : clubName_(std::move(clubName)), generatedCount_(0) {}

const std::string& ReportGenerator::getClubName() const { return clubName_; }
int ReportGenerator::getGeneratedCount() const { return generatedCount_; }

std::string ReportGenerator::buildMembersReport(const SportsClub& club, const Date& today) {
    generatedCount_++;
    std::ostringstream oss;
    oss << "=== Members Report for " << clubName_ << " (" << today.toString() << ") ===\n";
    oss << "Total members: " << club.getMembers().size() << "\n";
    oss << "Active members: " << club.countActiveMembers(today) << "\n";
    for (const auto& m : club.getMembers()) {
        if (m) {
            oss << "- " << m->getMemberId() << ": " << m->getFullName()
                << " | Active: " << (m->hasActiveMembership(today) ? "Yes" : "No") << "\n";
        }
    }
    return oss.str();
}

std::string ReportGenerator::buildFinanceReport(const FinanceDepartment& finance, const Date& from, const Date& to) {
    generatedCount_++;
    std::ostringstream oss;
    oss << "=== Finance Report for " << clubName_ << " (" << from.toString() << " - " << to.toString() << ") ===\n";
    oss << "Total Income: " << finance.calculateIncome(from, to) << "\n";
    oss << "Total Expenses: " << finance.calculateExpenses(from, to) << "\n";
    oss << "Net Profit: " << finance.calculateProfit(from, to) << "\n";
    oss << "Current Balance: " << finance.getBalance() << "\n";
    return oss.str();
}

std::string ReportGenerator::buildEquipmentReport(const Gym& gym) {
    generatedCount_++;
    std::ostringstream oss;
    oss << "=== Equipment Report for " << gym.getName() << " ===\n";
    oss << "Total equipment: " << gym.getEquipment().size() << "\n";
    oss << "Working equipment: " << gym.countWorkingEquipment() << "\n";
    oss << "Needing maintenance: " << gym.getEquipmentNeedingMaintenance().size() << "\n";
    return oss.str();
}

std::string ReportGenerator::buildScheduleReport(const Schedule& schedule, const Date& date) {
    generatedCount_++;
    std::ostringstream oss;
    oss << "=== Schedule Report for " << clubName_ << " on " << date.toString() << " ===\n";
    auto classes = schedule.getClassesOn(date);
    oss << "Scheduled classes count: " << classes.size() << "\n";
    for (const auto* gc : classes) {
        if (gc) {
            oss << "- " << gc->getName() << " [" << gc->getSlot().toString() << "]"
                << " Enrolled: " << gc->getParticipants().size() << "/" << gc->getMaxParticipants() << "\n";
        }
    }
    return oss.str();
}
