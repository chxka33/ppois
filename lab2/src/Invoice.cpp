#include "Invoice.h"

#include "Discount.h"
#include "InvalidDataException.h"
#include "Member.h"
#include "Payment.h"
#include "PaymentException.h"

Invoice::Invoice(std::string number, Member* member, const Date& issueDate, const Date& dueDate)
    : number_(std::move(number)), member_(member), issueDate_(issueDate), dueDate_(dueDate),
      discountPercent_(0.0), paid_(false) {
    if (number_.empty()) {
        throw InvalidDataException("Invoice number cannot be empty");
    }
}

const std::string& Invoice::getNumber() const { return number_; }
Member* Invoice::getMember() const { return member_; }
const std::vector<InvoiceItem>& Invoice::getItems() const { return items_; }
const Date& Invoice::getIssueDate() const { return issueDate_; }
const Date& Invoice::getDueDate() const { return dueDate_; }
bool Invoice::isPaid() const { return paid_; }

void Invoice::addItem(const InvoiceItem& item) {
    if (paid_) {
        throw PaymentException("Cannot add items to already paid invoice");
    }
    items_.push_back(item);
}

double Invoice::calculateSubtotal() const {
    double sum = 0.0;
    for (const auto& item : items_) {
        sum += item.calculateTotal();
    }
    return sum;
}

double Invoice::calculateTotal() const {
    double subtotal = calculateSubtotal();
    return subtotal * (1.0 - discountPercent_ / 100.0);
}

void Invoice::applyDiscount(Discount& discount, const Date& today) {
    if (paid_) {
        throw PaymentException("Cannot apply discount to paid invoice");
    }
    if (!discount.isValid(today)) {
        throw InvalidDataException("Discount is expired or invalid");
    }
    discountPercent_ = discount.getPercent();
    discount.apply(calculateSubtotal(), today);
}

void Invoice::markPaid(const Payment& payment) {
    if (paid_) {
        throw PaymentException("Invoice is already paid");
    }
    if (!payment.isCompleted()) {
        throw PaymentException("Payment is not completed");
    }
    paid_ = true;
}

bool Invoice::isOverdue(const Date& today) const {
    return !paid_ && dueDate_ < today;
}
