/*
 * test_functional.cpp
 *
 *  Created on: Dec 10, 2019
 *      Author: bflynt
 */


// Let Catch provide main():
#define CATCH_CONFIG_MAIN
#include "../../test/catch.hpp"


#include "xstd/detail/tuple/functional.hpp"

#include <tuple>


TEST_CASE("Tuple Functionals", "[default]") {
	using namespace xstd;

	SECTION("Plus"){

		auto a = std::make_tuple(-1, 3.1415, 5);
		auto b = std::make_tuple(+1, 1.1235, 3);
		auto res = a + b;

		REQUIRE( std::get<0>(res) == (std::get<0>(a)+std::get<0>(b)) );
		REQUIRE( std::get<1>(res) == (std::get<1>(a)+std::get<1>(b)) );
		REQUIRE( std::get<2>(res) == (std::get<2>(a)+std::get<2>(b)) );
	}

	SECTION("Minus"){

		auto a = std::make_tuple(-1, 3.1415, 5);
		auto b = std::make_tuple(+1, 1.1235, 3);
		auto res = a - b;

		REQUIRE( std::get<0>(res) == (std::get<0>(a)-std::get<0>(b)) );
		REQUIRE( std::get<1>(res) == (std::get<1>(a)-std::get<1>(b)) );
		REQUIRE( std::get<2>(res) == (std::get<2>(a)-std::get<2>(b)) );
	}

	SECTION("Multiply"){

		auto a = std::make_tuple(-1, 3.1415, 5);
		auto b = std::make_tuple(+1, 1.1235, 3);
		auto res = a * b;

		REQUIRE( std::get<0>(res) == (std::get<0>(a)*std::get<0>(b)) );
		REQUIRE( std::get<1>(res) == (std::get<1>(a)*std::get<1>(b)) );
		REQUIRE( std::get<2>(res) == (std::get<2>(a)*std::get<2>(b)) );
	}

	SECTION("Divide"){

		auto a = std::make_tuple(-1, 3.1415, 5);
		auto b = std::make_tuple(+1, 1.1235, 3);
		auto res = a / b;

		REQUIRE( std::get<0>(res) == (std::get<0>(a)/std::get<0>(b)) );
		REQUIRE( std::get<1>(res) == (std::get<1>(a)/std::get<1>(b)) );
		REQUIRE( std::get<2>(res) == (std::get<2>(a)/std::get<2>(b)) );
	}

	SECTION("Modulus"){

		auto a = std::make_tuple(-1, 3, 5);
		auto b = std::make_tuple(+1, 1, 3);
		auto res = a % b;

		REQUIRE( std::get<0>(res) == (std::get<0>(a)%std::get<0>(b)) );
		REQUIRE( std::get<1>(res) == (std::get<1>(a)%std::get<1>(b)) );
		REQUIRE( std::get<2>(res) == (std::get<2>(a)%std::get<2>(b)) );
	}

	SECTION("Negate"){

		auto a = std::make_tuple(-1, 3.1415, 5);
		auto b = std::make_tuple(+1, 1.1235, 3);
		auto res = -a;

		REQUIRE( std::get<0>(res) == -std::get<0>(a) );
		REQUIRE( std::get<1>(res) == -std::get<1>(a) );
		REQUIRE( std::get<2>(res) == -std::get<2>(a) );
	}

}


