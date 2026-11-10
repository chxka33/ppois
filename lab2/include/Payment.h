#pragma once

#include <string>

#include "Date.h"
#include "Enums.h"

class Payment {
public:
    Payment(std::string id, std::string payerId, double amount, const Date& date, PaymentMethod method);

    const std::string& getId() const;
    const std::string& getPayerId() const;
    double getAmount() const;
    const Date& getDate() const;
    PaymentMethod getMethod() const;
    PaymentStatus getStatus() const;

    void process();
    void fail();
    void refund();
    bool isCompleted() const;

private:
    std::string id_;
    std::string payerId_;
    double amount_;
    Date date_;
    PaymentMethod method_;
    PaymentStatus status_;
};
