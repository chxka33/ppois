#pragma once

#include <string>
#include <vector>

#include "Date.h"
#include "InvoiceItem.h"

class Member;
class Discount;
class Payment;

// Счёт, выставленный клиенту.
class Invoice {
public:
    Invoice(std::string number, Member* member, const Date& issueDate, const Date& dueDate);

    const std::string& getNumber() const;
    Member* getMember() const;
    const std::vector<InvoiceItem>& getItems() const;
    const Date& getIssueDate() const;
    const Date& getDueDate() const;
    bool isPaid() const;

    void addItem(const InvoiceItem& item);
    double calculateSubtotal() const;
    double calculateTotal() const;
    void applyDiscount(Discount& discount, const Date& today);
    void markPaid(const Payment& payment);
    bool isOverdue(const Date& today) const;

private:
    std::string number_;
    Member* member_;
    std::vector<InvoiceItem> items_;
    Date issueDate_;
    Date dueDate_;
    double discountPercent_;
    bool paid_;
};
