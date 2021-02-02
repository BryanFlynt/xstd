/*
 * radix.cpp
 *
 *  Created on: Dec 21, 2019
 *      Author: bflynt
 */


#include "catch.hpp"

#include "xstd/detail/algorithm/radix_sort.hpp"

#include <algorithm>
#include <cstdint>
#include <limits>
#include <random>
#include <vector>


using IndexTypes = std::tuple<std::int8_t, std::int16_t, std::int32_t, std::int64_t>;


TEMPLATE_LIST_TEST_CASE("Radix Sort", "[default]", IndexTypes) {

	const std::size_t N = 1000;

	SECTION("Random Vector of Positive Values"){

	    std::random_device rd;
	    std::mt19937 mte(rd());
	    std::uniform_int_distribution<TestType> dist(0, std::numeric_limits<TestType>::max());

	    // Generate Random Vector of Integers
		std::vector<TestType> a(N);
		std::generate(a.begin(), a.end(), [&](){return dist(mte);});

		// Copy Vector to second
		std::vector<TestType> b(a);

        // Sort using the STL (assumed to work)
		std::sort(a.begin(),a.end());
        
        // Sort using my Radix sort
		xstd::radix_sort(b.begin(),b.end());

        // Compare the answers are the same
		for(std::size_t i = 0; i < a.size(); ++i){
			REQUIRE(a[i] == b[i]);
		}
	}
}


