#include "ExerciseBike.h"

#include "InvalidDataException.h"

ExerciseBike::ExerciseBike(std::string inventoryNumber, std::string name, const Date& purchaseDate,
                           double maintenanceIntervalHours, int resistanceLevels)
    : Equipment(std::move(inventoryNumber), std::move(name), purchaseDate, maintenanceIntervalHours),
      resistanceLevels_(resistanceLevels), currentLevel_(1) {
    if (resistanceLevels_ <= 0) {
        throw InvalidDataException("Resistance levels must be positive");
    }
}

std::string ExerciseBike::getCategory() const {
    return "ExerciseBike";
}

int ExerciseBike::getResistanceLevels() const { return resistanceLevels_; }
int ExerciseBike::getCurrentLevel() const { return currentLevel_; }

void ExerciseBike::increaseResistance() {
    if (currentLevel_ < resistanceLevels_) {
        currentLevel_++;
    }
}

void ExerciseBike::decreaseResistance() {
    if (currentLevel_ > 1) {
        currentLevel_--;
    }
}

double ExerciseBike::estimateCaloriesPerMinute(double userWeightKg) const {
    if (userWeightKg <= 0.0) return 0.0;
    return (0.05 + 0.01 * currentLevel_) * userWeightKg;
}
