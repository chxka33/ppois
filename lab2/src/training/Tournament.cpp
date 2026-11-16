#include "training/Tournament.h"

#include <algorithm>

#include "finance/FinanceDepartment.h"
#include "exceptions/InvalidDataException.h"
#include "finance/Payment.h"
#include "exceptions/SportsClubException.h"
#include "training/Team.h"

Tournament::Tournament(std::string name, std::string sport, const Date& startDate,
                       int maxTeams, double entryFee, double prizeFund)
    : name_(std::move(name)), sport_(std::move(sport)), startDate_(startDate),
      maxTeams_(maxTeams), entryFee_(entryFee), prizeFund_(prizeFund) {
    if (name_.empty() || maxTeams_ < 2 || entryFee_ < 0.0 || prizeFund_ < 0.0) {
        throw InvalidDataException("Invalid tournament parameters");
    }
}

const std::string& Tournament::getName() const { return name_; }
const std::string& Tournament::getSport() const { return sport_; }
const Date& Tournament::getStartDate() const { return startDate_; }
const std::vector<Team*>& Tournament::getTeams() const { return teams_; }
std::vector<Match>& Tournament::getMatches() { return matches_; }
double Tournament::getEntryFee() const { return entryFee_; }
double Tournament::getPrizeFund() const { return prizeFund_; }

void Tournament::registerTeam(Team& team, FinanceDepartment& finance) {
    if (isRegistered(team)) {
        throw SportsClubException("Team is already registered: " + team.getName());
    }
    if (static_cast<int>(teams_.size()) >= maxTeams_) {
        throw SportsClubException("Tournament is full: " + name_);
    }
    teams_.push_back(&team);
    if (entryFee_ > 0.0) {
        Payment p("TOURNAMENT_" + name_ + "_" + team.getName(), team.getName(), entryFee_, startDate_, PaymentMethod::Balance);
        p.process();
        finance.registerPayment(p);
    }
}

bool Tournament::isRegistered(const Team& team) const {
    return std::find(teams_.begin(), teams_.end(), &team) != teams_.end();
}

void Tournament::generateMatches() {
    matches_.clear();
    for (size_t i = 0; i < teams_.size(); ++i) {
        for (size_t j = i + 1; j < teams_.size(); ++j) {
            matches_.emplace_back(teams_[i], teams_[j], startDate_);
        }
    }
}

bool Tournament::isCompleted() const {
    if (matches_.empty()) {
        return false;
    }
    for (const auto& m : matches_) {
        if (!m.isFinished()) {
            return false;
        }
    }
    return true;
}

Team* Tournament::getLeader() const {
    if (teams_.empty()) {
        return nullptr;
    }
    Team* leader = teams_[0];
    for (auto* t : teams_) {
        if (t->calculatePoints() > leader->calculatePoints()) {
            leader = t;
        } else if (t->calculatePoints() == leader->calculatePoints()) {
            if (t->getWins() > leader->getWins()) {
                leader = t;
            }
        }
    }
    return leader;
}
