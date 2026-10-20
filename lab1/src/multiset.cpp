#include "../include/multiset.hpp"
#include <algorithm>
#include <cctype>

Multiset::Multiset(const std::vector<Element> &elems)
    : Elements(elems) {
    std::sort(Elements.begin(), Elements.end());
}

bool Multiset::empty() const { return Elements.empty(); }

int Multiset::size() const { return static_cast<int>(Elements.size()); }

int Multiset::multiplicity(const Element &elem) const {
    auto r = std::equal_range(Elements.begin(), Elements.end(), elem);
    return static_cast<int>(std::distance(r.first, r.second));
}

void Multiset::clear() { Elements.clear(); }

bool Multiset::contains(const Element &elem) const {
    return std::binary_search(Elements.begin(), Elements.end(), elem);
}

int Multiset::add(const Element &elem) {
    auto it = std::lower_bound(Elements.begin(), Elements.end(), elem);
    Elements.insert(it, elem);
    return multiplicity(elem);
}

int Multiset::add(const Element &elem, int n) {
    if (n <= 0) return multiplicity(elem);
    auto it = std::lower_bound(Elements.begin(), Elements.end(), elem);
    Elements.insert(it, static_cast<size_t>(n), elem);
    return multiplicity(elem);
}

bool Multiset::remove(const Element &elem) {
    auto it = std::lower_bound(Elements.begin(), Elements.end(), elem);
    if (it == Elements.end() || !(*it == elem)) return false;
    Elements.erase(it);
    return true;
}

int Multiset::remove_all(const Element &elem) {
    auto r = std::equal_range(Elements.begin(), Elements.end(), elem);
    int n = static_cast<int>(std::distance(r.first, r.second));
    Elements.erase(r.first, r.second);
    return n;
}

bool Multiset::operator[](const Element &elem) const {
    return contains(elem);
}

Multiset Multiset::operator+(const Multiset &other) const {
    std::vector<Element> r;
    r.reserve(Elements.size() + other.Elements.size());
    std::merge(Elements.begin(), Elements.end(),
               other.Elements.begin(), other.Elements.end(),
               std::back_inserter(r));
    return Multiset(r);
}

Multiset &Multiset::operator+=(const Multiset &other) {
    *this = *this + other;
    return *this;
}

Multiset &Multiset::operator+=(const Element &elem) {
    add(elem);
    return *this;
}

Multiset Multiset::operator*(const Multiset &other) const {
    std::vector<Element> r;
    size_t i = 0, j = 0;
    while (i < Elements.size() && j < other.Elements.size()) {
        if (Elements[i] < other.Elements[j]) { ++i; }
        else if (other.Elements[j] < Elements[i]) { ++j; }
        else {
            size_t i2 = i, j2 = j;
            while (i2 < Elements.size() && Elements[i2] == Elements[i]) ++i2;
            while (j2 < other.Elements.size() && other.Elements[j2] == Elements[i]) ++j2;

            int m = static_cast<int>(std::min(i2 - i, j2 - j));
            for (int k = 0; k < m; ++k) r.push_back(Elements[i]);
            i = i2; j = j2;
        }
    }
    return Multiset(r);
}

Multiset &Multiset::operator*=(const Multiset &other) {
    *this = *this * other;
    return *this;
}

Multiset Multiset::operator-(const Multiset &other) const {
    std::vector<Element> r;
    size_t i = 0, j = 0;
    while (i < Elements.size()) {
        if (j >= other.Elements.size() || Elements[i] < other.Elements[j]) {
            r.push_back(Elements[i++]);
        } else if (other.Elements[j] < Elements[i]) {
            ++j;
        } else {
            size_t i2 = i, j2 = j;
            while (i2 < Elements.size() && Elements[i2] == Elements[i]) ++i2;
            while (j2 < other.Elements.size() && other.Elements[j2] == Elements[i]) ++j2;

            int m = static_cast<int>(i2 - i) - static_cast<int>(j2 - j);
            for (int k = 0; k < m; ++k) r.push_back(Elements[i]);
            i = i2; j = j2;
        }
    }
    return Multiset(r);
}

Multiset &Multiset::operator-=(const Multiset &other) {
    *this = *this - other;
    return *this;
}

Multiset &Multiset::operator-=(const Element &elem) {
    remove(elem);
    return *this;
}

Multiset Multiset::power_set() const {
    std::vector<Element> subsets;
    subsets.push_back(Element(std::vector<Element>{}));  

    size_t i = 0;
    while (i < Elements.size()) {
        const Element &item = Elements[i];

        size_t j = i;
        while (j < Elements.size() && Elements[j] == item) ++j;
        int mult = static_cast<int>(j - i);
        
        size_t current = subsets.size();
        for (size_t k = 0; k < current; ++k) {
            std::vector<Element> base = subsets[k].get_children();
            for (int m = 1; m <= mult; ++m) {
                std::vector<Element> kids = base;
                for (int c = 0; c < m; ++c) kids.push_back(item);
                std::sort(kids.begin(), kids.end());
                subsets.push_back(Element(kids));
            }
        }

        i = j;
    }

    std::sort(subsets.begin(), subsets.end());
    return Multiset(subsets);
}

bool Multiset::operator==(const Multiset &other) const {
    return Elements == other.Elements;
}

bool Multiset::operator!=(const Multiset &other) const {
    return !(*this == other);
}

std::string Multiset::to_string() const {
    std::string result = "{";
    for (size_t i = 0; i < Elements.size(); ++i) {
        if (i > 0) result += ", ";
        result += Elements[i].to_string();
    }
    result += "}";
    return result;
}

Multiset Multiset::parse(const std::string &str) {
    Element elem = Set::parse_Element(str);
    if (elem.is_set_Element()) {
        return Multiset(elem.get_children());
    }
    return Multiset(std::vector<Element>{elem});
}

Multiset::Element Multiset::parse_Element(const std::string &str) {
    return Set::parse_Element(str);   
}

std::ostream &operator<<(std::ostream &out, const Multiset &s) {
    out << s.to_string();
    return out;
}

std::istream &operator>>(std::istream &in, Set &s){
    std::string str;
    std::getline(in, str);
    if (!str.empty()) {
        s = Set::parse(str);
    }
    return in;
}