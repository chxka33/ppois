#pragma once

#include <string>
#include <vector>

#include "Product.h"

class Member;
class LoyaltyProgram;

class SportsShop {
public:
    SportsShop();

    const std::vector<Product>& getProducts() const;
    double getRevenue() const;

    void addProduct(const Product& product);
    Product& findProduct(const std::string& name);
    double sellToMember(const std::string& productName, int quantity, Member& member,
                        const LoyaltyProgram& loyaltyProgram);
    void restock(const std::string& productName, int quantity);
    std::vector<Product*> getLowStockProducts(int threshold);

private:
    std::vector<Product> products_;
    double revenue_;
};
