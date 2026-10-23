#include "../include/set.hpp"

/**
 * @brief Конструктор по умолчанию. Создает пустой атом.
 */
Set::Element::Element() : is_set(false) {}

/**
 * @brief Конструктор атома.
 * @param atom_val Строковое значение атома.
 */
Set::Element::Element(const std::string &atom_val)
    : is_set(false), atom(atom_val) {}

/**
 * @brief Конструктор вложенного множества.
 * @param children_val Вектор дочерних элементов.
 */
Set::Element::Element(const std::vector<Element> &children_val)
    : is_set(true), children(children_val) {}

/**
 * @brief Проверяет, является ли элемент множеством.
 * @return true, если это множество; false, если атом.
 */
bool Set::Element::is_set_Element() const
{
    return is_set;
}

/**
 * @brief Возвращает строковое значение атома.
 * @return Константная ссылка на строку.
 */
const std::string &Set::Element::get_atom() const
{
    return atom;
}

/**
 * @brief Возвращает вектор дочерних элементов.
 * @return Константная ссылка на вектор детей.
 */
const std::vector<Set::Element> &Set::Element::get_children() const
{
    return children;
}

/**
 * @brief Сравнение элементов на равенство.
 * @param other Другой элемент.
 * @return true, если элементы равны.
 */
bool Set::Element::operator==(const Element &other) const
{
    if (is_set != other.is_set)
        return false;
    if (is_set)
        return children == other.children;
    return atom == other.atom;
}

/**
 * @brief Сравнение элементов на неравенство.
 * @param other Другой элемент.
 * @return true, если элементы не равны.
 */
bool Set::Element::operator!=(const Element &other) const
{
    return !(*this == other);
}

/**
 * @brief Оператор строгого порядка (для сортировки).
 * 
 * Атомы всегда стоят перед вложенными множествами.
 * @param other Другой элемент.
 * @return true, если текущий элемент меньше.
 */
bool Set::Element::operator<(const Element &other) const
{
    // атомы стоят перед вложенными множествами
    if (is_set != other.is_set)
        return is_set < other.is_set;
    if (is_set)
        return children < other.children;
    return atom < other.atom;
}

/**
 * @brief Преобразует элемент в строку.
 * @return Строковое представление (например, "a" или "{a, b}").
 */
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

/**
 * @brief Конструктор по умолчанию. Создает пустое множество.
 */
Set::Set() {}

/**
 * @brief Конструктор из вектора элементов.
 * 
 * Автоматически удаляет дубликаты и сортирует элементы.
 * @param elem Вектор элементов.
 */
Set::Set(const std::vector<Element> &elem)
    : Elements(elem)
{
    normalize();
}

/**
 * @brief Нормализует множество: сортирует элементы и удаляет дубликаты.
 */
void Set::normalize()
{
    std::sort(Elements.begin(), Elements.end());
    Elements.erase(std::unique(Elements.begin(), Elements.end()), Elements.end());
}

/**
 * @brief Проверяет, пусто ли множество.
 * @return true, если элементов нет.
 */
bool Set::empty() const
{
    return Elements.empty();
}

/**
 * @brief Возвращает количество элементов во множестве.
 * @return Размер множества.
 */
int Set::size() const
{
    return static_cast<int>(Elements.size());
}

/**
 * @brief Очищает множество.
 */
void Set::clear()
{
    Elements.clear();
}

/**
 * @brief Проверяет наличие элемента.
 * 
 * Использует бинарный поиск, так как элементы всегда отсортированы.
 * @param elem Искомый элемент.
 * @return true, если элемент есть.
 */
bool Set::contains(const Element &elem) const
{
    return std::binary_search(Elements.begin(), Elements.end(), elem);
}

/**
 * @brief Добавляет элемент.
 * @param elem Добавляемый элемент.
 * @return true, если добавлен; false, если уже был.
 */
bool Set::add(const Element &elem)
{
    if (contains(elem))
        return false;
    Elements.push_back(elem);
    normalize();
    return true;
}

/**
 * @brief Удаляет элемент.
 * @param elem Удаляемый элемент.
 * @return true, если удален; false, если не найден.
 */
bool Set::remove(const Element &elem)
{
    auto it = std::find(Elements.begin(), Elements.end(), elem);
    if (it == Elements.end())
        return false;
    Elements.erase(it);
    return true;
}

