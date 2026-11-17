#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "club/NotificationService.h"
#include "club/ReportGenerator.h"
#include "club/SportsClub.h"
#include "common/Address.h"
#include "common/Date.h"
#include "common/Enums.h"
#include "common/TimeSlot.h"
#include "exceptions/SportsClubException.h"
#include "facilities/ExerciseBike.h"
#include "facilities/Gym.h"
#include "facilities/Sauna.h"
#include "facilities/SwimmingPool.h"
#include "facilities/TennisCourt.h"
#include "facilities/Treadmill.h"
#include "finance/Payroll.h"
#include "finance/Product.h"
#include "finance/SportsShop.h"
#include "members/AccessCard.h"
#include "members/LoyaltyProgram.h"
#include "members/Member.h"
#include "members/MembershipPlan.h"
#include "staff/Administrator.h"
#include "staff/Cleaner.h"
#include "staff/Coach.h"
#include "staff/MedicalOfficer.h"
#include "staff/Receptionist.h"
#include "staff/Technician.h"
#include "training/GroupClass.h"
#include "training/Workout.h"

namespace {

constexpr int kCurrentDay = 17;
constexpr int kCurrentMonth = 11;
constexpr int kCurrentYear = 2026;
constexpr int kBirthDay = 15;
constexpr int kBirthMonth = 5;
constexpr int kBirthYear = 1995;

constexpr double kInitialClubBalance = 50000.0;
constexpr int kPointsPerVisit = 10;
constexpr int kPointsPerCurrency = 1;
constexpr double kPointValue = 0.5;

constexpr double kBasicPlanPrice = 100.0;
constexpr int kBasicPlanMonths = 1;
constexpr int kBasicVisitsPerWeek = 3;
constexpr double kVipPlanPrice = 250.0;
constexpr int kVipPlanMonths = 3;
constexpr int kUnlimitedVisits = 0;

constexpr int kGymCapacity = 40;
constexpr double kGymArea = 350.0;
constexpr int kPoolCapacity = 20;
constexpr int kPoolLanes = 6;
constexpr double kPoolDepth = 2.0;
constexpr double kPoolTemp = 27.5;
constexpr double kCourtHourlyRate = 40.0;

constexpr double kTreadmillInterval = 200.0;
constexpr double kTreadmillMaxSpeed = 22.0;
constexpr double kTreadmillMaxWeight = 150.0;
constexpr double kBikeInterval = 150.0;
constexpr int kBikeResistanceLevels = 12;

constexpr double kCoachSalary = 1800.0;
constexpr int kCoachMaxClients = 15;
constexpr double kReceptionistSalary = 1200.0;
constexpr int kReceptionDeskNumber = 1;
constexpr double kDoctorSalary = 2000.0;
constexpr int kCertificateValidDays = 365;
constexpr double kTaxRate = 0.13;

constexpr int kWorkoutDuration = 60;
constexpr double kWorkoutCaloriesRate = 0.12;
constexpr int kMorningSlotStart = 600;
constexpr int kAfternoonSlotStart = 840;
constexpr int kVisitStartMinute = 540;
constexpr int kVisitEndMinute = 630;
constexpr int kClassMaxParticipants = 15;

constexpr double kProteinPrice = 45.0;
constexpr int kProteinStock = 20;
constexpr double kDrinkPrice = 4.5;
constexpr int kDrinkStock = 50;
constexpr double kDemoDepositAmount = 1000.0;
constexpr int kInitialMemberId = 100;
constexpr int kInitialEmployeeId = 101;

struct ClubContext {
    Date today;
    SportsClub club;
    Gym* gymPtr;
    TennisCourt* courtPtr;
    Receptionist receptionist;
    MedicalOfficer doctor;
    Coach* mainCoachPtr;
    Workout morningWorkout;
    GroupClass morningClass;
    SportsShop shop;
    Payroll payroll;
    ReportGenerator reporter;
    int nextMemberId;
    int nextEmployeeId;
};

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin >> std::ws, input);
    return input;
}

double readDouble(const std::string& prompt) {
    std::cout << prompt;
    double value = 0.0;
    if (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return 0.0;
    }
    return value;
}

