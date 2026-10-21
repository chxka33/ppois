#include "../include/set.hpp"
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
                  << "0. Выход\n"
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
                return;
            default:
                std::cout << "Неверный пункт меню.\n";
        }
    }
}

int main() {
    runSetMenu();
    return 0;
}