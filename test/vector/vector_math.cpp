/*
 * vector_math.cpp
 *
 *  Created on: Oct 12, 2020
 *      Author: bflynt
 */


// Let Catch provide main():
#define CATCH_CONFIG_MAIN
#include "../../test/catch.hpp"

#include "xstd/detail/vector/vector_math.hpp"


using ValueTypes = std::tuple<std::int32_t, std::int64_t, float, double>;



TEMPLATE_LIST_TEST_CASE("Vector Math", "[default]", ValueTypes) {
	using namespace xstd;

	using value_type = TestType;
	const std::size_t N = 3;

	SECTION("Unary Operations"){
		std::vector<value_type> a;
		std::vector<value_type> b;

		a.resize(N,+3);
		b.resize(N,-3);
		REQUIRE( -a == b );
		a.clear();
		b.clear();

		a.resize(N,+3);
		b.resize(N,+3);
		REQUIRE( +a == b );
		a.clear();
		b.clear();
	}

	SECTION("Vector / Scalar Operations"){
		value_type s;
		std::vector<value_type> a;
		std::vector<value_type> b;

		s = 1;
		a.resize(N,3);
		b.resize(N,4);
		REQUIRE( a+s == b );
		a.clear();
		b.clear();

		s = 1;
		a.resize(N,3);
		b.resize(N,4);
		REQUIRE( s+a == b );
		a.clear();
		b.clear();

		s = 1;
		a.resize(N,3);
		b.resize(N,2);
		REQUIRE( a-s == b );
		a.clear();
		b.clear();

		s = 1;
		a.resize(N,3);
		b.resize(N,-2);
		REQUIRE( s-a == b );
		a.clear();
		b.clear();

		s = 2;
		a.resize(N,3);
		b.resize(N,6);
		REQUIRE( a*s == b );
		a.clear();
		b.clear();

		s = 2;
		a.resize(N,3);
		b.resize(N,6);
		REQUIRE( s*a == b );
		a.clear();
		b.clear();

		s = 2;
		a.resize(N,6);
		b.resize(N,3);
		REQUIRE( a/s == b );
		a.clear();
		b.clear();

		s = 8;
		a.resize(N,2);
		b.resize(N,4);
		REQUIRE( s/a == b );
		a.clear();
		b.clear();
	}

	SECTION("Vector / Vector Operations"){
		std::vector<value_type> a;
		std::vector<value_type> b;
		std::vector<value_type> c;

		a.resize(N,3);
		b.resize(N,4);
		c.resize(N,7);
		REQUIRE( a+b == c );
		a.clear();
		b.clear();
		c.clear();

		a.resize(N,4);
		b.resize(N,3);
		c.resize(N,1);
		REQUIRE( a-b == c );
		a.clear();
		b.clear();
		c.clear();

		a.resize(N,2);
		b.resize(N,4);
		c.resize(N,8);
		REQUIRE( a*b == c );
		a.clear();
		b.clear();
		c.clear();

		a.resize(N,6);
		b.resize(N,2);
		c.resize(N,3);
		REQUIRE( a/b == c );
		a.clear();
		b.clear();
		c.clear();
	}

	SECTION("Linear Algebra Operations"){
		value_type s;
		std::vector<value_type> a;
		std::vector<value_type> b;
		std::vector<value_type> c;

		s = N * (2*4);
		a.resize(N,2);
		b.resize(N,4);
		REQUIRE( dot_product(a,b) == s );
		a.clear();
		b.clear();

		s = N * 2;
		a.resize(N,-2);
		REQUIRE( norm1(a) == s );
		a.clear();

		s = std::sqrt(N * (2*2));
		a.resize(N,2);
		REQUIRE( norm2(a) == s );
		a.clear();

		s = 3;
		a.resize(N,3);
		REQUIRE( norm_inf(a) == s );
		a.clear();

		a.resize(N,2);
		b.resize(N,4);
		c.resize(N,0);
		REQUIRE( cross_product(a,b) == c );
		a.clear();
		b.clear();
		c.clear();
	}

}



