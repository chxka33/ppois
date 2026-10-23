#include <catch2/catch_test_macros.hpp>

#include <sstream>
#include <string>
#include <vector>

#include "multiset.hpp"

namespace
{
using Element = Multiset::Element;

Element atom(const std::string &value)
{
    return Element(value);
}

Multiset make(const std::vector<std::string> &values)
{
    std::vector<Element> elements;
    for (const auto &value : values)
    {
        elements.push_back(atom(value));
    }
    return Multiset(elements);
}
} // namespace

TEST_CASE("Multiset: создание", "[multiset]")
{
    SECTION("пустое")
    {
        Multiset multiset;
        CHECK(multiset.empty());
        CHECK(multiset.size() == 0);
    }
    SECTION("конструктор сохраняет повторы и сортирует")
    {
        Multiset multiset = make({"b", "a", "b"});
        CHECK(multiset.size() == 3);
        CHECK(multiset.to_string() == "{a, b, b}");
    }
    SECTION("копирование")
    {
        Multiset original = make({"a", "a"});
        Multiset copy(original);
        Multiset assigned;
        assigned = original;
        CHECK(copy == original);
        CHECK(assigned == original);
    }
}

TEST_CASE("Multiset: кратность, add, remove", "[multiset]")
{
    Multiset multiset;

    CHECK(multiset.multiplicity(atom("a")) == 0);
    CHECK_FALSE(multiset.contains(atom("a")));

    CHECK(multiset.add(atom("a")) == 1);
    CHECK(multiset.add(atom("a")) == 2);
    CHECK(multiset.contains(atom("a")));
    CHECK(multiset[atom("a")]);
    CHECK_FALSE(multiset[atom("z")]);

    SECTION("add с количеством")
    {
        CHECK(multiset.add(atom("b"), 3) == 3);
        CHECK(multiset.size() == 5);
        CHECK(multiset.add(atom("b"), 0) == 3);
        CHECK(multiset.add(atom("b"), -2) == 3);
        CHECK(multiset.size() == 5);
    }
    SECTION("remove убирает одно вхождение")
    {
        CHECK(multiset.remove(atom("a")));
        CHECK(multiset.multiplicity(atom("a")) == 1);
        CHECK(multiset.remove(atom("a")));
        CHECK_FALSE(multiset.remove(atom("a")));
        CHECK(multiset.empty());
    }
    SECTION("remove отсутствующего элемента, меньшего всех имеющихся")
    {
        CHECK_FALSE(multiset.remove(atom("0")));
    }
    SECTION("remove_all убирает все вхождения")
    {
        multiset.add(atom("b"));
        CHECK(multiset.remove_all(atom("a")) == 2);
        CHECK(multiset.remove_all(atom("a")) == 0);
        CHECK(multiset.to_string() == "{b}");
    }
    SECTION("clear")
    {
        multiset.clear();
        CHECK(multiset.empty());
    }
}

TEST_CASE("Multiset: операторы с элементами", "[multiset][operators]")
{
    Multiset multiset = make({"a"});

    multiset += atom("a");
    CHECK(multiset.multiplicity(atom("a")) == 2);

    multiset -= atom("a");
    CHECK(multiset.multiplicity(atom("a")) == 1);

    multiset -= atom("missing");
    CHECK(multiset.size() == 1);
}

TEST_CASE("Multiset: операции над мультимножествами", "[multiset][operators]")
{
    const Multiset first = make({"a", "a", "b", "c"});
    const Multiset second = make({"a", "b", "b", "d"});
    const Multiset empty;

    SECTION("сумма складывает кратности")
    {
        CHECK((first + second).to_string() == "{a, a, a, b, b, b, c, d}");
        CHECK(first + empty == first);
        Multiset accumulator = first;
        accumulator += second;
        CHECK(accumulator == first + second);
    }
    SECTION("пересечение берёт минимальную кратность")
    {
        CHECK((first * second).to_string() == "{a, b}");
        CHECK((first * empty).empty());
        Multiset accumulator = first;
        accumulator *= second;
        CHECK(accumulator.to_string() == "{a, b}");
    }
    SECTION("пересечение с непересекающимися элементами")
    {
        CHECK((make({"a", "c"}) * make({"b", "d"})).empty());
    }
    SECTION("разность вычитает кратности")
    {
        CHECK((first - second).to_string() == "{a, c}");
        CHECK(first - empty == first);
        CHECK((first - first).empty());
        Multiset accumulator = first;
        accumulator -= second;
        CHECK(accumulator.to_string() == "{a, c}");
    }
    SECTION("разность, когда вычитаемое больше по кратности")
    {
        CHECK((make({"a"}) - make({"a", "a", "b"})).empty());
    }
    SECTION("сравнение")
    {
        CHECK(first == make({"c", "b", "a", "a"}));
        CHECK(first != second);
    }
}

TEST_CASE("Multiset: булеан", "[multiset][power_set]")
{
    SECTION("пустое мультимножество")
    {
        Multiset result = Multiset().power_set();
        CHECK(result.size() == 1);
        CHECK(result.contains(Element(std::vector<Element>{})));
    }
    SECTION("{a, a, b} имеет (2+1)*(1+1) = 6 подмножеств")
    {
        Multiset result = make({"a", "a", "b"}).power_set();
        CHECK(result.size() == 6);
        CHECK(result.contains(Element(std::vector<Element>{atom("a"), atom("a")})));
        CHECK(result.contains(Element(std::vector<Element>{atom("a"), atom("b")})));
    }
}

TEST_CASE("Multiset: разбор строки", "[multiset][parse]")
{
    SECTION("множество с повторами")
    {
        Multiset multiset = Multiset::parse("{1, 1, 2}");
        CHECK(multiset.multiplicity(atom("1")) == 2);
        CHECK(multiset.size() == 3);
    }
    SECTION("одиночный атом оборачивается в мультимножество")
    {
        Multiset multiset = Multiset::parse("x");
        CHECK(multiset.size() == 1);
        CHECK(multiset.contains(atom("x")));
    }
    SECTION("parse_Element делегирует Set")
    {
        CHECK(Multiset::parse_Element("abc") == atom("abc"));
    }
}

TEST_CASE("Multiset: потоковый ввод-вывод", "[multiset][io]")
{
    SECTION("вывод")
    {
        std::ostringstream out;
        out << make({"a", "a"});
        CHECK(out.str() == "{a, a}");
    }
    SECTION("ввод")
    {
        std::istringstream in("{x, x, y}\n");
        Multiset multiset;
        in >> multiset;
        CHECK(multiset.to_string() == "{x, x, y}");
    }
    SECTION("пустая строка не меняет мультимножество")
    {
        std::istringstream in("\n");
        Multiset multiset = make({"keep"});
        in >> multiset;
        CHECK(multiset.to_string() == "{keep}");
    }
}
