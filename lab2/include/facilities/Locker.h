#pragma once

#include <string>

// Шкафчик в раздевалке.
class Locker {
public:
    explicit Locker(int number);

    int getNumber() const;
    bool isOccupied() const;
    const std::string& getOwnerId() const;

    void occupy(const std::string& ownerId, const std::string& code);
    void release();
    bool unlock(const std::string& code) const;

private:
    int number_;
    bool occupied_;
    std::string ownerId_;
    std::string code_;
};
