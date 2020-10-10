/*
 * almost_equal_to.cpp
 *
 *  Created on: Oct 10, 2020
 *      Author: bflynt
 */

// Let Catch provide main():
#define CATCH_CONFIG_MAIN
#include "../../test/catch.hpp"

#include "xstd/detail/functional/almost_equal_to.hpp"


#include <cstdint>
#include <limits>


using IndexTypes = std::tuple<float,double>;


TEMPLATE_LIST_TEST_CASE("Almsot Equal To", "[default]", IndexTypes) {

	const std::size_t N = 1000;
	auto test = xstd::almost_equal_to<TestType>();
	constexpr TestType eps  = std::numeric_limits<TestType>::epsilon();

	SECTION("Zero Value X"){
		TestType x = 0;
		TestType y = eps;
		REQUIRE( test(x,y) );
	}

	SECTION("Zero Value Y"){
		TestType x = eps;
		TestType y = 0;
		REQUIRE( test(x,y) );
	}

	SECTION("Diff Less X"){
		TestType x = 1000;
		TestType y = x + eps*abs(x)/2;
		REQUIRE( test(x,y) );
	}

	SECTION("Diff Less X"){
		TestType y = 1000;
		TestType x = y + eps*abs(y)/2;
		REQUIRE( test(x,y) );
	}

	SECTION("Different Values"){
		TestType x = 1000;
		TestType y = x + 2*eps*abs(x);
		REQUIRE( not test(x,y) );
	}
}



