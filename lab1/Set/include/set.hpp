#pragma once
/**
 * @file set.hpp
 * @brief Заголовочный файл класса Set.
 */
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <cmath>

/**
 * @brief Класс, представляющий математическое множество.
 * 
 * Хранит уникальные элементы, отсортированные в лексикографическом порядке.
 * Поддерживает теоретико-множественные операции и разбор строк.
 */
class Set
{
public:
    /**
     * @brief Вложенный класс, представляющий элемент множества.
     * 
     * Может быть либо "атомом" (строкой), либо вложенным множеством.
     */
    class Element
    {
    private:
        bool is_set;                        ///< Флаг: true, если это множество, false — если атом
        std::string atom;                   ///< Значение, если это атом
        std::vector<Element> children;      ///< Дети, если это множество

        friend class Set;

    public:
        /// @brief Конструктор по умолчанию (пустой атом).
        Element();

        /**
         * @brief Конструктор атома.
         * @param atom_val Строковое значение атома.
         */
        Element(const std::string &atom_val);

        /**
         * @brief Конструктор вложенного множества.
         * @param children_val Вектор дочерних элементов.
         */
        Element(const std::vector<Element> &children_val);

        /// @return true, если элемент является множеством.
        bool is_set_Element() const;

        /// @return Ссылка на строковое значение (только для атома).
        const std::string &get_atom() const;

        /// @return Ссылка на вектор детей (только для множества).
        const std::vector<Element> &get_children() const;

        /// @brief Сравнение на равенство.
        bool operator==(const Element &other) const;
        
        /// @brief Сравнение на неравенство.
        bool operator!=(const Element &other) const;
        
        /// @brief Строгий порядок (сначала атомы, потом множества).
        bool operator<(const Element &other) const;

        /**
         * @brief Преобразует элемент в строку.
         * @return Строковое представление (например, "a" или "{a, b}").
         */
        std::string to_string() const;
    };

private:
    std::vector<Element> Elements; ///< Отсортированный вектор уникальных элементов

    /// @brief Сортирует и удаляет дубликаты.
    void normalize();

    /// @brief Вспомогательная рекурсивная функция для парсинга.
    static Element parse_Element_recursive(const std::string &str, size_t &pos);

public:
    /// @brief Конструктор по умолчанию (пустое множество).
    Set();

    /**
     * @brief Конструктор из вектора элементов.
     * @param elem Вектор элементов. Дубликаты будут удалены.
     */
    Set(const std::vector<Element> &elem);

    /// @return true, если множество пусто.
    bool empty() const;

    /// @return Количество элементов во множестве.
    int size() const;

    /// @brief Очищает множество.
    void clear();

    /**
     * @brief Проверяет наличие элемента.
     * @param elem Искомый элемент.
     * @return true, если элемент есть.
     */
    bool contains(const Element &elem) const;

    /**
     * @brief Добавляет элемент.
     * @param elem Добавляемый элемент.
     * @return true, если добавлен; false, если уже был.
     */
    bool add(const Element &elem);

    /**
     * @brief Удаляет элемент.
     * @param elem Удаляемый элемент.
     * @return true, если удален; false, если не найден.
     */
    bool remove(const Element &elem);

    /// @brief Проверка наличия (аналог contains).
    bool operator[](const Element &elem) const;

    /// @brief Объединение множеств.
    Set operator+(const Set &other) const;
    
    /// @brief Объединение с присваиванием.
    Set &operator+=(const Set &other);
    
    /// @brief Добавление элемента с присваиванием.
    Set &operator+=(const Element &elem);

    /// @brief Пересечение множеств.
    Set operator*(const Set &other) const;
    
    /// @brief Пересечение с присваиванием.
    Set &operator*=(const Set &other);

    /// @brief Разность множеств.
    Set operator-(const Set &other) const;
    
    /// @brief Разность с присваиванием.
    Set &operator-=(const Set &other);
    
    /// @brief Удаление элемента с присваиванием.
    Set &operator-=(const Element &elem);

    /**
     * @brief Строит булеан (множество всех подмножеств).
     * @return Множество, содержащее 2^n элементов.
     */
    Set power_set() const;

    /// @brief Сравнение на равенство.
    bool operator==(const Set &other) const;
    
    /// @brief Сравнение на неравенство.
    bool operator!=(const Set &other) const;

    /**
     * @brief Преобразование в строку.
     * @return Строка вида "{a, b}".
     */
    std::string to_string() const;

    /**
     * @brief Парсит строку в множество.
     * 
     * Пример использования:
     * @code
     * Set s = Set::parse("{a, {b, c}}");
     * @endcode
     * @param str Входная строка.
     * @return Разобранное множество.
     */
    static Set parse(const std::string &str);

    /**
     * @brief Парсит строку в один элемент.
     * @param str Входная строка.
     * @return Разобранный элемент.
     */
    static Element parse_Element(const std::string &str);

    /// @brief Ввод из потока.
    friend std::istream &operator>>(std::istream &in, Set &s);
    
    /// @brief Вывод в поток.
    friend std::ostream &operator<<(std::ostream &out, const Set &s);
};