/*
 * zip.cpp
 *
 *  Created on: Dec 21, 2019
 *      Author: bflynt
 */





// Let Catch provide main():
#define CATCH_CONFIG_MAIN
#include "../../test/catch.hpp"

#include "xstd/detail/iterator/zip.hpp"

#include <cstdint>
#include <algorithm>
#include <list>
#include <vector>


using IndexTypes = std::tuple<std::int8_t, std::int16_t, std::int32_t, std::int64_t>;


TEMPLATE_LIST_TEST_CASE("Zip Iterators", "[default]", IndexTypes) {


	const int N = 10;

	SECTION("Assignment to STL Containers"){

		std::vector<double> a;
		std::list<double>   b;
		std::vector<double> c;
		double d[N];
		for(auto i = 0; i < N; ++i){
			a.push_back(i);
			b.push_back(i);
			c.push_back(i);
			d[i] = i;
		}

		xstd::zip_iterator first(std::begin(a), std::begin(b), std::begin(c), std::begin(d));
		xstd::zip_iterator last(std::end(a), std::end(b), std::end(c),std::end(d));
		std::for_each(first,last,[](auto a){
			std::get<3>(a) += std::get<0>(a) + std::get<1>(a) + std::get<2>(a);
		});

		for(auto i = 0; i < N; ++i){
			REQUIRE( d[i] == 4*i );
		}
	}

}



TEMPLATE_LIST_TEST_CASE("Zip Range Based For Loop", "[default]", IndexTypes) {


	const int N = 10;

	SECTION("C++17 Structured Binding"){

		std::vector<double> a;
		std::list<double>   b;
		std::vector<double> c;
		double d[N];
		for(auto i = 0; i < N; ++i){
			a.push_back(i);
			b.push_back(i);
			c.push_back(i);
			d[i] = i;
		}

		for(auto&& [av, bv, cv, dv]: xstd::zip(a,b,c,d)){
			dv += av + bv + cv;
		}

		for(auto i = 0; i < N; ++i){
			REQUIRE( d[i] == 4*i );
		}
	}

}


