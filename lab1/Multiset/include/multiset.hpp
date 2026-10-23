#pragma once
/**
 * @file multiset.hpp
 * @brief Заголовочный файл класса Multiset.
 */
#include "set.hpp"      // переиспользуем Set::Element
#include <vector>
#include <string>

/**
 * @brief Класс, представляющий мультимножество (multiset).
 * 
 * В отличие от обычного множества, мультимножество допускает хранение 
 * нескольких экземпляров одного и того же элемента. 
 * Элементы хранятся в отсортированном виде.
 */
class Multiset
{
public:
    using Element = Set::Element;   ///< Переиспользование типа Element из класса Set

private:
    std::vector<Element> Elements;  ///< Отсортированный вектор элементов (с возможными повторами)

public:
    /// @brief Конструктор по умолчанию. Создает пустое мультимножество.
    Multiset() = default;

    /**
     * @brief Конструктор из вектора элементов.
     * @param elems Вектор элементов для инициализации. Дубликаты сохраняются.
     */
    Multiset(const std::vector<Element> &elems);

    /// @brief Конструктор копирования (по умолчанию).
    Multiset(const Multiset& other) = default;

    /// @brief Оператор присваивания копированием (по умолчанию).
    Multiset& operator=(const Multiset& other) = default;

    /// @brief Деструктор (по умолчанию).
    ~Multiset() = default;

    /**
     * @brief Проверяет, пусто ли мультимножество.
     * @return true, если элементов нет; false в противном случае.
     */
    bool empty() const;

    /**
     * @brief Возвращает общее количество элементов (с учетом повторов).
     * @return Размер мультимножества.
     */
    int size() const;

    /**
     * @brief Возвращает количество вхождений конкретного элемента.
     * @param elem Элемент для поиска.
     * @return Число вхождений (0, если элемента нет).
     */
    int multiplicity(const Element &elem) const;

    /// @brief Очищает мультимножество.
    void clear();

    /**
     * @brief Проверяет наличие элемента (хотя бы одного).
     * @param elem Элемент для проверки.
     * @return true, если элемент есть.
     */
    bool contains(const Element &elem) const;

    /**
     * @brief Добавляет один экземпляр элемента.
     * @param elem Добавляемый элемент.
     * @return Новое количество вхождений этого элемента.
     */
    int add(const Element &elem);

    /**
     * @brief Добавляет n экземпляров элемента.
     * @param elem Добавляемый элемент.
     * @param n Количество добавляемых копий.
     * @return Новое количество вхождений этого элемента.
     */
    int add(const Element &elem, int n);

    /**
     * @brief Удаляет один экземпляр элемента.
     * @param elem Элемент для удаления.
     * @return true, если элемент был удален; false, если его не было.
     */
    bool remove(const Element &elem);

    /**
     * @brief Удаляет все экземпляры элемента.
     * @param elem Элемент для полного удаления.
     * @return Количество удаленных экземпляров.
     */
    int remove_all(const Element &elem);

    /**
     * @brief Проверяет наличие элемента (аналог contains).
     * @param elem Элемент для проверки.
     * @return true, если элемент есть.
     */
    bool operator[](const Element &elem) const;

    /**
     * @brief Объединение мультимножеств (максимум из кратностей).
     * @param other Второе мультимножество.
     * @return Новое мультимножество.
     */
    Multiset operator+(const Multiset &other) const;

    /**
     * @brief Объединение с присваиванием.
     * @param other Второе мультимножество.
     * @return Ссылка на измененное текущее мультимножество.
     */
    Multiset &operator+=(const Multiset &other);

    /**
     * @brief Добавление одного элемента с присваиванием.
     * @param elem Элемент.
     * @return Ссылка на текущее мультимножество.
     */
    Multiset &operator+=(const Element &elem);

    /**
     * @brief Пересечение мультимножеств (минимум из кратностей).
     * @param other Второе мультимножество.
     * @return Новое мультимножество.
     */
    Multiset operator*(const Multiset &other) const;

    /**
     * @brief Пересечение с присваиванием.
     * @param other Второе мультимножество.
     * @return Ссылка на текущее мультимножество.
     */
    Multiset &operator*=(const Multiset &other);

    /**
     * @brief Разность мультимножеств (вычитание кратностей).
     * @param other Вычитаемое мультимножество.
     * @return Новое мультимножество.
     */
    Multiset operator-(const Multiset &other) const;

    /**
     * @brief Разность с присваиванием.
     * @param other Вычитаемое мультимножество.
     * @return Ссылка на текущее мультимножество.
     */
    Multiset &operator-=(const Multiset &other);

    /**
     * @brief Удаление одного элемента с присваиванием.
     * @param elem Удаляемый элемент.
     * @return Ссылка на текущее мультимножество.
     */
    Multiset &operator-=(const Element &elem);

    /**
     * @brief Строит булеан (множество всех подмножеств).
     * @return Новое мультимножество, содержащее все подмножества.
     */
    Multiset power_set() const;

    /**
     * @brief Оператор сравнения на равенство.
     * @param other Другое мультимножество.
     * @return true, если кратности всех элементов совпадают.
     */
    bool operator==(const Multiset &other) const;

    /**
     * @brief Оператор сравнения на неравенство.
     * @param other Другое мультимножество.
     * @return true, если мультимножества различаются.
     */
    bool operator!=(const Multiset &other) const;

    /**
     * @brief Преобразует мультимножество в строку.
     * @return Строковое представление вида "{a, a, b}".
     */
    std::string to_string() const;

    /**
     * @brief Разбирает строку и создает мультимножество.
     * @param str Строка (например, "{a, a, b}").
     * @return Новое мультимножество.
     */
    static Multiset parse(const std::string &str);

    /**
     * @brief Разбирает строку в один элемент (прокси к Set::parse_Element).
     * @param str Строка для разбора.
     * @return Разобранный элемент.
     */
    static Element parse_Element(const std::string &str);

    /// @brief Вывод в поток.
    friend std::ostream &operator<<(std::ostream &out, const Multiset &s);
    
    /// @brief Ввод из потока.
    friend std::istream &operator>>(std::istream &in, Multiset &s);
};