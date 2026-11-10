#pragma once

#include "Date.h"

class Team;

class Match {
public:
    Match(Team* homeTeam, Team* awayTeam, const Date& date);

    Team* getHomeTeam() const;
    Team* getAwayTeam() const;
    const Date& getDate() const;
    int getHomeScore() const;
    int getAwayScore() const;
    bool isFinished() const;

    void finish(int homeScore, int awayScore);
    bool isDraw() const;
    Team* getWinner() const;

private:
    Team* homeTeam_;
    Team* awayTeam_;
    Date date_;
    int homeScore_;
    int awayScore_;
    bool finished_;
};
