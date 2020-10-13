/*
 * string_to.cpp
 *
 *  Created on: Oct 12, 2020
 *      Author: bflynt
 */


// Let Catch provide main():
#define CATCH_CONFIG_MAIN
#include "../../test/catch.hpp"

#include "xstd/detail/string/string_to.hpp"


TEST_CASE("String To", "[default]") {

	SECTION("Unsigned Integers"){
		std::string value = "128";
		REQUIRE( xstd::string_to(xstd::type<std::uint64_t>(), value) == 128ul );
		REQUIRE( xstd::string_to(xstd::type<std::uint32_t>(), value) == 128u );
		REQUIRE( xstd::string_to(xstd::type<std::uint16_t>(), value) == 128u );
		REQUIRE( xstd::string_to(xstd::type<std::uint8_t>(),  value) == 128u );
	}

	SECTION("Signed Integers"){
		std::string value = "-128";
		REQUIRE( xstd::string_to(xstd::type<std::int64_t>(), value) == -128l );
		REQUIRE( xstd::string_to(xstd::type<std::int32_t>(), value) == -128 );
		REQUIRE( xstd::string_to(xstd::type<std::int16_t>(), value) == -128 );
		REQUIRE( xstd::string_to(xstd::type<std::int8_t>(),  value) == -128 );
	}

	SECTION("Real"){
		std::string value = "3.1415";
		REQUIRE( xstd::string_to(xstd::type<float>(),       value) == 3.1415F );
		REQUIRE( xstd::string_to(xstd::type<double>(),      value) == 3.1415 );
		REQUIRE( xstd::string_to(xstd::type<long double>(), value) == 3.1415L );
	}

	SECTION("Boolean"){
		REQUIRE( xstd::string_to(xstd::type<bool>(), "F")     == false );
		REQUIRE( xstd::string_to(xstd::type<bool>(), "f")     == false );
		REQUIRE( xstd::string_to(xstd::type<bool>(), "False") == false );
		REQUIRE( xstd::string_to(xstd::type<bool>(), "false") == false );
		REQUIRE( xstd::string_to(xstd::type<bool>(), "FaLsE") == false );
		REQUIRE( xstd::string_to(xstd::type<bool>(), "0")     == false );

		REQUIRE( xstd::string_to(xstd::type<bool>(), "T")     == true );
		REQUIRE( xstd::string_to(xstd::type<bool>(), "t")     == true );
		REQUIRE( xstd::string_to(xstd::type<bool>(), "True")  == true );
		REQUIRE( xstd::string_to(xstd::type<bool>(), "true")  == true );
		REQUIRE( xstd::string_to(xstd::type<bool>(), "TrUe")  == true );
		REQUIRE( xstd::string_to(xstd::type<bool>(), "1")     == true );
	}
}
