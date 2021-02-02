/*
 * type.cpp
 *
 *  Created on: Oct 9, 2020
 *      Author: bflynt
 */





#include "catch.hpp"


#include "xstd/detail/type_traits/type.hpp"

#include <type_traits>

template<typename R>
R simple_function(xstd::type<R>, int i){
	return i + 1;
}


TEST_CASE("xstd::type", "[default]") {


	SECTION("Return Type Check"){
		using return_type    = std::size_t;
		using type_of_return = xstd::type<return_type>;
		using resulting_type = decltype(simple_function(type_of_return(),1));
		REQUIRE( std::is_same<return_type,resulting_type>::value );
	}

}
