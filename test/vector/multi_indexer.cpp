/**
 * \file       multi_indexer.cpp
 * \author     Bryan Flynt
 * \date       Jan 18, 2021
 * \copyright  Copyright (C) 2021 Bryan Flynt - All Rights Reserved
 */



#include "catch.hpp"

#include "xstd/detail/vector/multi_indexer.hpp"

#include <iostream>

TEST_CASE("MultiIndexer", "[default]") {
	using namespace xstd;

	SECTION("1-D Operations"){

		const std::size_t N = 10;
		std::vector<int> a(N);

		std::size_t n = 0;
		std::generate(a.begin(), a.end(), [&n](){return n++;});


		RowMajorIndex<1> index({N});
		std::cout << " Value = " << std::size_t(index) << std::endl;

		REQUIRE( 0 == a[index] );
		REQUIRE( 1 == a[++index] );
		REQUIRE( 2 == a[++index] );
		index++;
		REQUIRE( 3 == a[index] );
		index = 4;
		REQUIRE( 4 == a[index] );

		auto i = index(5);
		REQUIRE( 5 == a[i] );

		REQUIRE( 6 == a[++index] );
		REQUIRE( 7 == a[++index] );

		REQUIRE( 8 == a[++index] );
		REQUIRE( 9 == a[++index] );
		REQUIRE( index.size() == 10 );
	}

	SECTION("2-D Operations"){

		const std::size_t R = 2;
		const std::size_t C = 4;
		RowMajorIndex<2> index({R,C});

		std::vector<int> a(index.size());

		std::size_t n = 0;
		std::generate(a.begin(), a.end(), [&n](){return n++;});


		REQUIRE( 0 == a[index] );
		REQUIRE( 1 == a[++index] );
		REQUIRE( 2 == a[++index] );
		index++;
		REQUIRE( 3 == a[index] );

		index = 4;
		REQUIRE( 4 == a[index] );
		REQUIRE( 5 == a[++index] );
		REQUIRE( 6 == a[++index] );
		REQUIRE( 7 == a[++index] );

		auto i = index(1,2);
		REQUIRE( 6 == a[i] );
		index--;
		REQUIRE( 5 == a[index] );
	}


	SECTION("3-D Operations"){

		const std::size_t R = 2;
		const std::size_t C = 4;
		const std::size_t M = 2;
		RowMajorIndex<3> index({R,C,M});

		std::vector<int> a(index.size());

		std::size_t n = 0;
		std::generate(a.begin(), a.end(), [&n](){return n++;});


		REQUIRE( 0 == a[index] );
		REQUIRE( 1 == a[++index] );
		REQUIRE( 2 == a[++index] );

		auto i = index(1,3,1);
		REQUIRE( index.size()-1 == a[i] );
	}
}

