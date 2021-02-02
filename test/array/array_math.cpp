/*
 * array_math.cpp
 *
 *  Created on: Oct 12, 2020
 *      Author: bflynt
 */


#include "catch.hpp"

#include "xstd/detail/array/array_math.hpp"


using ValueTypes = std::tuple<std::int32_t, std::int64_t, float, double>;



TEMPLATE_LIST_TEST_CASE("Array Math", "[default]", ValueTypes) {
	using namespace xstd;

	using value_type = TestType;
	const std::size_t N = 3;

	SECTION("Unary Operations"){
		std::array<value_type,N> a;
		std::array<value_type,N> b;

		a.fill(+3);
		b.fill(-3);
		REQUIRE( -a == b );

		a.fill(+3);
		b.fill(+3);
		REQUIRE( +a == b );
	}

	SECTION("Array / Scalar Operations"){
		value_type s;
		std::array<value_type,N> a;
		std::array<value_type,N> b;

		s = 1;
		a.fill(3);
		b.fill(4);
		REQUIRE( a+s == b );

		s = 1;
		a.fill(3);
		b.fill(4);
		REQUIRE( s+a == b );

		s = 1;
		a.fill(3);
		b.fill(2);
		REQUIRE( a-s == b );

		s = 1;
		a.fill(3);
		b.fill(-2);
		REQUIRE( s-a == b );

		s = 2;
		a.fill(3);
		b.fill(6);
		REQUIRE( a*s == b );

		s = 2;
		a.fill(3);
		b.fill(6);
		REQUIRE( s*a == b );

		s = 2;
		a.fill(6);
		b.fill(3);
		REQUIRE( a/s == b );

		s = 8;
		a.fill(2);
		b.fill(4);
		REQUIRE( s/a == b );
	}

	SECTION("Array / Array Operations"){
		std::array<value_type,N> a;
		std::array<value_type,N> b;
		std::array<value_type,N> c;

		a.fill(3);
		b.fill(4);
		c.fill(7);
		REQUIRE( a+b == c );

		a.fill(4);
		b.fill(3);
		c.fill(1);
		REQUIRE( a-b == c );

		a.fill(2);
		b.fill(4);
		c.fill(8);
		REQUIRE( a*b == c );

		a.fill(6);
		b.fill(2);
		c.fill(3);
		REQUIRE( a/b == c );
	}

	SECTION("Linear Algebra Operations"){
		value_type s;
		std::array<value_type,N> a;
		std::array<value_type,N> b;
		std::array<value_type,N> c;

		s = N * (2*4);
		a.fill(2);
		b.fill(4);
		REQUIRE( dot_product(a,b) == s );

		s = N * 2;
		a.fill(-2);
		REQUIRE( norm1(a) == s );

		s = std::sqrt(N * (2*2));
		a.fill(2);
		REQUIRE( norm2(a) == s );

		s = 3;
		a.fill(3);
		REQUIRE( norm_inf(a) == s );

		a.fill(2);
		b.fill(4);
		c.fill(0);
		REQUIRE( cross_product(a,b) == c );
	}

}


