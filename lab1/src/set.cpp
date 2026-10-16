#include "../include/set.hpp"

Set::Element::Element() : is_set(false) {}

Set::Element::Element(const std::string &atom_val)
    : is_set(false), atom(atom_val) {}

Set::Element::Element(const std::vector<Element> &children_val)
    : is_set(true), children(children_val) {}

bool Set::Element::is_set_Element() const
{
    return is_set;
}

const std::string &Set::Element::get_atom() const
{
    return atom;
}

const std::vector<Set::Element> &Set::Element::get_children() const
{
    return children;
}

bool Set::Element::operator==(const Element &other) const
{
    if (is_set != other.is_set)
        return false;
    if (is_set)
        return children == other.children;
    return atom == other.atom;
}

bool Set::Element::operator!=(const Element &other) const
{
    return !(*this == other);
}

bool Set::Element::operator<(const Element &other) const
{
    // атомы стоят перед вложенными множествами
    if (is_set != other.is_set)
        return is_set > other.is_set;
    if (is_set)
        return children < other.children;
    return atom < other.atom;
}

std::string Set::Element::to_string() const
{
    if (!is_set)
        return atom;
    std::string result = "{";
    for (size_t i = 0; i < children.size(); ++i)
    {
        if (i > 0)
            result += ", ";
        result += children[i].Set::Element::to_string();
    }
    result += "}";
    return result;
}

Set::Set() {}

Set::Set(const std::vector<Element> &elem)
    : Elements(elem)
{
    normalize();
}

void Set::normalize()
{
    std::sort(Elements.begin(), Elements.end());
    Elements.erase(std::unique(Elements.begin(), Elements.end()), Elements.end());
}

bool Set::empty() const
{
    return Elements.empty();
}

int Set::size() const
{
    return static_cast<int>(Elements.size());
}

void Set::clear()
{
    Elements.clear();
}

bool Set::contains(const Element &elem) const
{
    return std::binary_search(Elements.begin(), Elements.end(), elem);
}

bool Set::add(const Element &elem)
{
    if (contains(elem))
        return false;
    Elements.push_back(elem);
    normalize();
    return true;
}

bool Set::remove(const Element &elem)
{
    auto it = std::find(Elements.begin(), Elements.end(), elem);
    if (it == Elements.end())
        return false;
    Elements.erase(it);
    return true;
}

bool Set::operator[](const Element &elem) const
{
    return contains(elem);
}

Set Set::operator+(const Set &other) const
{
    Set result = *this;
    result += other;
    return result;
}

Set &Set::operator+=(const Set &other)
{
    for (const auto &e : other.Elements)
    {
        add(e);
    }
    return *this;
}

Set &Set::operator+=(const Element &elem)
{
    add(elem);
    return *this;
}

Set Set::operator*(const Set &other) const
{
    std::vector<Element> result;
    for (const auto &e : Elements)
    {
        if (other.contains(e))
        {
            result.push_back(e);
        }
    }
    return Set(result);
}

Set &Set::operator*=(const Set &other)
{
    std::vector<Element> result;
    for (const auto &e : Elements)
    {
        if (other.contains(e))
        {
            result.push_back(e);
        }
    }
    Elements = result;
    return *this;
}

Set Set::operator-(const Set &other) const
{
    std::vector<Element> result;
    for (const auto &e : Elements)
    {
        if (!other.contains(e))
        {
            result.push_back(e);
        }
    }
    return Set(result);
}

Set &Set::operator-=(const Set &other)
{
    std::vector<Element> result;
    for (const auto &e : Elements)
    {
        if (!other.contains(e))
        {
            result.push_back(e);
        }
    }
    Elements = result;
    return *this;
}

Set &Set::operator-=(const Element &elem)
{
    remove(elem);
    return *this;
}

Set Set::power_set() const
{
    Set res;
    Element empty_subset(std::vector<Element>{});
    res.Elements.push_back(empty_subset);
    size_t total_subsets = static_cast<size_t>(std::pow(2, Elements.size()));
    res.Elements.reserve(total_subsets);

    for (const auto &item : Elements)
    {
        size_t current_size = res.Elements.size();
        for (int i = 0; i < current_size; ++i)
        {
            Element new_subset = res.Elements[i];
            new_subset.children.push_back(item);
            res.Elements.push_back(new_subset);
        }
    }
    res.normalize();
    return res;
}

bool Set::operator==(const Set &other) const {
    return Elements == other.Elements;
} 

bool Set::operator!=(const Set &other) const {
    return !(*this == other);
}

std::string Set::to_string() const {
    std::string result = "{";
    for(int i = 0; i < Elements.size(); ++i){
        if(i > 0) result += ", ";
        result += Elements[i].to_string();
    }
    result += "}";
    return result;
}

Set::Element Set::parse_Element_recursive(const std::string &str, size_t &pos) {
    // Skip leading whitespace
    while (pos < str.size() && std::isspace(static_cast<unsigned char>(str[pos]))) {
        ++pos;
    }

    if (pos < str.size() && str[pos] == '{') {
        ++pos; // consume '{'
        std::vector<Element> children;

        while (true) {
            // Skip whitespace
            while (pos < str.size() && std::isspace(static_cast<unsigned char>(str[pos]))) {
                ++pos;
            }
            if (pos >= str.size()) break;
            if (str[pos] == '}') {
                ++pos; // consume '}'
                break;
            }

            Element child = parse_Element_recursive(str, pos);
            children.push_back(child);

            // Skip whitespace
            while (pos < str.size() && std::isspace(static_cast<unsigned char>(str[pos]))) {
                ++pos;
            }
            // Consume optional comma
            if (pos < str.size() && str[pos] == ',') {
                ++pos;
            }
        }
        return Element(children);
    }

    // Parse atom: read until whitespace, ',', '}', or '{'
    std::string atom;
    while (pos < str.size()
           && !std::isspace(static_cast<unsigned char>(str[pos]))
           && str[pos] != ','
           && str[pos] != '}'
           && str[pos] != '{') {
        atom += str[pos];
        ++pos;
    }
    return Element(atom);
}

Set Set::parse(const std::string &str) {
    size_t pos = 0;
    // Skip leading whitespace
    while (pos < str.size() && std::isspace(static_cast<unsigned char>(str[pos]))) {
        ++pos;
    }
    if (pos >= str.size()) {
        return Set(); // empty input -> empty set
    }
    Element elem = parse_Element_recursive(str, pos);
    if (elem.is_set_Element()) {
        return Set(elem.get_children());
    }
    // If the parsed element is an atom, wrap it in a set
    return Set(std::vector<Element>{elem});
}

Set::Element Set::parse_Element(const std::string &str) {
    size_t pos = 0;
    return parse_Element_recursive(str, pos);
}

std::istream &operator>>(std::istream &in, Set &s){
    std::string str;
    in >> str;
    s = Set::parse(str);
    return in;
}

std::ostream &operator<<(std::ostream &out, Set &s){
    out << s.to_string();
    return out;
}