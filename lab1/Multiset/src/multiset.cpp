#include "../include/multiset.hpp"
#include <algorithm>
#include <cctype>

/**
 * @brief Конструктор из вектора элементов.
 * 
 * Инициализирует мультимножество и сортирует элементы.
 * В отличие от обычного множества, дубликаты сохраняются.
 * @param elems Вектор элементов для инициализации.
 */
Multiset::Multiset(const std::vector<Element> &elems)
    : Elements(elems) {
    std::sort(Elements.begin(), Elements.end());
}

/**
 * @brief Проверяет, пусто ли мультимножество.
 * @return true, если элементов нет.
 */
bool Multiset::empty() const { return Elements.empty(); }

/**
 * @brief Возвращает общее количество элементов (с учетом повторов).
 * @return Размер мультимножества.
 */
int Multiset::size() const { return static_cast<int>(Elements.size()); }

/**
 * @brief Возвращает количество вхождений конкретного элемента.
 * 
 * Использует бинарный поиск (equal_range) для нахождения границ диапазона элемента.
 * @param elem Элемент для поиска.
 * @return Число вхождений (0, если элемента нет).
 */
int Multiset::multiplicity(const Element &elem) const {
    auto r = std::equal_range(Elements.begin(), Elements.end(), elem);
    return static_cast<int>(std::distance(r.first, r.second));
}

/**
 * @brief Очищает мультимножество.
 */
void Multiset::clear() { Elements.clear(); }

/**
 * @brief Проверяет наличие элемента (хотя бы одного).
 * @param elem Элемент для проверки.
 * @return true, если элемент есть.
 */
bool Multiset::contains(const Element &elem) const {
    return std::binary_search(Elements.begin(), Elements.end(), elem);
}

/**
 * @brief Добавляет один экземпляр элемента.
 * 
 * Вставляет элемент в отсортированный вектор на нужную позицию.
 * @param elem Добавляемый элемент.
 * @return Новое количество вхождений этого элемента.
 */
int Multiset::add(const Element &elem) {
    auto it = std::lower_bound(Elements.begin(), Elements.end(), elem);
    Elements.insert(it, elem);
    return multiplicity(elem);
}

/**
 * @brief Добавляет n экземпляров элемента.
 * @param elem Добавляемый элемент.
 * @param n Количество добавляемых копий. Если n <= 0, ничего не происходит.
 * @return Новое количество вхождений этого элемента.
 */
int Multiset::add(const Element &elem, int n) {
    if (n <= 0) return multiplicity(elem);
    auto it = std::lower_bound(Elements.begin(), Elements.end(), elem);
    Elements.insert(it, static_cast<size_t>(n), elem);
    return multiplicity(elem);
}

/**
 * @brief Удаляет один экземпляр элемента.
 * @param elem Элемент для удаления.
 * @return true, если элемент был удален; false, если его не было.
 */
bool Multiset::remove(const Element &elem) {
    auto it = std::lower_bound(Elements.begin(), Elements.end(), elem);
    if (it == Elements.end() || !(*it == elem)) return false;
    Elements.erase(it);
    return true;
}

/**
 * @brief Удаляет все экземпляры элемента.
 * @param elem Элемент для полного удаления.
 * @return Количество удаленных экземпляров.
 */
int Multiset::remove_all(const Element &elem) {
    auto r = std::equal_range(Elements.begin(), Elements.end(), elem);
    int n = static_cast<int>(std::distance(r.first, r.second));
    Elements.erase(r.first, r.second);
    return n;
}

/**
 * @brief Проверяет наличие элемента (аналог contains).
 * @param elem Элемент для проверки.
 * @return true, если элемент есть.
 */
bool Multiset::operator[](const Element &elem) const {
    return contains(elem);
}

/**
 * @brief Объединение мультимножеств.
 * 
 * Результат содержит каждый элемент в количестве максимальной кратности из двух множеств.
 * Использует алгоритм слияния (merge) для эффективного объединения.
 * @param other Второе мультимножество.
 * @return Новое мультимножество.
 */
Multiset Multiset::operator+(const Multiset &other) const {
    std::vector<Element> r;
    r.reserve(Elements.size() + other.Elements.size());
    std::merge(Elements.begin(), Elements.end(),
               other.Elements.begin(), other.Elements.end(),
               std::back_inserter(r));
    return Multiset(r);
}

