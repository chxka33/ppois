#include "training/Match.h"

#include "exceptions/InvalidDataException.h"
#include "training/Team.h"

Match::Match(Team* homeTeam, Team* awayTeam, const Date& date)
    : homeTeam_(homeTeam), awayTeam_(awayTeam), date_(date),
      homeScore_(0), awayScore_(0), finished_(false) {}

Team* Match::getHomeTeam() const { return homeTeam_; }
Team* Match::getAwayTeam() const { return awayTeam_; }
const Date& Match::getDate() const { return date_; }
int Match::getHomeScore() const { return homeScore_; }
int Match::getAwayScore() const { return awayScore_; }
bool Match::isFinished() const { return finished_; }

void Match::finish(int homeScore, int awayScore) {
    if (homeScore < 0 || awayScore < 0) {
        throw InvalidDataException("Match score cannot be negative");
    }
    homeScore_ = homeScore;
    awayScore_ = awayScore;
    finished_ = true;
    if (homeScore > awayScore) {
        if (homeTeam_) homeTeam_->recordWin();
        if (awayTeam_) awayTeam_->recordLoss();
    } else if (awayScore > homeScore) {
        if (awayTeam_) awayTeam_->recordWin();
        if (homeTeam_) homeTeam_->recordLoss();
    } else {
        if (homeTeam_) homeTeam_->recordDraw();
        if (awayTeam_) awayTeam_->recordDraw();
    }
}

bool Match::isDraw() const {
    return finished_ && homeScore_ == awayScore_;
}

Team* Match::getWinner() const {
    if (!finished_ || isDraw()) {
        return nullptr;
    }
    return homeScore_ > awayScore_ ? homeTeam_ : awayTeam_;
}
