#pragma once

#include <string>
#include <vector>

#include "Exercise.h"

class Coach;

// Программа тренировок, составленная тренером.
class TrainingProgram {
public:
    TrainingProgram(std::string name, std::string goal, int weeks, Coach* author);

    const std::string& getName() const;
    const std::string& getGoal() const;
    int getWeeks() const;
    Coach* getAuthor() const;
    const std::vector<Exercise>& getExercises() const;

    void addExercise(const Exercise& exercise);
    void removeExercise(const std::string& exerciseName);
    double calculateTotalVolume() const;
    int estimateSessionDurationMinutes() const;
    void progressAll(double weightIncrementKg);

private:
    std::string name_;
    std::string goal_;
    int weeks_;
    Coach* author_;
    std::vector<Exercise> exercises_;
};
