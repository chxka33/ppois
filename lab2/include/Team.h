#pragma once

#include <string>
#include <vector>

class Coach;
class Member;

// Спортивная команда клуба.
class Team {
public:
    Team(std::string name, std::string sport, Coach* coach, int maxPlayers);

    const std::string& getName() const;
    const std::string& getSport() const;
    Coach* getCoach() const;
    const std::vector<Member*>& getPlayers() const;
    int getWins() const;
    int getLosses() const;
    int getDraws() const;

    void addPlayer(Member& player);
    void removePlayer(const std::string& memberId);
    bool hasPlayer(const std::string& memberId) const;
    void recordWin();
    void recordLoss();
    void recordDraw();
    double calculateWinRate() const;
    int calculatePoints() const;

private:
    std::string name_;
    std::string sport_;
    Coach* coach_;
    std::vector<Member*> players_;
    int maxPlayers_;
    int wins_;
    int losses_;
    int draws_;
};
