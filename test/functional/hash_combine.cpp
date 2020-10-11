/*
 * hash_combine.cpp
 *
 *  Created on: Oct 10, 2020
 *      Author: bflynt
 */



// Let Catch provide main():
#define CATCH_CONFIG_MAIN
#include "../../test/catch.hpp"

#include "xstd/detail/functional/hash_combine.hpp"


#include <cstdint>
#include <limits>

//
// Create a Struct
//
struct MyStruct {
	std::int64_t key1;
	std::string  key2;
	bool key3;
};

// Make hashable
MAKE_HASHABLE(MyStruct, t.key1, t.key2, t.key3)

//
// Create a Class
//
struct MyClass {
public:
	MyClass() :
		key1(423),
		key2("hello"),
		key3(true){
	}

	std::int64_t getKey1() const {
		return key1;
	}

	std::string getKey2() const {
		return key2;
	}

	bool getKey3() const {
		return key3;
	}
private:
	std::int64_t key1;
	std::string  key2;
	bool key3;
};

// Make hashable
MAKE_HASHABLE(MyClass, t.getKey1(), t.getKey2(), t.getKey3())

//
// Type we want to test Hashing on
//
using BuiltInTypes  = std::tuple<std::int32_t,std::int64_t,float,double>;
using StructTypes   = std::tuple<MyStruct,MyClass>;


TEMPLATE_LIST_TEST_CASE("Hash Combine", "[default]", BuiltInTypes) {


	SECTION("Combine Hash"){
		std::size_t seed = 0;
		TestType a = 2;
		TestType b = 3;
		TestType c = 4;
		xstd::hash_combine(seed, a, b, c);
		REQUIRE( seed != 0 );
	}
}

TEMPLATE_LIST_TEST_CASE("Hash Combine", "[default]", StructTypes) {


	SECTION("Combine Hash"){
		std::size_t seed = 0;
		TestType a;
		TestType b;
		TestType c;
		xstd::hash_combine(seed, a, b, c);
		REQUIRE( seed != 0 );
	}
}

