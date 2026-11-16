#include "TrainingProgram.h"

#include <algorithm>

#include "Coach.h"
#include "InvalidDataException.h"

TrainingProgram::TrainingProgram(std::string name, std::string goal, int weeks, Coach* author)
    : name_(std::move(name)), goal_(std::move(goal)), weeks_(weeks), author_(author) {
    if (name_.empty() || weeks_ <= 0) {
        throw InvalidDataException("Invalid training program parameters");
    }
}

const std::string& TrainingProgram::getName() const { return name_; }
const std::string& TrainingProgram::getGoal() const { return goal_; }
int TrainingProgram::getWeeks() const { return weeks_; }
Coach* TrainingProgram::getAuthor() const { return author_; }
const std::vector<Exercise>& TrainingProgram::getExercises() const { return exercises_; }

void TrainingProgram::addExercise(const Exercise& exercise) {
    exercises_.push_back(exercise);
}

void TrainingProgram::removeExercise(const std::string& exerciseName) {
    exercises_.erase(
        std::remove_if(exercises_.begin(), exercises_.end(),
                       [&exerciseName](const Exercise& ex) { return ex.getName() == exerciseName; }),
        exercises_.end());
}

double TrainingProgram::calculateTotalVolume() const {
    double total = 0.0;
    for (const auto& ex : exercises_) {
        total += ex.calculateVolume();
    }
    return total;
}

int TrainingProgram::estimateSessionDurationMinutes() const {
    int total = 0;
    for (const auto& ex : exercises_) {
        total += ex.estimateDurationMinutes();
    }
    return total;
}

void TrainingProgram::progressAll(double weightIncrementKg) {
    for (auto& ex : exercises_) {
        ex.increaseWeight(weightIncrementKg);
    }
}