int readInt(const std::string& prompt) {
    std::cout << prompt;
    int value = 0;
    if (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    return value;
}

bool printMembersList(const ClubContext& context) {
    const auto& members = context.club.getMembers();
    if (members.empty()) {
        std::cout << "Список клиентов пуст. Сначала зарегистрируйте клиента (пункт 1 или 12).\n";
        return false;
    }
    std::cout << "\nДоступные клиенты:\n";
    for (const auto& memberPtr : members) {
        bool isActive = memberPtr->hasActiveMembership(context.today);
        std::cout << "  - ID: " << memberPtr->getMemberId() << " | " << memberPtr->getFullName()
                  << " | Баланс: " << memberPtr->getBalance() << " | Бонусы: " << memberPtr->getBonusPoints()
                  << " | Абонемент: " << (isActive ? "Активен" : "Нет") << "\n";
    }
    return true;
}

void printPlansList(const ClubContext& context) {
    std::cout << "\nДоступные тарифные планы:\n";
    for (const auto& plan : context.club.getPlans()) {
        std::cout << "  - " << plan.getName() << " | Цена/мес: " << plan.getMonthlyPrice()
                  << " | Срок: " << plan.getDurationMonths() << " мес."
                  << " | Итого: " << plan.calculateTotalPrice() << "\n";
    }
}

void printFacilitiesList(const ClubContext& context) {
    std::cout << "\nДоступные объекты клуба:\n";
    for (const auto& facilityPtr : context.club.getFacilities()) {
        std::cout << "  - ID: " << facilityPtr->getId() << " | " << facilityPtr->getName()
                  << " | Занято: " << facilityPtr->getOccupancy() << "/" << facilityPtr->getCapacity()
                  << " | Статус: " << (facilityPtr->isOpen() ? "Открыт" : "Закрыт") << "\n";
    }
}

bool printEmployeesList(const ClubContext& context) {
    const auto& employees = context.club.getEmployees();
    if (employees.empty()) {
        std::cout << "Список сотрудников пуст.\n";
        return false;
    }
    std::cout << "\nТекущие сотрудники клуба:\n";
    for (const auto& employeePtr : employees) {
        std::cout << "  - ID: " << employeePtr->getEmployeeId() << " | " << employeePtr->getFullName()
                  << " | Должность: " << employeePtr->getPosition()
                  << " | Оклад: " << employeePtr->getBaseSalary() << "\n";
    }
    return true;
}

void printClassesList(ClubContext& context) {
    const auto& classes = context.club.getSchedule().getGroupClasses();
    std::cout << "\nДоступные групповые занятия:\n";
    for (const auto* groupClassPtr : classes) {
        std::cout << "  - " << groupClassPtr->getName()
                  << " | Время: " << groupClassPtr->getSlot().toString()
                  << " | Свободных мест: " << groupClassPtr->getFreeSpots()
                  << "/" << groupClassPtr->getMaxParticipants() << "\n";
    }
}

void printProductsList(const ClubContext& context) {
    std::cout << "\nТовары в спортивном магазине:\n";
    for (const auto& product : context.shop.getProducts()) {
        std::cout << "  - " << product.getName() << " (" << product.getCategory() << ")"
                  << " | Цена: " << product.getPrice()
                  << " | В наличии: " << product.getStock() << " шт.\n";
    }
}

void setupPlansAndShop(ClubContext& context) {
    MembershipPlan basicPlan("Basic", kBasicPlanPrice, kBasicPlanMonths, AccessLevel::Basic,
                             kBasicVisitsPerWeek, false, false);
    MembershipPlan vipPlan("VIP", kVipPlanPrice, kVipPlanMonths, AccessLevel::Vip,
                           kUnlimitedVisits, true, true);
    context.club.addPlan(basicPlan);
    context.club.addPlan(vipPlan);

    Product protein("WheyProtein", "Nutrition", kProteinPrice, kProteinStock);
    Product isotonic("IsotonicDrink", "Drinks", kDrinkPrice, kDrinkStock);
    context.shop.addProduct(protein);
    context.shop.addProduct(isotonic);
}

void setupFacilitiesAndStaff(ClubContext& context) {
    auto gym = std::make_unique<Gym>("GYM-1", "Main Gym", kGymCapacity, AccessLevel::Basic, kGymArea);
    gym->addEquipment(std::make_unique<Treadmill>("TR-01", "ProRunner", context.today,
                                                  kTreadmillInterval, kTreadmillMaxSpeed, kTreadmillMaxWeight));
    gym->addEquipment(std::make_unique<ExerciseBike>("BK-01", "AeroBike", context.today,
                                                     kBikeInterval, kBikeResistanceLevels));
    context.gymPtr = static_cast<Gym*>(&context.club.addFacility(std::move(gym)));

    auto pool = std::make_unique<SwimmingPool>("POOL-1", "Olympic Pool", kPoolCapacity,
                                               AccessLevel::Premium, kPoolLanes, kPoolDepth, kPoolTemp);
    context.club.addFacility(std::move(pool));

    auto court = std::make_unique<TennisCourt>("COURT-1", "Center Court", AccessLevel::Basic,
                                               CourtSurface::Hard, true, kCourtHourlyRate);
    context.courtPtr = static_cast<TennisCourt*>(&context.club.addFacility(std::move(court)));

    Date birthDate(kBirthDay, kBirthMonth, kBirthYear);
    auto coach = std::make_unique<Coach>("100", "Alexey", "Smirnov", birthDate, "+375291112233",
                                         "smirnov@club.by", kCoachSalary, context.today, "CrossFit", kCoachMaxClients);
    context.mainCoachPtr = static_cast<Coach*>(&context.club.hireEmployee(std::move(coach)));
}

void attachSchedule(ClubContext& context) {
    TimeSlot classSlot(context.today, kMorningSlotStart, kWorkoutDuration);
    context.morningClass = GroupClass("CrossFit Group", context.morningWorkout, context.mainCoachPtr,
                                      context.gymPtr, classSlot, kClassMaxParticipants);
    context.club.getSchedule().addGroupClass(context.morningClass);
}

ClubContext createDefaultContext() {
    Date today(kCurrentDay, kCurrentMonth, kCurrentYear);
    Date birthDate(kBirthDay, kBirthMonth, kBirthYear);
    Address address("Minsk", "Pobediteley Ave", "10", "220004");
    LoyaltyProgram loyalty(kPointsPerVisit, kPointsPerCurrency, kPointValue);
    Workout workout("Morning CrossFit", Intensity::Medium, kWorkoutDuration, kWorkoutCaloriesRate);
    TimeSlot classSlot(today, kMorningSlotStart, kWorkoutDuration);
    ClubContext context{
        today, SportsClub("Olympus Sports Club", address, kInitialClubBalance, loyalty),
        nullptr, nullptr,
        Receptionist("REC-1", "Elena", "Orlova", birthDate, "+375292223344",
                     "orlova@club.by", kReceptionistSalary, today, kReceptionDeskNumber),
        MedicalOfficer("DOC-1", "Igor", "Volkov", birthDate, "+375293334455",
                       "volkov@club.by", kDoctorSalary, today, "MED-9981"),
        nullptr, workout, GroupClass("CrossFit Group", workout, nullptr, nullptr, classSlot, kClassMaxParticipants),
        SportsShop(), Payroll(kTaxRate), ReportGenerator("Olympus Sports Club"),
        kInitialMemberId, kInitialEmployeeId
    };
    setupPlansAndShop(context);
    setupFacilitiesAndStaff(context);
    attachSchedule(context);
    return context;
}

void enrollNewMemberInteractive(ClubContext& context) {
    std::string memberId = std::to_string(context.nextMemberId);
    std::string firstName = readLine("Имя: ");
    std::string lastName = readLine("Фамилия: ");
    std::string phone = readLine("Телефон (мин. 7 цифр, например +375291234567): ");
    std::string email = readLine("Email (формат name@domain.com): ");
    std::string pin = readLine("PIN-код пропуска (мин. 4 цифры, например 1234): ");

    Date birthDate(kBirthDay, kBirthMonth, kBirthYear);
    auto member = std::make_unique<Member>(memberId, firstName, lastName, birthDate, phone, email);
    auto card = std::make_unique<AccessCard>("CARD-" + memberId, memberId, context.today, pin);
    member->issueAccessCard(std::move(card));

    Member& registered = context.club.registerMember(std::move(member));
    ++context.nextMemberId;
    context.doctor.examine(registered, context.today, kCertificateValidDays, true);
    std::cout << "Клиент " << registered.getFullName() << " зарегистрирован (присвоен ID: "
              << memberId << ") и прошёл медосмотр.\n";
}

void removeMemberInteractive(ClubContext& context) {
    if (!printMembersList(context)) {
        return;
    }
    std::string memberId = readLine("Введите ID клиента из списка выше для удаления: ");
    context.club.removeMember(memberId);
    std::cout << "Клиент " << memberId << " удалён.\n";
}

void depositMemberBalanceInteractive(ClubContext& context) {
    if (!printMembersList(context)) {
        return;
    }
    std::string memberId = readLine("Введите ID клиента из списка выше: ");
    double amount = readDouble("Сумма пополнения: ");
    Member& member = context.club.findMember(memberId);
    member.deposit(amount);
    std::cout << "Баланс обновлён: " << member.getBalance() << "\n";
}

void sellMembershipInteractive(ClubContext& context) {
    if (!printMembersList(context)) {
        return;
    }
    std::string memberId = readLine("Введите ID клиента из списка выше: ");
    printPlansList(context);
    std::string planName = readLine("Выберите тарифный план из списка выше: ");
    context.club.sellMembership(memberId, planName, context.today);
    std::cout << "Абонемент " << planName << " успешно оформлен для клиента ID " << memberId << ".\n";
}

void checkInMemberInteractive(ClubContext& context) {
    if (!printMembersList(context)) {
        return;
    }
    std::string memberId = readLine("Введите ID клиента из списка выше: ");
    printFacilitiesList(context);
    std::string facilityId = readLine("Выберите ID объекта из списка выше: ");
    std::string pin = readLine("PIN-код карты клиента: ");

    Member& member = context.club.findMember(memberId);
    Facility& facility = context.club.findFacility(facilityId);
    const LoyaltyProgram& loyalty = context.club.getLoyaltyProgram();

    Visit visit = context.receptionist.checkIn(member, facility, pin, context.today, kVisitStartMinute);
    context.receptionist.checkOut(member, visit, kVisitEndMinute, loyalty);
    std::cout << "Посещение завершено. Длительность: " << visit.getDurationMinutes()
              << " мин. Бонусы клиента: " << member.getBonusPoints() << "\n";
}

void hireCoachInteractive(ClubContext& context) {
    std::string empId = std::to_string(context.nextEmployeeId++);
    std::string firstName = readLine("Имя: ");
    std::string lastName = readLine("Фамилия: ");
    std::string spec = readLine("Специализация (например, Yoga / Boxing): ");
    Date birthDate(kBirthDay, kBirthMonth, kBirthYear);
    auto coach = std::make_unique<Coach>(empId, firstName, lastName, birthDate, "+375297778899",
                                         "coach@club.by", kCoachSalary, context.today, spec, kCoachMaxClients);
    context.club.hireEmployee(std::move(coach));
    std::cout << "Сотрудник нанят (присвоен ID: " << empId << ").\n";
}

void manageStaffInteractive(ClubContext& context) {
    printEmployeesList(context);
    int action = readInt("Выберите действие (1 - Нанять тренера, 2 - Уволить сотрудника): ");
    if (action == 1) {
        hireCoachInteractive(context);
    } else if (action == 2) {
        if (context.club.getEmployees().empty()) {
            return;
        }
        std::string empId = readLine("Введите ID сотрудника из списка выше для увольнения: ");
        context.club.fireEmployee(empId);
        std::cout << "Сотрудник " << empId << " уволен.\n";
    }
}

void enrollGroupClassInteractive(ClubContext& context) {
    if (!printMembersList(context)) {
        return;
    }
    printClassesList(context);
    std::string memberId = readLine("Введите ID клиента из списка выше для записи на занятие: ");
    Member& member = context.club.findMember(memberId);
    context.morningClass.enroll(member, context.today);
    std::cout << "Клиент записан на занятие '" << context.morningClass.getName()
              << "'. Осталось свободных мест: " << context.morningClass.getFreeSpots() << "\n";
}

void bookTennisCourtInteractive(ClubContext& context) {
    if (!printMembersList(context)) {
        return;
    }
    std::cout << "\nКорт для бронирования: " << context.courtPtr->getId() << " ("
              << context.courtPtr->getName() << "), тариф: "
              << context.courtPtr->getHourlyRate() << "/час (скидка членам клуба 20%)\n";
    std::string memberId = readLine("Введите ID клиента из списка выше: ");
    Member& member = context.club.findMember(memberId);
    TimeSlot slot(context.today, kAfternoonSlotStart, kWorkoutDuration);
    BookingSystem& bookingSystem = context.club.getBookingSystem();
    Booking& booking = bookingSystem.bookCourt(member, *context.courtPtr, slot);
    std::cout << "Корт забронирован на слот " << slot.toString() << "! Бронь #"
              << booking.getId() << ", списано: " << booking.getCost() << "\n";
}

void sellShopProductInteractive(ClubContext& context) {
    if (!printMembersList(context)) {
        return;
    }
    std::string memberId = readLine("Введите ID покупателя из списка выше: ");
    printProductsList(context);
    std::string productName = readLine("Введите название товара из списка выше: ");
    int quantity = readInt("Количество: ");
    Member& member = context.club.findMember(memberId);
    const LoyaltyProgram& loyalty = context.club.getLoyaltyProgram();
    double paidCost = context.shop.sellToMember(productName, quantity, member, loyalty);
    std::cout << "Покупка совершена на сумму: " << paidCost
              << ". Остаток баланса клиента: " << member.getBalance() << "\n";
}

void paySalariesInteractive(ClubContext& context) {
    if (!printEmployeesList(context)) {
        return;
    }
    std::vector<Employee*> staffList;
    for (const auto& employeePtr : context.club.getEmployees()) {
        staffList.push_back(employeePtr.get());
    }
    FinanceDepartment& finance = context.club.getFinance();
    context.payroll.payAll(staffList, finance, context.today);
    std::cout << "Зарплата выплачена всем сотрудникам (" << staffList.size()
              << " чел.). Текущий бюджет клуба: " << finance.getBalance() << "\n";
}

void showReportsInteractive(ClubContext& context) {
    const FinanceDepartment& finance = context.club.getFinance();
    const Schedule& schedule = context.club.getSchedule();
    std::cout << "\n" << context.reporter.buildMembersReport(context.club, context.today);
    std::cout << "\n" << context.reporter.buildFinanceReport(finance, context.today, context.today);
    std::cout << "\n" << context.reporter.buildEquipmentReport(*context.gymPtr);
    std::cout << "\n" << context.reporter.buildScheduleReport(schedule, context.today) << "\n";
}

void runDemoScenario(ClubContext& context) {
    std::string demoId = std::to_string(context.nextMemberId++);
    Date birthDate(kBirthDay, kBirthMonth, kBirthYear);
    auto demoMember = std::make_unique<Member>(demoId, "Ivan", "Petrov", birthDate,
                                               "+375291234567", "ivan@mail.by");
    auto demoCard = std::make_unique<AccessCard>("CARD-" + demoId, demoId, context.today, "1234");
    demoMember->issueAccessCard(std::move(demoCard));

    Member& member = context.club.registerMember(std::move(demoMember));
    context.doctor.examine(member, context.today, kCertificateValidDays, true);
    member.deposit(kDemoDepositAmount);
    context.club.sellMembership(demoId, "VIP", context.today);

    const LoyaltyProgram& loyalty = context.club.getLoyaltyProgram();
    Visit visit = context.receptionist.checkIn(member, *context.gymPtr, "1234", context.today, kVisitStartMinute);
    context.receptionist.checkOut(member, visit, kVisitEndMinute, loyalty);
    context.morningClass.enroll(member, context.today);
    context.shop.sellToMember("WheyProtein", 1, member, loyalty);

    std::cout << "Демо-сценарий успешно выполнен для клиента ID " << demoId << " (Ivan Petrov)!\n";
    showReportsInteractive(context);
}

void printMenu() {
    std::cout << "\n=== СПОРТИВНЫЙ КЛУБ (CLI-МЕНЮ) ===\n"
              << "1. Зарегистрировать клиента (с пропуском и медосмотром)\n"
              << "2. Удалить клиента\n"
              << "3. Пополнить баланс клиента\n"
              << "4. Оформить абонемент клиенту (Basic / VIP)\n"
              << "5. Отметить посещение объекта (Check-In / Check-Out)\n"
              << "6. Управление персоналом (нанять / уволить)\n"
              << "7. Записать клиента на групповую тренировку\n"
              << "8. Забронировать теннисный корт\n"
              << "9. Продать товар в спортивном магазине\n"
              << "10. Выплатить зарплату персоналу\n"
              << "11. Сформировать отчёты клуба\n"
              << "12. Запустить автоматический демо-сценарий\n"
              << "0. Выход\n";
}

bool executeChoice(int choice, ClubContext& context) {
    switch (choice) {
        case 1: enrollNewMemberInteractive(context); return true;
        case 2: removeMemberInteractive(context); return true;
        case 3: depositMemberBalanceInteractive(context); return true;
        case 4: sellMembershipInteractive(context); return true;
        case 5: checkInMemberInteractive(context); return true;
        case 6: manageStaffInteractive(context); return true;
        case 7: enrollGroupClassInteractive(context); return true;
        case 8: bookTennisCourtInteractive(context); return true;
        case 9: sellShopProductInteractive(context); return true;
        case 10: paySalariesInteractive(context); return true;
        case 11: showReportsInteractive(context); return true;
        case 12: runDemoScenario(context); return true;
        case 0: return false;
        default:
            std::cout << "Неизвестный пункт меню.\n";
            return true;
    }
}

}  // namespace

int main() {
    ClubContext context = createDefaultContext();
    bool running = true;
    while (running) {
        printMenu();
        int choice = readInt("Выберите действие: ");
        if (std::cin.eof()) {
            break;
        }
        try {
            running = executeChoice(choice, context);
        } catch (const SportsClubException& error) {
            std::cout << "[Ошибка домена]: " << error.what() << "\n";
        }
    }
    return 0;
}
