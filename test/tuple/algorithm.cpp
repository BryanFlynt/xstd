/*
 * test_functional.cpp
 *
 *  Created on: Dec 10, 2019
 *      Author: bflynt
 */

// Let Catch provide main():
#define CATCH_CONFIG_MAIN
#include "../../test/catch.hpp"

#include "xstd/detail/tuple/algorithm.hpp"

#include <tuple>


TEST_CASE("Tuple Algorithm", "[default]") {


	SECTION("Min"){
		auto a = std::make_tuple(-1, 3.1415, 5);
		auto res = xstd::min(a);
		REQUIRE( res == -1 );
	}

	SECTION("Max"){
		auto a = std::make_tuple(-1, 3.1415, 5);
		auto res = xstd::max(a);
		REQUIRE( res == 5 );
	}

	SECTION("all_of [1 arg]"){
		auto a = std::make_tuple(-1, 3.1415, 5);
		REQUIRE(  xstd::all_of(a, [](auto val){return val < 10;}) );
		REQUIRE( !xstd::all_of(a, [](auto val){return val == -1;}) );
	}

	SECTION("all_of [2 arg]"){
		auto a = std::make_tuple(-1, 3.1415, 5);
		auto b = std::make_tuple(-1, 3.1415, 5);
		REQUIRE(  xstd::all_of(a, b, [](auto x, auto y){return x == y;}) );
		REQUIRE( !xstd::all_of(a, b, [](auto x, auto y){return x+y == 10;}) );
	}

	SECTION("any_of [1 arg]"){
		auto a = std::make_tuple(-1, 3.1415, 5);
		REQUIRE(  xstd::any_of(a, [](auto val){return val < 10;}) );
		REQUIRE( !xstd::any_of(a, [](auto val){return val == -2;}) );
	}

	SECTION("any_of [2 arg]"){
		auto a = std::make_tuple(-1, 3.1415, 5);
		auto b = std::make_tuple(-1, 3.1415, 5);
		REQUIRE(  xstd::any_of(a, b, [](auto x, auto y){return x+y == 10;}) );
		REQUIRE( !xstd::any_of(a, b, [](auto x, auto y){return x+y == 5;}) );
	}

	SECTION("none_of [1 arg]"){
		auto a = std::make_tuple(-1, 3.1415, 5);
		REQUIRE(  xstd::none_of(a, [](auto val){return val > 10;}) );
		REQUIRE( !xstd::none_of(a, [](auto val){return val == 5;}) );
	}

	SECTION("none_of [2 arg]"){
		auto a = std::make_tuple(-1, 3.1415, 5);
		auto b = std::make_tuple(-1, 3.1415, 5);
		REQUIRE(  xstd::none_of(a, b, [](auto x, auto y){return x+y > 10;}) );
		REQUIRE( !xstd::none_of(a, b, [](auto x, auto y){return x+y == 10;}) );
	}

	SECTION("for_each [1 arg]"){
		auto a = std::make_tuple(-1, 3.1415, 5);
		auto b = a;
		xstd::for_each(a, [](auto& val){++val;});
		REQUIRE( std::get<0>(a) == 1+std::get<0>(b) );
		REQUIRE( std::get<1>(a) == 1+std::get<1>(b) );
		REQUIRE( std::get<2>(a) == 1+std::get<2>(b) );
	}

	SECTION("for_each [2 arg]"){
		auto a = std::make_tuple(-1, 3.1415, 5);
		auto b = a;
		xstd::for_each(a, b, [](auto& x, auto& y){x+=y;});
		REQUIRE( std::get<0>(a) == 2*std::get<0>(b) );
		REQUIRE( std::get<1>(a) == 2*std::get<1>(b) );
		REQUIRE( std::get<2>(a) == 2*std::get<2>(b) );
	}

	SECTION("count_if [1 arg]"){
		auto a = std::make_tuple(-1, 3.1415, 5);
		REQUIRE( 1 == xstd::count_if(a, [](auto& val){return (val == 5);}) );
		REQUIRE( 2 == xstd::count_if(a, [](auto& val){return (val < 5);}) );
	}

	SECTION("count_if [2 arg]"){
		auto a = std::make_tuple(-1, 3.1415, 5);
		auto b = a;
		REQUIRE( 3 == xstd::count_if(a, b, [](auto x, auto y){return (x == y);}) );
	}

	SECTION("find_if [1 arg]"){
		auto a = std::make_tuple(-1, 3.1415, 5);
		REQUIRE( 2 == xstd::find_if(a, [](auto& val){return (val == 5);}) );
		REQUIRE( 0 == xstd::find_if(a, [](auto& val){return (val < 5);}) );
	}

	SECTION("find_if [2 arg]"){
		auto a = std::make_tuple(-1, 3.1415, 5);
		auto b = a;
		REQUIRE( 0 == xstd::find_if(a, b, [](auto x, auto y){return (x == y);}) );
	}

	SECTION("transform [1 arg]"){
		auto a = std::make_tuple(-1, 3.1415, 5);
		auto res = xstd::transform(a, [](auto val){return (val-1);});
		REQUIRE( std::get<0>(res) == std::get<0>(a)-1 );
		REQUIRE( std::get<1>(res) == std::get<1>(a)-1 );
		REQUIRE( std::get<2>(res) == std::get<2>(a)-1 );
	}

	SECTION("transform [2 arg]"){
		auto a = std::make_tuple(-1, 3.1415, 5);
		auto b = a;
		auto res = xstd::transform(a, b, [](auto x, auto y){return (x + y);});
		REQUIRE( std::get<0>(res) == 2*std::get<0>(a) );
		REQUIRE( std::get<1>(res) == 2*std::get<1>(a) );
		REQUIRE( std::get<2>(res) == 2*std::get<2>(a) );
	}

	SECTION("perform [1 arg]"){
		auto a = std::make_tuple(-1, 3.1415, 5);
		auto b = a;
		xstd::perform(a, 1, [](auto& val){val-=1;});
		REQUIRE( std::get<0>(a) == std::get<0>(b) );
		REQUIRE( std::get<1>(a) == std::get<1>(b)-1 );
		REQUIRE( std::get<2>(a) == std::get<2>(b) );
	}
}


