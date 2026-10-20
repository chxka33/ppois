#pragma once

#include "set.hpp"      // переиспользуем Set::Element
#include <vector>
#include <string>

class Multiset
{
public:
    using Element = Set::Element;   // ← ключевое переиспользование

private:
    std::vector<Element> Elements;  // отсортирован, но с повторами

public:
    Multiset() = default;

    Multiset(const std::vector<Element> &elems);

    bool empty() const;
    int  size() const;                              // общее число (с повторами)
    int  multiplicity(const Element &elem) const;          // число вхождений
    void clear();
    bool contains(const Element &elem) const;

    int  add(const Element &elem);                  // +1
    int  add(const Element &elem, int n);           // +n
    bool remove(const Element &elem);               // −1
    int  remove_all(const Element &elem);           // удалить все

    bool operator[](const Element &elem) const;

    Multiset  operator+(const Multiset &other) const;
    Multiset &operator+=(const Multiset &other);
    Multiset &operator+=(const Element &elem);

    Multiset  operator*(const Multiset &other) const;
    Multiset &operator*=(const Multiset &other);

    Multiset  operator-(const Multiset &other) const;
    Multiset &operator-=(const Multiset &other);
    Multiset &operator-=(const Element &elem);

    Multiset power_set() const;

    bool operator==(const Multiset &other) const;
    bool operator!=(const Multiset &other) const;

    std::string to_string() const;

    static Multiset parse(const std::string &str);
    static Element  parse_Element(const std::string &str);   // прокси к Set::parse_Element

    friend std::ostream &operator<<(std::ostream &out, const Multiset &s);
    friend std::istream &operator>>(std::istream &in, Multiset &s);
};