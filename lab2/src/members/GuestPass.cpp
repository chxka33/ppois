#include "members/GuestPass.h"

#include "exceptions/AccessDeniedException.h"
#include "exceptions/InvalidDataException.h"
#include "exceptions/InvalidDateException.h"

GuestPass::GuestPass(std::string code, std::string guestName, const Date& validDate, double price)
    : code_(std::move(code)), guestName_(std::move(guestName)), validDate_(validDate),
      price_(price), used_(false) {
    if (code_.empty() || guestName_.empty() || price_ < 0.0) {
        throw InvalidDataException("Invalid guest pass data");
    }
}

const std::string& GuestPass::getCode() const { return code_; }
const std::string& GuestPass::getGuestName() const { return guestName_; }
const Date& GuestPass::getValidDate() const { return validDate_; }
double GuestPass::getPrice() const { return price_; }
bool GuestPass::isUsed() const { return used_; }

bool GuestPass::isValidOn(const Date& date) const {
    return !used_ && validDate_ == date;
}

void GuestPass::use(const Date& today) {
    if (used_) {
        throw AccessDeniedException("Guest pass is already used");
    }
    if (validDate_ != today) {
        throw InvalidDateException("Guest pass is not valid on this date");
    }
    used_ = true;
}
