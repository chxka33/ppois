#pragma once

#include <string>

// Товар спортивного магазина клуба.
class Product {
public:
    Product(std::string name, std::string category, double price, int stock);

    const std::string& getName() const;
    const std::string& getCategory() const;
    double getPrice() const;
    int getStock() const;

    bool isInStock(int quantity) const;
    double sell(int quantity);
    void restock(int quantity);
    void changePrice(double newPrice);

private:
    std::string name_;
    std::string category_;
    double price_;
    int stock_;
};
