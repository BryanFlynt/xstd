/*
 * container_check.cpp
 *
 *  Created on: Dec 20, 2019
 *      Author: bflynt
 */





#include "catch.hpp"


#include "xstd/detail/type_traits/container_checks.hpp"

#include <cstdint>
#include <algorithm>
#include <vector>


using IndexTypes = std::tuple<std::int8_t, std::int16_t, std::int32_t, std::int64_t>;


struct Dummy {
};


TEMPLATE_LIST_TEST_CASE("Has Size", "[default]", IndexTypes) {

	using vector_type = std::vector<TestType>;

	SECTION("Size Check"){
		REQUIRE( xstd::has_size_v<vector_type> );
		REQUIRE( not xstd::has_size_v<Dummy> );
	}

	SECTION("Begin Check"){
		REQUIRE( xstd::has_begin_v<vector_type> );
		REQUIRE( not xstd::has_begin_v<Dummy> );
	}

	SECTION("End Check"){
		REQUIRE( xstd::has_end_v<vector_type> );
		REQUIRE( not xstd::has_end_v<Dummy> );
	}

	SECTION("Iterator Check"){
		REQUIRE( xstd::has_iterator_v<vector_type> );
		REQUIRE( not xstd::has_iterator_v<Dummy> );
	}


}


