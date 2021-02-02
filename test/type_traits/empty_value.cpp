/*
 * empty_type.cpp
 *
 *  Created on: Apr 14, 2020
 *      Author: bflynt
 */




#include "catch.hpp"


#include "xstd/detail/type_traits/empty_value.hpp"

class EmptyBase {
public:
	EmptyBase() = default;
	int doit() const {
		return 5;
	}
};

class NonEmptyBase {
public:

	NonEmptyBase() {
		value_ = 5;
	}

	int doit() const {
		return value_;
	}

private:
	int value_ = 5;
};


template<typename BASE>
class Parent : public xstd::empty_value<BASE> {
public:

	Parent()
		: xstd::empty_value<BASE>(xstd::empty_init_t()){
	}

	const BASE& base() const {
		return xstd::empty_value<BASE>::get();
	}

	BASE& base() {
		return xstd::empty_value<BASE>::get();
	}
};



TEST_CASE("empty_base", "[default]") {

	Parent<EmptyBase>    empty;
	Parent<NonEmptyBase> nonempty;

	SECTION("Size Check"){
		REQUIRE( sizeof(empty)    == 1 );
		REQUIRE( sizeof(nonempty) >  1 );
	}

	SECTION("Value Check"){
		REQUIRE( empty.base().doit()    == 5 );
		REQUIRE( nonempty.base().doit() == 5 );
	}
}




