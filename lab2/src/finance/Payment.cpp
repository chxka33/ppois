#include "finance/Payment.h"

#include "exceptions/InvalidDataException.h"
#include "exceptions/PaymentException.h"

Payment::Payment(std::string id, std::string payerId, double amount, const Date& date, PaymentMethod method)
    : id_(std::move(id)), payerId_(std::move(payerId)), amount_(amount), date_(date),
      method_(method), status_(PaymentStatus::Pending) {
    if (id_.empty() || amount_ < 0.0) {
        throw InvalidDataException("Invalid payment parameters");
    }
}

const std::string& Payment::getId() const { return id_; }
const std::string& Payment::getPayerId() const { return payerId_; }
double Payment::getAmount() const { return amount_; }
const Date& Payment::getDate() const { return date_; }
PaymentMethod Payment::getMethod() const { return method_; }
PaymentStatus Payment::getStatus() const { return status_; }

void Payment::process() {
    status_ = PaymentStatus::Completed;
}

void Payment::fail() {
    status_ = PaymentStatus::Failed;
}

void Payment::refund() {
    if (status_ != PaymentStatus::Completed) {
        throw PaymentException("Only completed payments can be refunded");
    }
    status_ = PaymentStatus::Refunded;
}

bool Payment::isCompleted() const {
    return status_ == PaymentStatus::Completed;
}
