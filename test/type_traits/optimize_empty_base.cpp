/*
 * optimize_empty_base.cpp
 *
 *  Created on: Apr 14, 2020
 *      Author: bflynt
 */




#include "catch.hpp"


#include "xstd/detail/type_traits/optimize_empty_base.hpp"

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
class Parent : public xstd::optimize_empty_base<BASE> {
public:

	Parent()
		: xstd::optimize_empty_base<BASE>(xstd::empty_init_t()){
	}

	const BASE& base() const {
		return xstd::optimize_empty_base<BASE>::get();
	}

	BASE& base() {
		return xstd::optimize_empty_base<BASE>::get();
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




