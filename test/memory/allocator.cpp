/*
 * allocator.cpp
 *
 *  Created on: Oct 8, 2020
 *      Author: bflynt
 */





#include "catch.hpp"

#include "xstd/detail/memory/allocator/allocator.hpp"

#include <cstdint>
#include <vector>


class MyClassA {
	std::int8_t  a;
	std::int16_t b;
	std::int32_t c;
	std::int64_t d;
	float        e;
	double       f;
};

class alignas(64) MyClassB {
	std::int8_t  a;
	std::int16_t b;
	std::int32_t c;
	std::int64_t d;
	float        e;
	double       f;
};


using ValueTypes = std::tuple<std::int8_t, std::int16_t, std::int32_t, std::int64_t, float, double, MyClassA, MyClassB>;

TEMPLATE_LIST_TEST_CASE("Memory Allocator", "[default]", ValueTypes) {

	using value_type = TestType;
	const std::size_t N = 1000;

	SECTION("Allocate & DeAllocate with Size") {

		xstd::allocator<value_type> a;
		auto ptr = a.allocate(N);
		REQUIRE( ptr != nullptr );

		a.deallocate(ptr,N);
	}

	SECTION("Allocate & DeAllocate No Size") {

		xstd::allocator<value_type> a;
		auto ptr = a.allocate(N);
		REQUIRE( ptr != nullptr );

		a.deallocate(ptr);
	}

	SECTION("Use as std::vector allocator") {

		std::vector<value_type, xstd::allocator<value_type>> v;
		for(auto i = 0; i < N; ++i){
			v.push_back( value_type() );
		}
		REQUIRE( v.size() == N );
		v.clear();
		REQUIRE( v.size() == 0 );
	}

}
