/*
 * test_intrusive.cpp
 *
 *  Created on: Apr 25, 2019
 *      Author: bryan.flynt
 */

// Let Catch provide main():
#define CATCH_CONFIG_MAIN
#include "../../test/catch.hpp"

#include "xstd/detail/memory/intrusive/intrusive_base.hpp"
#include "xstd/detail/memory/intrusive/intrusive_ptr.hpp"

#include <iostream>
#include <string>


struct Animal : public xstd::intrusive_base<Animal> {

	Animal() : age(-999) {
	}
	Animal(const int value) : age(value){
	}
	virtual ~Animal(){
	}
	virtual std::string name() const{
		return "Animal";
	}

	int age;
};


struct Dog : public Animal {
	std::string name() const{
		return "Dog";
	}
};


struct Cat : public Animal {
	std::string name() const{
		return "Cat";
	}
};



TEST_CASE("Intrusive Base", "[default]") {

	SECTION("No Pointers") {
		Animal a(3);
		Animal b(a);
		Animal c;

		REQUIRE( a.age == 3 );
		REQUIRE( b.age == 3 );
		REQUIRE( c.age != 3 );
		REQUIRE( a.use_count() == 0 );
		REQUIRE( b.use_count() == 0 );
		REQUIRE( c.use_count() == 0 );

		c = a;
		REQUIRE( a.age == 3 );
		REQUIRE( b.age == 3 );
		REQUIRE( c.age == 3 );
		REQUIRE( a.use_count() == 0 );
		REQUIRE( b.use_count() == 0 );
		REQUIRE( c.use_count() == 0 );
	}

	SECTION("Animal Pointers") {
		xstd::intrusive_ptr<Animal> a = new Animal(3);
		xstd::intrusive_ptr<Animal> b = a;
		xstd::intrusive_ptr<Animal> c = nullptr;

		REQUIRE( a->age == 3 );
		REQUIRE( b->age == 3 );
		REQUIRE( c->age != 3 );
		REQUIRE( a.use_count() == 2 );
		REQUIRE( b.use_count() == 2 );
		REQUIRE( c.use_count() == 0 );

		c = a;
		REQUIRE( a->age == 3 );
		REQUIRE( b->age == 3 );
		REQUIRE( c->age == 3 );
		REQUIRE( a.use_count() == 0 );
		REQUIRE( b.use_count() == 0 );
		REQUIRE( c.use_count() == 0 );
	}


}

