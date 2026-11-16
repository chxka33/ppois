#pragma once

#include <stdexcept>
#include <string>

// Базовое исключение предметной области «Спортивный клуб».
class SportsClubException : public std::runtime_error {
public:
    explicit SportsClubException(const std::string& message) : std::runtime_error(message) {}
};
