/*
 * dual_number.cpp
 *
 *  Created on: Feb 19, 2020
 *      Author: bflynt
 */

#include "xstd/detail/numeric/differential/dual_number.hpp"

#include <iostream>

#include "catch.hpp"

using ValueTypes = std::tuple<float, double>;

TEMPLATE_LIST_TEST_CASE("Dual Number", "[default]", ValueTypes) {
    using namespace xstd;

    using value_type = TestType;

    SECTION("Construction") {
        value_type r = 5;
        value_type i = 6;
        DualNumber<value_type> a;
        DualNumber<value_type> b(1);
        DualNumber<value_type> c(2, 3);
        DualNumber<value_type> d(r);
        DualNumber<value_type> e(r, i);

        REQUIRE(b.value() == 1);
        REQUIRE(c.value() == 2);
        REQUIRE(c.deriv() == 3);
        REQUIRE(d.value() == r);
        REQUIRE(e.value() == r);
        REQUIRE(e.deriv() == i);
    }

    SECTION("Add Op") {
        value_type r = 5;
        value_type i = 6;
        DualNumber<value_type> d1(r, i);
        DualNumber<value_type> d2(r, i);

        auto res1 = d1 + r;
        REQUIRE(res1.value() == r + r);
        REQUIRE(res1.deriv() == i);

        auto res2 = r + d2;
        REQUIRE(res2.value() == r + r);
        REQUIRE(res2.deriv() == i);

        auto res3 = d1 + d2;
        REQUIRE(res3.value() == d1.value() + d2.value());
        REQUIRE(res3.deriv() == d1.deriv() + d2.deriv());
    }

    SECTION("Math Functions") {
        value_type r = 5;
        value_type i = 1;

        DualNumber<value_type> dual(r, i);
        auto ans = exp(dual);
        REQUIRE(ans.value() == std::exp(r));
        REQUIRE(ans.deriv() == std::exp(r));
    }


}
