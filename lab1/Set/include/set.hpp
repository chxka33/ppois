#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

class Set
{
public:
    struct Element
    {
    private:
        bool is_set;                   ///< Флаг, указывающий, является ли узел множеством (true) или атомом (false).
        std::string atom;              ///< Текстовое значение атома (используется, если is_set == false).
        std::vector<Element> children; ///< Дочерние элементы подмножества (используется, если is_set == true).

        friend class Set;

    public:
        Element();

        Element(const std::string &atom_val);

        Element(const std::vector<Element> &children_val);

        bool is_set_Element() const;

        const std::string &get_atom() const;

        const std::vector<Element> &get_children() const;

        bool operator==(const Element &other) const;

        bool operator!=(const Element &other) const;

        bool operator<(const Element &other) const;

        std::string to_string() const;
    };

private:
    std::vector<Element> Elements; ///< Вектор элементов текущего множества.
    void normalize();
    static Element parse_Element_recursive(const std::string &str, size_t &pos);

public:
    Set(const Set& other) = default;

    Set& operator=(const Set& other) = default;
    
    ~Set() = default;

    Set();

    Set(const std::vector<Element> &elem);

    bool empty() const;

    int size() const;

    void clear();

    bool contains(const Element &elem) const;

    bool add(const Element &elem);

    bool remove(const Element &elem);

    bool operator[](const Element &elem) const;

    Set operator+(const Set &other) const;
    Set &operator+=(const Set &other);
    Set &operator+=(const Element &elem);

    Set operator*(const Set &other) const;
    Set &operator*=(const Set &other);

    Set operator-(const Set &other) const;
    Set &operator-=(const Set &other);
    Set &operator-=(const Element &elem);

    Set power_set() const;

    bool operator==(const Set &other) const;

    bool operator!=(const Set &other) const;

    std::string to_string() const;

    static Set parse(const std::string &str);

    static Element parse_Element(const std::string &str);

    friend std::ostream &operator<<(std::ostream &out, const Set &s);

    friend std::istream &operator>>(std::istream &in, Set &s);
};