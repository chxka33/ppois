#include "training/Exercise.h"

#include "exceptions/InvalidDataException.h"

Exercise::Exercise(std::string name, std::string muscleGroup, int sets, int reps, double weightKg)
    : name_(std::move(name)), muscleGroup_(std::move(muscleGroup)), sets_(sets), reps_(reps), weightKg_(weightKg) {
    if (name_.empty() || muscleGroup_.empty() || sets_ <= 0 || reps_ <= 0 || weightKg_ < 0.0) {
        throw InvalidDataException("Invalid exercise parameters");
    }
}

const std::string& Exercise::getName() const { return name_; }
const std::string& Exercise::getMuscleGroup() const { return muscleGroup_; }
int Exercise::getSets() const { return sets_; }
int Exercise::getReps() const { return reps_; }
double Exercise::getWeightKg() const { return weightKg_; }

double Exercise::calculateVolume() const {
    return sets_ * reps_ * weightKg_;
}

void Exercise::increaseWeight(double deltaKg) {
    if (weightKg_ + deltaKg < 0.0) {
        throw InvalidDataException("Weight cannot become negative");
    }
    weightKg_ += deltaKg;
}

void Exercise::scale(double factor) {
    if (factor <= 0.0) {
        throw InvalidDataException("Scale factor must be positive");
    }
    weightKg_ *= factor;
}

int Exercise::estimateDurationMinutes() const {
    return (sets_ * 90 + 60 - 1) / 60;
}
