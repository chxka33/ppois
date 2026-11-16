#include "Review.h"

#include "Coach.h"
#include "InvalidDataException.h"

Review::Review(std::string authorId, Coach* coach, int rating, std::string comment, const Date& date)
    : authorId_(std::move(authorId)), coach_(coach), rating_(rating),
      comment_(std::move(comment)), date_(date), published_(false) {
    if (rating < 1 || rating > 5) {
        throw InvalidDataException("Rating must be between 1 and 5");
    }
}

const std::string& Review::getAuthorId() const { return authorId_; }
Coach* Review::getCoach() const { return coach_; }
int Review::getRating() const { return rating_; }
const std::string& Review::getComment() const { return comment_; }
const Date& Review::getDate() const { return date_; }

bool Review::isPositive() const {
    return rating_ >= 4;
}

void Review::publish() {
    published_ = true;
}

bool Review::isPublished() const {
    return published_;
}
