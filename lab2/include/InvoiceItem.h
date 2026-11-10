#pragma once

#include <string>

class InvoiceItem {
public:
    InvoiceItem(std::string description, int quantity, double unitPrice);

    const std::string& getDescription() const;
    int getQuantity() const;
    double getUnitPrice() const;

    double calculateTotal() const;

private:
    std::string description_;
    int quantity_;
    double unitPrice_;
};
