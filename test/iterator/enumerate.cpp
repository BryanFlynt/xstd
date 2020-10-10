/*
 * enumerating_iterator.cpp
 *
 *  Created on: Dec 20, 2019
 *      Author: bflynt
 */



// Let Catch provide main():
#define CATCH_CONFIG_MAIN
#include "../../test/catch.hpp"


#include "xstd/detail/iterator/enumerate.hpp"

#include <cstdint>
#include <algorithm>
#include <vector>


using IndexTypes = std::tuple<std::int8_t, std::int16_t, std::int32_t, std::int64_t>;


TEMPLATE_LIST_TEST_CASE("Enumerating Iterators", "[default]", IndexTypes) {

	using Index = TestType;

	const TestType S = 3;
	const TestType N = 10;


	SECTION("Zero Starting Index"){

		std::vector<TestType> a(N,9);

		std::for_each( xstd::enumerating_iterator(a.begin()),
				       xstd::enumerating_iterator(a.end()),
					   [](auto pr){
			pr.second = pr.first;
		});

		for(TestType i = 0; i < a.size(); ++i){
			REQUIRE(i == a[i]);
		}
	}

	SECTION("Non-Zero Starting Index"){

		std::vector<TestType> a(N,9);

		std::for_each( xstd::enumerating_iterator(a.begin(),S),
				       xstd::enumerating_iterator(a.end(),S),
					   [](auto pr){
			pr.second = pr.first;
		});

		for(TestType i = 0; i < a.size(); ++i){
			REQUIRE(i+S == a[i]);
		}
	}

}

TEST_CASE("Enumerated Iterators", "[default]") {

	using Index = std::size_t;

	const std::size_t S = 3;
	const std::size_t N = 10;

	SECTION("C++17 Structured Binding from Zero", "[default]"){

		std::vector<Index> a(N);
		for(Index i = 0; i < a.size(); ++i){
			a[i] = i;
		}

		for(auto&& [i, val]: xstd::enumerate(a)){
			REQUIRE(i == val);
		}
	}

	SECTION("C++17 Structured Binding Assignment", "[default]"){

		std::vector<Index> a(N);
		for(auto&& [i, val]: xstd::enumerate(a)){
			val = i;
		}

		for(auto&& [i, val]: xstd::enumerate(a)){
			REQUIRE(i == val);
		}
	}

	SECTION("C++17 Structured Binding from Non-Zero", "[default]"){

		std::vector<Index> a(N);
		for(Index i = 0; i < a.size(); ++i){
			a[i] = i+S;
		}

		for(auto&& [i, val]: xstd::enumerate(a.begin(),a.end(),S)){
			REQUIRE(i == val);
		}
	}

	SECTION("C++17 Structured Binding from initializer_list","[default]"){

		for(auto&& [i, val]: xstd::enumerate({0,1,2,3,4,5,6})){
			REQUIRE(i == val);
		}
	}

	SECTION("C++17 Structured Binding from Array", "[default]"){

		Index a[N];
		for(Index i = 0; i < N; ++i){
			a[i] = i;
		}

		for(auto&& [i, val]: xstd::enumerate(a)){
			REQUIRE(i == val);
		}
	}

}


