#pragma once

#include <string>
#include <vector>

#include "Date.h"
#include "Match.h"

class Team;
class FinanceDepartment;

// Турнир между командами по круговой системе.
class Tournament {
public:
    Tournament(std::string name, std::string sport, const Date& startDate, int maxTeams, double entryFee,
               double prizeFund);

    const std::string& getName() const;
    const std::string& getSport() const;
    const Date& getStartDate() const;
    const std::vector<Team*>& getTeams() const;
    std::vector<Match>& getMatches();
    double getEntryFee() const;
    double getPrizeFund() const;

    void registerTeam(Team& team, FinanceDepartment& finance);
    bool isRegistered(const Team& team) const;
    void generateMatches();
    bool isCompleted() const;
    Team* getLeader() const;

private:
    std::string name_;
    std::string sport_;
    Date startDate_;
    int maxTeams_;
    double entryFee_;
    double prizeFund_;
    std::vector<Team*> teams_;
    std::vector<Match> matches_;
};
