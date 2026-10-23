#include <catch2/catch_test_macros.hpp>

#include <sstream>
#include <string>
#include <vector>

#include "set.hpp"

namespace
{
using Element = Set::Element;

Element atom(const std::string &value)
{
    return Element(value);
}

Element nested(const std::vector<Element> &children)
{
    return Element(children);
}
} // namespace

TEST_CASE("Element: конструкторы и доступ к данным", "[set][element]")
{
    SECTION("по умолчанию - пустой атом")
    {
        Element element;
        CHECK_FALSE(element.is_set_Element());
        CHECK(element.get_atom().empty());
    }
    SECTION("атом")
    {
        Element element("a");
        CHECK_FALSE(element.is_set_Element());
        CHECK(element.get_atom() == "a");
    }
    SECTION("вложенное множество")
    {
        Element element = nested({atom("a"), atom("b")});
        CHECK(element.is_set_Element());
        CHECK(element.get_children().size() == 2);
    }
}

TEST_CASE("Element: сравнение и строковое представление", "[set][element]")
{
    CHECK(atom("a") == atom("a"));
    CHECK(atom("a") != atom("b"));
    CHECK(atom("a") != nested({atom("a")}));
    CHECK(nested({atom("a")}) == nested({atom("a")}));
    CHECK(nested({atom("a")}) != nested({atom("b")}));

    CHECK(atom("a") < atom("b"));
    CHECK_FALSE(atom("b") < atom("a"));
    CHECK(nested({atom("a")}) < nested({atom("b")}));
    // атом и множество строго упорядочены в одну из сторон
    CHECK((atom("a") < nested({})) != (nested({}) < atom("a")));

    CHECK(atom("x").to_string() == "x");
    CHECK(nested({}).to_string() == "{}");
    CHECK(nested({atom("a"), nested({atom("b")})}).to_string() == "{a, {b}}");
}

TEST_CASE("Set: создание и нормализация", "[set]")
{
    SECTION("пустое множество")
    {
        Set set;
        CHECK(set.empty());
        CHECK(set.size() == 0);
    }
    SECTION("конструктор убирает дубликаты и сортирует")
    {
        Set set({atom("b"), atom("a"), atom("b")});
        CHECK(set.size() == 2);
        CHECK(set.to_string() == "{a, b}");
    }
    SECTION("копирование")
    {
        Set original({atom("a")});
        Set copy(original);
        Set assigned;
        assigned = original;
        CHECK(copy == original);
        CHECK(assigned == original);
    }
}

TEST_CASE("Set: add, remove, contains, clear", "[set]")
{
    Set set;

    CHECK(set.add(atom("a")));
    CHECK_FALSE(set.add(atom("a")));
    CHECK(set.size() == 1);
    CHECK(set.contains(atom("a")));
    CHECK(set[atom("a")]);
    CHECK_FALSE(set[atom("z")]);

    CHECK(set.remove(atom("a")));
    CHECK_FALSE(set.remove(atom("a")));
    CHECK(set.empty());

    set.add(atom("a"));
    set.add(atom("b"));
    set.clear();
    CHECK(set.empty());
}

TEST_CASE("Set: операторы с элементами", "[set][operators]")
{
    Set set({atom("a")});

    set += atom("b");
    CHECK(set.to_string() == "{a, b}");
    set += atom("b");
    CHECK(set.size() == 2);

    set -= atom("a");
    CHECK(set.to_string() == "{b}");
    set -= atom("missing");
    CHECK(set.size() == 1);
}

TEST_CASE("Set: теоретико-множественные операции", "[set][operators]")
{
    const Set first({atom("1"), atom("2"), atom("3")});
    const Set second({atom("2"), atom("3"), atom("4")});
    const Set empty;

    SECTION("объединение")
    {
        CHECK((first + second).to_string() == "{1, 2, 3, 4}");
        CHECK(first + empty == first);
        Set accumulator = first;
        accumulator += second;
        CHECK(accumulator.to_string() == "{1, 2, 3, 4}");
    }
    SECTION("пересечение")
    {
        CHECK((first * second).to_string() == "{2, 3}");
        CHECK((first * empty).empty());
        Set accumulator = first;
        accumulator *= second;
        CHECK(accumulator.to_string() == "{2, 3}");
    }
    SECTION("разность")
    {
        CHECK((first - second).to_string() == "{1}");
        CHECK(first - first == empty);
        Set accumulator = first;
        accumulator -= second;
        CHECK(accumulator.to_string() == "{1}");
    }
    SECTION("равенство")
    {
        CHECK(first == Set({atom("3"), atom("2"), atom("1")}));
        CHECK(first != second);
    }
}

TEST_CASE("Set: булеан", "[set][power_set]")
{
    SECTION("булеан пустого множества содержит только пустое множество")
    {
        Set result = Set().power_set();
        CHECK(result.size() == 1);
        CHECK(result.contains(nested({})));
    }
    SECTION("булеан из трёх элементов содержит 2^3 подмножеств")
    {
        Set result = Set({atom("a"), atom("b"), atom("c")}).power_set();
        CHECK(result.size() == 8);
        CHECK(result.contains(nested({})));
        CHECK(result.contains(nested({atom("a"), atom("c")})));
        CHECK(result.contains(nested({atom("a"), atom("b"), atom("c")})));
    }
}

TEST_CASE("Set: разбор строки", "[set][parse]")
{
    SECTION("пустая и пробельная строки дают пустое множество")
    {
        CHECK(Set::parse("").empty());
        CHECK(Set::parse("   ").empty());
    }
    SECTION("плоское множество")
    {
        CHECK(Set::parse("{1, 2, 3}").to_string() == "{1, 2, 3}");
    }
    SECTION("дубликаты и порядок нормализуются")
    {
        CHECK(Set::parse("{b, a, b}") == Set({atom("a"), atom("b")}));
    }
    SECTION("вложенные множества")
    {
        Set set = Set::parse("{a, {b, c}, {}}");
        CHECK(set.size() == 3);
        CHECK(set.contains(nested({atom("b"), atom("c")})));
        CHECK(set.contains(nested({})));
    }
    SECTION("одиночный атом оборачивается в множество")
    {
        Set set = Set::parse("x");
        CHECK(set.size() == 1);
        CHECK(set.contains(atom("x")));
    }
    SECTION("незакрытая скобка не приводит к падению")
    {
        CHECK(Set::parse("{a, b").size() == 2);
    }
    SECTION("parse_Element разбирает атом и множество")
    {
        CHECK(Set::parse_Element("abc") == atom("abc"));
        CHECK(Set::parse_Element("{a}") == nested({atom("a")}));
    }
}

TEST_CASE("Set: потоковый ввод-вывод", "[set][io]")
{
    SECTION("вывод")
    {
        std::ostringstream out;
        out << Set({atom("a"), atom("b")});
        CHECK(out.str() == "{a, b}");
    }
    SECTION("ввод")
    {
        std::istringstream in("{x, y}\n");
        Set set;
        in >> set;
        CHECK(set.to_string() == "{x, y}");
    }
    SECTION("пустая строка не меняет множество")
    {
        std::istringstream in("\n");
        Set set({atom("keep")});
        in >> set;
        CHECK(set.to_string() == "{keep}");
    }
}
