#pragma once

#include <string>

#include "Date.h"

class Coach;

// Отзыв клиента о тренере.
class Review {
public:
    Review(std::string authorId, Coach* coach, int rating, std::string comment, const Date& date);

    const std::string& getAuthorId() const;
    Coach* getCoach() const;
    int getRating() const;
    const std::string& getComment() const;
    const Date& getDate() const;

    bool isPositive() const;
    void publish();
    bool isPublished() const;

private:
    std::string authorId_;
    Coach* coach_;
    int rating_;
    std::string comment_;
    Date date_;
    bool published_;
};
