#pragma once

enum class AccessLevel { Basic, Standard, Premium, Vip };

enum class FacilityType { Gym, SwimmingPool, TennisCourt, Sauna, LockerRoom };

enum class EquipmentCondition { New, Good, Worn, Broken };

enum class Intensity { Low, Medium, High };

enum class SessionStatus { Planned, InProgress, Completed, Cancelled };

enum class BookingStatus { Pending, Confirmed, Cancelled };

enum class PaymentMethod { Cash, Card, Balance };

enum class PaymentStatus { Pending, Completed, Refunded, Failed };

enum class LoyaltyTier { Bronze, Silver, Gold, Platinum };

enum class CourtSurface { Hard, Clay, Grass };

enum class ExpenseCategory { Salary, Maintenance, Utilities, Purchase, Other };