/**
 * @brief Объединение с присваиванием.
 * @param other Второе мультимножество.
 * @return Ссылка на измененное текущее мультимножество.
 */
Multiset &Multiset::operator+=(const Multiset &other) {
    *this = *this + other;
    return *this;
}

/**
 * @brief Добавление одного элемента с присваиванием.
 * @param elem Элемент.
 * @return Ссылка на текущее мультимножество.
 */
Multiset &Multiset::operator+=(const Element &elem) {
    add(elem);
    return *this;
}

/**
 * @brief Пересечение мультимножеств.
 * 
 * Результат содержит каждый элемент в количестве минимальной кратности из двух множеств.
 * Проходит по двум отсортированным векторам одновременно.
 * @param other Второе мультимножество.
 * @return Новое мультимножество.
 */
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

/**
 * @brief Пересечение с присваиванием.
 * @param other Второе мультимножество.
 * @return Ссылка на текущее мультимножество.
 */
Multiset &Multiset::operator*=(const Multiset &other) {
    *this = *this * other;
    return *this;
}

/**
 * @brief Разность мультимножеств.
 * 
 * Убирает из текущего множества столько экземпляров каждого элемента, 
 * сколько их во втором множестве. Если в вычитаемом множестве больше элементов, 
 * результат для этого элемента равен 0.
 * @param other Вычитаемое мультимножество.
 * @return Новое мультимножество.
 */
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

/**
 * @brief Разность с присваиванием.
 * @param other Вычитаемое мультимножество.
 * @return Ссылка на текущее мультимножество.
 */
Multiset &Multiset::operator-=(const Multiset &other) {
    *this = *this - other;
    return *this;
}

/**
 * @brief Удаление одного элемента с присваиванием.
 * @param elem Удаляемый элемент.
 * @return Ссылка на текущее мультимножество.
 */
Multiset &Multiset::operator-=(const Element &elem) {
    remove(elem);
    return *this;
}

/**
 * @brief Строит булеан (множество всех подмножеств).
 * 
 * Учитывает кратность элементов: для элемента с кратностью m 
 * в подмножества добавляются его копии от 1 до m.
 * @return Новое мультимножество, содержащее все подмножества.
 */
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

/**
 * @brief Оператор сравнения на равенство.
 * @param other Другое мультимножество.
 * @return true, если кратности всех элементов совпадают.
 */
bool Multiset::operator==(const Multiset &other) const {
    return Elements == other.Elements;
}

/**
 * @brief Оператор сравнения на неравенство.
 * @param other Другое мультимножество.
 * @return true, если мультимножества различаются.
 */
bool Multiset::operator!=(const Multiset &other) const {
    return !(*this == other);
}

/**
 * @brief Преобразует мультимножество в строку.
 * @return Строковое представление вида "{a, a, b}".
 */
std::string Multiset::to_string() const {
    std::string result = "{";
    for (size_t i = 0; i < Elements.size(); ++i) {
        if (i > 0) result += ", ";
        result += Elements[i].to_string();
    }
    result += "}";
    return result;
}

/**
 * @brief Разбирает строку и создает мультимножество.
 * 
 * Делегирует разбор базовому классу Set, а затем извлекает 
 * дочерние элементы, сохраняя дубликаты.
 * @param str Строка (например, "{a, a, b}").
 * @return Новое мультимножество.
 */
Multiset Multiset::parse(const std::string &str) {
    Element elem = Set::parse_Element(str);
    if (elem.is_set_Element()) {
        return Multiset(elem.get_children());
    }
    return Multiset(std::vector<Element>{elem});
}

/**
 * @brief Разбирает строку в один элемент (прокси к Set::parse_Element).
 * @param str Строка для разбора.
 * @return Разобранный элемент.
 */
Multiset::Element Multiset::parse_Element(const std::string &str) {
    return Set::parse_Element(str);   
}

/**
 * @brief Вывод в поток.
 * @param out Поток вывода.
 * @param s Мультимножество.
 * @return Ссылка на поток.
 */
std::ostream &operator<<(std::ostream &out, const Multiset &s) {
    out << s.to_string();
    return out;
}

/**
 * @brief Ввод из потока.
 * @param in Поток ввода.
 * @param s Мультимножество для заполнения.
 * @return Ссылка на поток.
 */
std::istream &operator>>(std::istream &in, Multiset &s){
    std::string str;
    std::getline(in, str);
    if (!str.empty()) {
        s = Multiset::parse(str);
    }
    return in;
}