#include "Product.h"

#include "InvalidDataException.h"
#include "OutOfStockException.h"

Product::Product(std::string name, std::string category, double price, int stock)
    : name_(std::move(name)), category_(std::move(category)), price_(price), stock_(stock) {
    if (name_.empty() || price_ < 0.0 || stock_ < 0) {
        throw InvalidDataException("Invalid product parameters");
    }
}

const std::string& Product::getName() const { return name_; }
const std::string& Product::getCategory() const { return category_; }
double Product::getPrice() const { return price_; }
int Product::getStock() const { return stock_; }

bool Product::isInStock(int quantity) const {
    return quantity > 0 && stock_ >= quantity;
}

double Product::sell(int quantity) {
    if (quantity <= 0) {
        throw InvalidDataException("Quantity must be positive");
    }
    if (!isInStock(quantity)) {
        throw OutOfStockException("Not enough stock for product: " + name_);
    }
    stock_ -= quantity;
    return price_ * quantity;
}

void Product::restock(int quantity) {
    if (quantity <= 0) {
        throw InvalidDataException("Restock quantity must be positive");
    }
    stock_ += quantity;
}

void Product::changePrice(double newPrice) {
    if (newPrice < 0.0) {
        throw InvalidDataException("Price cannot be negative");
    }
    price_ = newPrice;
}
