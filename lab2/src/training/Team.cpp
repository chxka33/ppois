#include "training/Team.h"

#include <algorithm>

#include "staff/Coach.h"
#include "exceptions/InvalidDataException.h"
#include "members/Member.h"
#include "exceptions/SportsClubException.h"

Team::Team(std::string name, std::string sport, Coach* coach, int maxPlayers)
    : name_(std::move(name)), sport_(std::move(sport)), coach_(coach),
      maxPlayers_(maxPlayers), wins_(0), losses_(0), draws_(0) {
    if (name_.empty() || maxPlayers_ <= 0) {
        throw InvalidDataException("Invalid team parameters");
    }
}

const std::string& Team::getName() const { return name_; }
const std::string& Team::getSport() const { return sport_; }
Coach* Team::getCoach() const { return coach_; }
const std::vector<Member*>& Team::getPlayers() const { return players_; }
int Team::getWins() const { return wins_; }
int Team::getLosses() const { return losses_; }
int Team::getDraws() const { return draws_; }

void Team::addPlayer(Member& player) {
    if (hasPlayer(player.getMemberId())) {
        return;
    }
    if (static_cast<int>(players_.size()) >= maxPlayers_) {
        throw SportsClubException("Team is full: " + name_);
    }
    players_.push_back(&player);
}

void Team::removePlayer(const std::string& memberId) {
    players_.erase(
        std::remove_if(players_.begin(), players_.end(),
                       [&memberId](Member* m) { return m && m->getMemberId() == memberId; }),
        players_.end());
}

bool Team::hasPlayer(const std::string& memberId) const {
    for (const auto* m : players_) {
        if (m && m->getMemberId() == memberId) {
            return true;
        }
    }
    return false;
}

void Team::recordWin() { wins_++; }
void Team::recordLoss() { losses_++; }
void Team::recordDraw() { draws_++; }

double Team::calculateWinRate() const {
    int total = wins_ + losses_ + draws_;
    return total > 0 ? (static_cast<double>(wins_) / total) * 100.0 : 0.0;
}

int Team::calculatePoints() const {
    return wins_ * 3 + draws_ * 1;
}
