/*
 * aligned_allocator.cpp
 *
 *  Created on: Oct 8, 2020
 *      Author: bflynt
 */




// Let Catch provide main():
#define CATCH_CONFIG_MAIN
#include "../../test/catch.hpp"

#include "xstd/detail/memory/aligned.hpp"
#include "xstd/detail/memory/allocator/aligned_allocator.hpp"

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

class alignas(32) MyClassB {
	std::int8_t  a;
	std::int16_t b;
	std::int32_t c;
	std::int64_t d;
	float        e;
	double       f;
};


using ValueTypes = std::tuple<std::int8_t, std::int16_t, std::int32_t, std::int64_t, float, double, MyClassA, MyClassB>;

TEMPLATE_LIST_TEST_CASE("Aligned Memory Allocator", "[default]", ValueTypes) {

	using value_type = TestType;
	constexpr std::size_t A = 64;
	const std::size_t N = 1000;

	SECTION("Allocate & DeAllocate with Size") {

		xstd::aligned_allocator<value_type, A> a;
		auto ptr = a.allocate(N);
		REQUIRE( xstd::is_aligned(ptr, A) );
		a.deallocate(ptr,N);
	}

	SECTION("Allocate & DeAllocate No Size") {

		xstd::aligned_allocator<value_type, A> a;
		auto ptr = a.allocate(N);
		REQUIRE( xstd::is_aligned(ptr, A) );
		a.deallocate(ptr);
	}

	SECTION("Use as std::vector allocator") {

		std::vector<value_type, xstd::aligned_allocator<value_type, A>> v;
		for(auto i = 0; i < N; ++i){
			v.push_back( value_type() );
		}
		REQUIRE( v.size() == N );
		REQUIRE( xstd::is_aligned(v.data(), A) );
		v.clear();
		REQUIRE( v.size() == 0 );
	}

}


