/*
 * test_intrusive.cpp
 *
 *  Created on: Apr 25, 2019
 *      Author: bryan.flynt
 */

// Let Catch provide main():
#define CATCH_CONFIG_MAIN
#include "../../test/catch.hpp"

#include "xstd/detail/memory/aligned.hpp"

#include <cstdint>
#include <memory>

using PointerTypes = std::tuple<std::int8_t, std::int16_t, std::int32_t, std::int64_t, float, double>;


TEMPLATE_LIST_TEST_CASE("Aligned Memory", "[default]", PointerTypes) {

	const std::size_t N = 10;
	const std::size_t A = 64;

	SECTION("Aligned Allocate & Free") {

		TestType* ptr = static_cast<TestType*>( std::aligned_alloc(A, N*sizeof(TestType)) );
		REQUIRE( xstd::is_aligned(ptr,A) );
		std::free(ptr);

	}
}

TEMPLATE_LIST_TEST_CASE("Assume Aligned", "[default]", PointerTypes) {

	const std::size_t N = 10;
	const std::size_t A = 64;

	SECTION("std::vector") {

		auto ptr  = static_cast<TestType*>( std::aligned_alloc(A, N*sizeof(TestType)) );
		auto aptr = xstd::assume_aligned<A>(ptr);
		REQUIRE( xstd::is_aligned(aptr,A) );
		std::free(aptr);
	}
}
