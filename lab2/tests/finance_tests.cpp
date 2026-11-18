#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "exceptions/InsufficientFundsException.h"
#include "exceptions/InvalidDataException.h"
#include "exceptions/OutOfStockException.h"
#include "exceptions/PaymentException.h"
#include "exceptions/SportsClubException.h"
#include "facilities/MaintenanceRecord.h"
#include "finance/Discount.h"
#include "finance/Expense.h"
#include "finance/FinanceDepartment.h"
#include "finance/Invoice.h"
#include "finance/InvoiceItem.h"
#include "finance/Payment.h"
#include "finance/Payroll.h"
#include "finance/Product.h"
#include "finance/SportsShop.h"
#include "members/LoyaltyProgram.h"
#include "members/Member.h"
#include "staff/Coach.h"

TEST_CASE("Discount, Expense, Payment, InvoiceItem and Invoice", "[finance][Invoice]") {
    Date today(1, 6, 2026);
    Date validUntil(10, 6, 2026);

    REQUIRE_THROWS_AS(Discount("", 10.0, validUntil, 5), InvalidDataException);
    Discount disc("SAVE20", 20.0, validUntil, 1);
    REQUIRE(disc.getCode() == "SAVE20");
    REQUIRE(disc.getPercent() == Catch::Approx(20.0));
    REQUIRE(disc.getValidUntil() == validUntil);
    REQUIRE(disc.getUsageLimit() == 1);
    REQUIRE(disc.getUsedCount() == 0);
    REQUIRE(disc.getRemainingUses() == 1);
    REQUIRE(disc.previewPrice(-10.0) == Catch::Approx(0.0));
    REQUIRE(disc.apply(100.0, today) == Catch::Approx(80.0));
    REQUIRE_THROWS_AS(disc.apply(100.0, today), InvalidDataException);

    REQUIRE_THROWS_AS(Expense(ExpenseCategory::Other, -1.0, today, "Bad"), InvalidDataException);
    Expense exp(ExpenseCategory::Utilities, 120.0, today, "Electricity");
    REQUIRE(exp.getCategory() == ExpenseCategory::Utilities);
    REQUIRE(exp.getAmount() == Catch::Approx(120.0));
    REQUIRE(exp.getDate() == today);
    REQUIRE(exp.getDescription() == "Electricity");
    REQUIRE(exp.isInPeriod(today, validUntil));

    REQUIRE_THROWS_AS(Payment("", "M1", 10.0, today, PaymentMethod::Cash), InvalidDataException);
    Payment pay("P1", "M1", 100.0, today, PaymentMethod::Card);
    REQUIRE(pay.getId() == "P1");
    REQUIRE(pay.getPayerId() == "M1");
    REQUIRE(pay.getAmount() == Catch::Approx(100.0));
    REQUIRE(pay.getDate() == today);
    REQUIRE(pay.getMethod() == PaymentMethod::Card);
    REQUIRE(pay.getStatus() == PaymentStatus::Pending);
    REQUIRE_THROWS_AS(pay.refund(), PaymentException);
    pay.fail();
    REQUIRE(pay.getStatus() == PaymentStatus::Failed);
    pay.process();
    REQUIRE(pay.isCompleted());
    pay.refund();
    REQUIRE(pay.getStatus() == PaymentStatus::Refunded);

    REQUIRE_THROWS_AS(InvoiceItem("", 1, 10.0), InvalidDataException);
    InvoiceItem item("Massage", 2, 50.0);
    REQUIRE(item.getDescription() == "Massage");
    REQUIRE(item.getQuantity() == 2);
    REQUIRE(item.getUnitPrice() == Catch::Approx(50.0));
    REQUIRE(item.calculateTotal() == Catch::Approx(100.0));

    REQUIRE_THROWS_AS(Invoice("", nullptr, today, validUntil), InvalidDataException);
    Invoice inv("INV-1", nullptr, today, validUntil);
    REQUIRE(inv.getNumber() == "INV-1");
    REQUIRE(inv.getMember() == nullptr);
    REQUIRE(inv.getIssueDate() == today);
    REQUIRE(inv.getDueDate() == validUntil);
    inv.addItem(item);
    REQUIRE(inv.getItems().size() == 1);

    Discount expiredDisc("OLD", 10.0, Date(1, 1, 2026), 1);
    REQUIRE_THROWS_AS(inv.applyDiscount(expiredDisc, today), InvalidDataException);
    Discount validDisc("NEW", 10.0, validUntil, 2);
    inv.applyDiscount(validDisc, today);
    REQUIRE(inv.calculateTotal() == Catch::Approx(90.0));
    REQUIRE(inv.isOverdue(Date(20, 6, 2026)));

    Payment pendingPay("P2", "M1", 90.0, today, PaymentMethod::Cash);
    REQUIRE_THROWS_AS(inv.markPaid(pendingPay), PaymentException);
    pendingPay.process();
    inv.markPaid(pendingPay);
    REQUIRE(inv.isPaid());
    REQUIRE_THROWS_AS(inv.markPaid(pendingPay), PaymentException);
    REQUIRE_THROWS_AS(inv.addItem(item), PaymentException);
    REQUIRE_THROWS_AS(inv.applyDiscount(validDisc, today), PaymentException);
}

