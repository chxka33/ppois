#pragma once

namespace constants {

constexpr int kMinutesInHour = 60;
constexpr int kMinutesInDay = 24 * kMinutesInHour;
constexpr int kDaysInWeek = 7;
constexpr int kMonthsInYear = 12;
constexpr int kDaysInYear = 365;
constexpr int kAverageDaysInMonth = 30;
constexpr int kMinYear = 1900;
constexpr int kMaxYear = 2100;

constexpr int kAdultAge = 18;
constexpr int kMinPinLength = 4;
constexpr int kMaxPinAttempts = 3;
constexpr int kMaxFreezeDays = 30;
constexpr int kMembershipReminderDays = 7;

constexpr double kPercentBase = 100.0;
constexpr double kCentimetersInMeter = 100.0;

constexpr double kMinSafeWaterTemperature = 24.0;
constexpr double kMaxSafeWaterTemperature = 30.0;
constexpr double kMinSaunaTemperature = 60.0;
constexpr double kMaxSaunaTemperature = 110.0;

constexpr double kDefaultTaxRate = 0.13;
constexpr double kOvertimeMultiplier = 1.5;
constexpr double kStandardMonthlyHours = 160.0;
constexpr double kCoachBonusPerClient = 50.0;
constexpr double kMemberCourtDiscount = 0.2;

constexpr int kMinRating = 1;
constexpr int kMaxRating = 5;
constexpr int kPositiveReviewThreshold = 4;

constexpr double kBmiUnderweight = 18.5;
constexpr double kBmiOverweight = 25.0;
constexpr double kBmiObese = 30.0;

constexpr int kPointsPerWin = 3;
constexpr int kPointsPerDraw = 1;

constexpr int kSilverTierPoints = 500;
constexpr int kGoldTierPoints = 1500;
constexpr int kPlatinumTierPoints = 5000;

constexpr int kSecondsPerSet = 90;

}  // namespace constants
