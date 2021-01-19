/*
 * intersection.cpp
 *
 *  Created on: Jan 7, 2020
 *      Author: bflynt
 */

// Let Catch provide main():
#define CATCH_CONFIG_MAIN
#include "../../test/catch.hpp"

#include "xstd/detail/set/intersection.hpp"

#include <list>
#include <set>
#include <vector>


TEST_CASE("Set Intersection", "[default]") {

	SECTION("Mixed Containers"){

		std::vector<double> vec = {1, 3, 5, 7, 9, 11};
		std::list<double>   lst = {0, 2, 3, 5, 8, 11};
		std::set<double>     st = {0, 2, 3, 5, 9, 10};

		std::vector<double> ans;
		xstd::Intersection(ans,vec,lst,st);
		REQUIRE( ans.size() == 2 );
		REQUIRE( ans[0] == 3 );
		REQUIRE( ans[1] == 5 );
	}

}

