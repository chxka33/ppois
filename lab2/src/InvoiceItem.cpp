#include "InvoiceItem.h"

#include "InvalidDataException.h"

InvoiceItem::InvoiceItem(std::string description, int quantity, double unitPrice)
    : description_(std::move(description)), quantity_(quantity), unitPrice_(unitPrice) {
    if (description_.empty() || quantity_ <= 0 || unitPrice_ < 0.0) {
        throw InvalidDataException("Invalid invoice item parameters");
    }
}

const std::string& InvoiceItem::getDescription() const { return description_; }
int InvoiceItem::getQuantity() const { return quantity_; }
double InvoiceItem::getUnitPrice() const { return unitPrice_; }

double InvoiceItem::calculateTotal() const {
    return quantity_ * unitPrice_;
}