TEST_CASE("FinanceDepartment, Payroll, Product and SportsShop", "[finance][SportsShop]") {
    Date today(1, 6, 2026);
    Date birth(1, 1, 1990);
    FinanceDepartment fin(200.0);

    Payment p1("P1", "M1", 100.0, today, PaymentMethod::Cash);
    p1.process();
    fin.registerPayment(p1);
    REQUIRE(fin.getPayments().size() == 1);
    REQUIRE(fin.getBalance() == Catch::Approx(300.0));

    REQUIRE_THROWS_AS(fin.registerExpense(Expense(ExpenseCategory::Other, 500.0, today, "TooMuch")), InsufficientFundsException);
    MaintenanceRecord mrec(nullptr, "T1", today, 50.0, "Repair");
    fin.registerMaintenance(mrec);
    REQUIRE(fin.getExpenses().size() == 1);
    REQUIRE(fin.calculateIncome(today, today) == Catch::Approx(100.0));
    REQUIRE(fin.calculateExpenses(today, today) == Catch::Approx(50.0));
    REQUIRE(fin.calculateProfit(today, today) == Catch::Approx(50.0));

    fin.refundPayment("P1");
    REQUIRE_THROWS_AS(fin.refundPayment("UNKNOWN"), PaymentException);
    Payment pBig("PBIG", "M1", 1000.0, today, PaymentMethod::Cash);
    pBig.process();
    fin.registerPayment(pBig);
    fin.registerExpense(Expense(ExpenseCategory::Other, 1100.0, today, "Spend"));
    REQUIRE_THROWS_AS(fin.refundPayment("PBIG"), InsufficientFundsException);

    REQUIRE_THROWS_AS(Payroll(-0.1), InvalidDataException);
    Payroll payroll(0.10);
    REQUIRE(payroll.getTaxRate() == Catch::Approx(0.10));
    Coach c1("C1", "Alex", "S", birth, "+375291112233", "c@b.c", 100.0, today, "Fit", 5);
    Coach c2("C2", "Inactive", "S", birth, "+375291112233", "c2@b.c", 100.0, today, "Fit", 5);
    c2.terminate();
    REQUIRE(payroll.calculateTax(c1) == Catch::Approx(10.0));
    FinanceDepartment richFin(5000.0);
    payroll.payAll({&c1, &c2}, richFin, today);
    REQUIRE(payroll.getTotalPaid() == Catch::Approx(90.0));

    REQUIRE_THROWS_AS(Product("", "Cat", 10.0, 5), InvalidDataException);
    Product prod("Bar", "Food", 5.0, 2);
    REQUIRE(prod.getName() == "Bar");
    REQUIRE(prod.getCategory() == "Food");
    REQUIRE(prod.getPrice() == Catch::Approx(5.0));
    REQUIRE_THROWS_AS(prod.sell(0), InvalidDataException);
    REQUIRE_THROWS_AS(prod.sell(5), OutOfStockException);
    REQUIRE_THROWS_AS(prod.restock(0), InvalidDataException);
    REQUIRE_THROWS_AS(prod.changePrice(-1.0), InvalidDataException);
    prod.changePrice(6.0);

    SportsShop shop;
    shop.addProduct(prod);
    shop.restock("Bar", 3);
    REQUIRE(shop.getProducts().size() == 1);
    REQUIRE_THROWS_AS(shop.findProduct("None"), SportsClubException);
    Member mem("M1", "Ivan", "P", birth, "+375291112233", "m@b.c");
    mem.deposit(100.0);
    LoyaltyProgram loyalty(10, 1.0, 0.5);
    double cost = shop.sellToMember("Bar", 4, mem, loyalty);
    REQUIRE(cost == Catch::Approx(24.0));
    REQUIRE(shop.getRevenue() == Catch::Approx(24.0));
    REQUIRE(shop.getLowStockProducts(2).size() == 1);
}
