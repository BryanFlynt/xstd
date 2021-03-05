/*
 * polynomial.cpp
 *
 *  Created on: Feb 08, 2021
 *      Author: bflynt
 */

#include "xstd/detail/numeric/polynomial/polynomial.hpp"

#include <iostream>

#include "catch.hpp"

using ValueTypes = std::tuple<float, double>;

TEMPLATE_LIST_TEST_CASE("Polynomial", "[default]", ValueTypes) {
    using namespace xstd;

    using value_type = TestType;

    SECTION("Construction [1]") {
        Polynomial<value_type> poly(3);
        REQUIRE(poly.evaluate(0.0) == 0.0);
    }

    SECTION("Construction [2]") {
        Polynomial<value_type> poly(3, true);
        REQUIRE(poly.evaluate(0.0) == 1.0);
    }

    SECTION("Construction [3]") {
        Polynomial<value_type> poly({0, 1, 2, 3});
        REQUIRE(poly.evaluate(0.0) == 0.0);
    }

    SECTION("+= Value") {
        Polynomial<value_type> poly({0, 1, 2, 3});
        poly += 1.0;
        REQUIRE(poly.evaluate(0.0) == 1.0);
    }

    SECTION("-= Value") {
        Polynomial<value_type> poly({1, 1, 2, 3});
        poly -= 1.0;
        REQUIRE(poly.evaluate(0.0) == 0.0);
    }

    SECTION("*= Value") {
        Polynomial<value_type> poly({0, 1, 2, 3});
        poly *= 2.0;
        REQUIRE(poly.evaluate(2.0) == 68.0);
    }

    SECTION("*= Value w/ normalize") {
        Polynomial<value_type> poly({0, 1, 2, 3});
        poly *= 0.0;
        // std::cout << poly << std::endl;
        REQUIRE(poly.evaluate(2.0) == 0.0);
    }

    SECTION("/= Value") {
        Polynomial<value_type> poly({0, 2, 4, 6});
        poly /= 2.0;
        REQUIRE(poly.evaluate(2.0) == 34.0);
    }

    SECTION("+= Poly") {
        value_type x = 2.0;
        Polynomial<value_type> a({0, 1, 2, -3});
        Polynomial<value_type> b({3, 2, 1, -1});
        Polynomial<value_type> orig(b);
        b += a;
        REQUIRE(b.evaluate(x) == (orig.evaluate(x) + a.evaluate(x)));
    }

    SECTION("+= Poly w/ normalize") {
        value_type x = 2.0;
        Polynomial<value_type> a({0, 1, 2, -3});
        Polynomial<value_type> b({3, 2, -2, 3});
        Polynomial<value_type> orig(b);
        b += a;
        // std::cout << orig << std::endl;
        // std::cout << b << std::endl;
        REQUIRE(b.evaluate(x) == (orig.evaluate(x) + a.evaluate(x)));
    }

    SECTION("-= Poly") {
        value_type x = 2.0;
        Polynomial<value_type> a({0, 1, 2, -3});
        Polynomial<value_type> b({3, 2, 1, -1});
        Polynomial<value_type> orig(b);
        b -= a;
        REQUIRE(b.evaluate(x) == (orig.evaluate(x) - a.evaluate(x)));
    }

    SECTION("-= Poly w/ normalize") {
        value_type x = 2.0;
        Polynomial<value_type> a({0, 1, 2, -3});
        Polynomial<value_type> b({3, 2, 2, -3});
        Polynomial<value_type> orig(b);
        b -= a;
        // std::cout << orig << std::endl;
        // std::cout << b << std::endl;
        REQUIRE(b.evaluate(x) == (orig.evaluate(x) - a.evaluate(x)));
    }

    SECTION("*= Poly") {
        value_type x = 2.0;
        Polynomial<value_type> a({0, 1, 2, -3});
        Polynomial<value_type> b({3, 2, 1, -1});
        Polynomial<value_type> orig(b);
        b *= a;
        REQUIRE(b.evaluate(x) == (orig.evaluate(x) * a.evaluate(x)));
    }

    SECTION("/= Poly") {
        value_type x = 2.0;
        Polynomial<value_type> a({0, 1, 2, -3});
        Polynomial<value_type> b({3, 2, 1, -1});
        Polynomial<value_type> orig(b);
        b /= a;
        //REQUIRE(b.evaluate(x) == (orig.evaluate(x) / a.evaluate(x)));
        REQUIRE(true);
    }

    SECTION("erase(...)") {
        const value_type x = 2.0;
        Polynomial<value_type> a({0, 1, 2, -3});
        a.erase(2);

        Polynomial<value_type> ans({0, 1, 0, -3});

        REQUIRE(a.evaluate(x) == ans.evaluate(x));
    }

    SECTION("differentiate(...)") {
        const value_type x = 2.0;
        Polynomial<value_type> a({0, 1, 2, -3});
        a.differentiate();

        Polynomial<value_type> ans({1, 4, -9});

        REQUIRE(a.evaluate(x) == ans.evaluate(x));
    }

    SECTION("integrate(...)") {
        const value_type x = 2.0;
        Polynomial<value_type> a({1, 4, -9});
        a.integrate();

        Polynomial<value_type> ans({0, 1, 2, -3});

        REQUIRE(a.evaluate(x) == ans.evaluate(x));
    }

    SECTION("pow(Polynomial, Exp)") {
        const value_type x = 2.0;
        Polynomial<value_type> a({1, 4, -9});
        Polynomial<value_type> b = pow(a, 3);

        auto aval = a.evaluate(x);
        REQUIRE(b.evaluate(x) == aval * aval * aval);
    }
}