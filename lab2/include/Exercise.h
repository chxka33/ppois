#pragma once

#include <string>

// Упражнение в составе программы тренировок.
class Exercise {
public:
    Exercise(std::string name, std::string muscleGroup, int sets, int reps, double weightKg);

    const std::string& getName() const;
    const std::string& getMuscleGroup() const;
    int getSets() const;
    int getReps() const;
    double getWeightKg() const;

    double calculateVolume() const;
    void increaseWeight(double deltaKg);
    void scale(double factor);
    int estimateDurationMinutes() const;

private:
    std::string name_;
    std::string muscleGroup_;
    int sets_;
    int reps_;
    double weightKg_;
};
