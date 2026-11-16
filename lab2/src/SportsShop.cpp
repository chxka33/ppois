#include "SportsShop.h"

#include "LoyaltyProgram.h"
#include "Member.h"
#include "SportsClubException.h"

SportsShop::SportsShop() : revenue_(0.0) {}

const std::vector<Product>& SportsShop::getProducts() const {
    return products_;
}

double SportsShop::getRevenue() const {
    return revenue_;
}

void SportsShop::addProduct(const Product& product) {
    products_.push_back(product);
}

Product& SportsShop::findProduct(const std::string& name) {
    for (auto& p : products_) {
        if (p.getName() == name) {
            return p;
        }
    }
    throw SportsClubException("Product not found: " + name);
}

double SportsShop::sellToMember(const std::string& productName, int quantity,
                                Member& member, const LoyaltyProgram& loyaltyProgram) {
    Product& prod = findProduct(productName);
    double baseCost = prod.sell(quantity);
    LoyaltyTier tier = loyaltyProgram.getTier(member.getBonusPoints());
    double discountPercent = loyaltyProgram.getTierDiscountPercent(tier);
    double finalCost = baseCost * (1.0 - discountPercent / 100.0);
    member.withdraw(finalCost);
    int pointsEarned = loyaltyProgram.calculatePurchasePoints(finalCost);
    member.addBonusPoints(pointsEarned);
    revenue_ += finalCost;
    return finalCost;
}

void SportsShop::restock(const std::string& productName, int quantity) {
    Product& prod = findProduct(productName);
    prod.restock(quantity);
}

std::vector<Product*> SportsShop::getLowStockProducts(int threshold) {
    std::vector<Product*> result;
    for (auto& p : products_) {
        if (p.getStock() <= threshold) {
            result.push_back(&p);
        }
    }
    return result;
}
