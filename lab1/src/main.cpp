#include "../include/set.hpp"
#include "../include/multiset.hpp"
#include <iostream>
#include <string>
#include <limits>

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void runSetMenu() {
    Set currentSet;
    std::string input;
    int choice = -1;

    while (choice != 0) {
        std::cout << "\n--- Меню Set ---\n"
                  << "1. Задать множество из строки\n"
                  << "2. Вывести множество\n"
                  << "3. Добавить элемент\n"
                  << "4. Удалить элемент\n"
                  << "5. Объединение (A + B)\n"
                  << "6. Пересечение (A * B)\n"
                  << "7. Разность (A - B)\n"
                  << "8. Булеан (power_set)\n"
                  << "0. Назад\n"
                  << "Выбор: ";
        std::cin >> choice;
        clearInput();

        switch (choice) {
            case 1: {
                std::cout << "Введите строку (например, {1, 2, {3}}): ";
                std::getline(std::cin, input);
                currentSet = Set::parse(input);
                std::cout << "Создано: " << currentSet << "\n";
                break;
            }
            case 2:
                std::cout << "Текущее множество: " << currentSet << "\n";
                break;
            case 3: {
                std::cout << "Введите атом для добавления: ";
                std::getline(std::cin, input);
                currentSet.add(Set::Element(input));
                std::cout << "Добавлено. Итог: " << currentSet << "\n";
                break;
            }
            case 4: {
                std::cout << "Введите атом для удаления: ";
                std::getline(std::cin, input);
                if (currentSet.remove(Set::Element(input))) {
                    std::cout << "Удалено. Итог: " << currentSet << "\n";
                } else {
                    std::cout << "Элемент не найден.\n";
                }
                break;
            }
            case 5: case 6: case 7: {
                std::cout << "Введите второе множество: ";
                std::getline(std::cin, input);
                Set second = Set::parse(input);
                if (choice == 5) std::cout << "Объединение: " << (currentSet + second) << "\n";
                if (choice == 6) std::cout << "Пересечение: " << (currentSet * second) << "\n";
                if (choice == 7) std::cout << "Разность: " << (currentSet - second) << "\n";
                break;
            }
            case 8:
                std::cout << "Булеан: " << currentSet.power_set() << "\n";
                break;
            case 0:
                break;
            default:
                std::cout << "Неверный пункт меню.\n";
        }
    }
}

void runMultisetMenu() {
    Multiset currentMultiset;
    std::string input;
    int choice = -1;

    while (choice != 0) {
        std::cout << "\n--- Меню Multiset ---\n"
                  << "1. Задать мультимножество из строки\n"
                  << "2. Вывести мультимножество\n"
                  << "3. Добавить элемент\n"
                  << "4. Удалить одно вхождение\n"
                  << "5. Удалить все вхождения\n"
                  << "6. Сумма (A + B)\n"
                  << "7. Пересечение (A * B)\n"
                  << "8. Разность (A - B)\n"
                  << "9. Булеан (power_set)\n"
                  << "0. Назад\n"
                  << "Выбор: ";
        std::cin >> choice;
        clearInput();

        switch (choice) {
            case 1: {
                std::cout << "Введите строку (например, {1, 1, 2}): ";
                std::getline(std::cin, input);
                currentMultiset = Multiset::parse(input);
                std::cout << "Создано: " << currentMultiset << "\n";
                break;
            }
            case 2:
                std::cout << "Текущее мультимножество: " << currentMultiset << "\n";
                break;
            case 3: {
                std::cout << "Введите атом: ";
                std::getline(std::cin, input);
                int count = currentMultiset.add(Multiset::Element(input));
                std::cout << "Добавлено. Кратность: " << count << "\n";
                break;
            }
            case 4: {
                std::cout << "Введите атом для удаления: ";
                std::getline(std::cin, input);
                if (currentMultiset.remove(Multiset::Element(input))) {
                    std::cout << "Удалено одно вхождение.\n";
                } else {
                    std::cout << "Элемент не найден.\n";
                }
                break;
            }
            case 5: {
                std::cout << "Введите атом для удаления всех вхождений: ";
                std::getline(std::cin, input);
                int removed = currentMultiset.remove_all(Multiset::Element(input));
                std::cout << "Удалено копий: " << removed << "\n";
                break;
            }
            case 6: case 7: case 8: {
                std::cout << "Введите второе мультимножество: ";
                std::getline(std::cin, input);
                Multiset second = Multiset::parse(input);
                if (choice == 6) std::cout << "Сумма: " << (currentMultiset + second) << "\n";
                if (choice == 7) std::cout << "Пересечение: " << (currentMultiset * second) << "\n";
                if (choice == 8) std::cout << "Разность: " << (currentMultiset - second) << "\n";
                break;
            }
            case 9:
                std::cout << "Булеан: " << currentMultiset.power_set() << "\n";
                break;
            case 0:
                break;
            default:
                std::cout << "Неверный пункт меню.\n";
        }
    }
}

int main() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n=== ГЛАВНОЕ МЕНЮ ===\n"
                  << "1. Работа с Set (множество)\n"
                  << "2. Работа с Multiset (мультимножество)\n"
                  << "0. Выход\n"
                  << "Выбор: ";
        std::cin >> choice;
        clearInput();

        if (choice == 1) runSetMenu();
        else if (choice == 2) runMultisetMenu();
        else if (choice != 0) std::cout << "Неверный пункт меню.\n";
    }
    return 0;
}