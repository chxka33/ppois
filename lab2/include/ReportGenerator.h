#pragma once

#include <string>

class SportsClub;
class FinanceDepartment;
class Gym;
class Schedule;
class Date;

// Формирование текстовых отчётов.
class ReportGenerator {
public:
    explicit ReportGenerator(std::string clubName);

    const std::string& getClubName() const;
    int getGeneratedCount() const;

    std::string buildMembersReport(const SportsClub& club, const Date& today);
    std::string buildFinanceReport(const FinanceDepartment& finance, const Date& from, const Date& to);
    std::string buildEquipmentReport(const Gym& gym);
    std::string buildScheduleReport(const Schedule& schedule, const Date& date);

private:
    std::string clubName_;
    int generatedCount_;
};