/**
 * @brief Проверка наличия элемента (аналог contains).
 * @param elem Искомый элемент.
 * @return true, если элемент есть.
 */
bool Set::operator[](const Element &elem) const
{
    return contains(elem);
}

/**
 * @brief Объединение множеств.
 * @param other Второе множество.
 * @return Новое множество.
 */
Set Set::operator+(const Set &other) const
{
    Set result = *this;
    result += other;
    return result;
}

/**
 * @brief Объединение с присваиванием.
 * @param other Второе множество.
 * @return Ссылка на текущее множество.
 */
Set &Set::operator+=(const Set &other)
{
    for (const auto &e : other.Elements)
    {
        add(e);
    }
    return *this;
}

/**
 * @brief Добавление элемента с присваиванием.
 * @param elem Элемент.
 * @return Ссылка на текущее множество.
 */
Set &Set::operator+=(const Element &elem)
{
    add(elem);
    return *this;
}

/**
 * @brief Пересечение множеств.
 * @param other Второе множество.
 * @return Новое множество.
 */
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

/**
 * @brief Пересечение с присваиванием.
 * @param other Второе множество.
 * @return Ссылка на текущее множество.
 */
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

/**
 * @brief Разность множеств.
 * @param other Второе множество.
 * @return Новое множество.
 */
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

/**
 * @brief Разность с присваиванием.
 * @param other Второе множество.
 * @return Ссылка на текущее множество.
 */
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

/**
 * @brief Удаление элемента с присваиванием.
 * @param elem Удаляемый элемент.
 * @return Ссылка на текущее множество.
 */
Set &Set::operator-=(const Element &elem)
{
    remove(elem);
    return *this;
}

/**
 * @brief Строит булеан (множество всех подмножеств).
 * 
 * Использует итеративный алгоритм добавления элементов к уже существующим подмножествам.
 * Внимание: размер результата равен 2^n, где n - размер исходного множества.
 * @return Множество, содержащее все подмножества.
 */
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
        for (size_t i = 0; i < current_size; ++i)
        {
            Element new_subset = res.Elements[i];
            new_subset.children.push_back(item);
            res.Elements.push_back(new_subset);
        }
    }
    res.normalize();
    return res;
}

/**
 * @brief Сравнение множеств на равенство.
 * @param other Другое множество.
 * @return true, если множества равны.
 */
bool Set::operator==(const Set &other) const {
    return Elements == other.Elements;
} 

/**
 * @brief Сравнение множеств на неравенство.
 * @param other Другое множество.
 * @return true, если множества различаются.
 */
bool Set::operator!=(const Set &other) const {
    return !(*this == other);
}

/**
 * @brief Преобразование множества в строку.
 * @return Строка вида "{a, b}".
 */
std::string Set::to_string() const {
    std::string result = "{";
    for(size_t i = 0; i < Elements.size(); ++i){
        if(i > 0) result += ", ";
        result += Elements[i].to_string();
    }
    result += "}";
    return result;
}

/**
 * @brief Рекурсивный спуск для парсинга строки.
 * 
 * Обрабатывает пробелы, фигурные скобки и запятые.
 * @param str Входная строка.
 * @param pos Текущая позиция в строке (передается по ссылке).
 * @return Разобранный элемент.
 */
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

/**
 * @brief Парсит строку в множество.
 * 
 * Поддерживает пустые строки (возвращает пустое множество) и 
 * одиночные атомы (оборачивает их в множество).
 * @param str Входная строка.
 * @return Множество.
 */
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

/**
 * @brief Парсит строку в один элемент.
 * @param str Входная строка.
 * @return Разобранный элемент.
 */
Set::Element Set::parse_Element(const std::string &str) {
    size_t pos = 0;
    return parse_Element_recursive(str, pos);
}

/**
 * @brief Ввод множества из потока.
 * 
 * Считывает строку целиком и парсит её.
 * @param in Поток ввода.
 * @param s Множество для заполнения.
 * @return Ссылка на поток.
 */
std::istream &operator>>(std::istream &in, Set &s){
    std::string str;
    std::getline(in, str);
    if (!str.empty()) {
        s = Set::parse(str);
    }
    return in;
}

/**
 * @brief Вывод множества в поток.
 * @param out Поток вывода.
 * @param s Множество.
 * @return Ссылка на поток.
 */
std::ostream &operator<<(std::ostream &out, const Set &s){
    out << s.to_string();
    return out;
}