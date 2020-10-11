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

//
// Test Animal Class
//
class Animal : public xstd::intrusive_base<Animal>  {
public:
	Animal() : age_(-999){
	}
	Animal(const int value) : age_(value){
	}
	virtual ~Animal(){
	}
	virtual std::string name() const{
		return "Animal";
	}
	int age() const noexcept {
		return age_;
	}

protected:
	int age_;
};


//
// Dog is derived type to test runtime polymorphism
//
class Dog : public Animal {
public:
	Dog(const int value) : Animal(value){
	}
	std::string name() const{
		return "Dog";
	}
};

//
// Can is derived type to test runtime polymorphism
//
class Cat : public Animal {
public:
	Cat(const int value) : Animal(value){
	}
	std::string name() const{
		return "Cat";
	}
};




TEST_CASE("Intrusive Base", "[default]") {

	SECTION("No Pointers") {
		Animal a(3);
		Animal b(a);
		Animal c;

		REQUIRE( a.age() == 3 );
		REQUIRE( b.age() == 3 );
		REQUIRE( c.age() != 3 );
		REQUIRE( a.use_count() == 0 );
		REQUIRE( b.use_count() == 0 );
		REQUIRE( c.use_count() == 0 );

		c = a;
		REQUIRE( a.age() == 3 );
		REQUIRE( b.age() == 3 );
		REQUIRE( c.age() == 3 );
		REQUIRE( a.use_count() == 0 );
		REQUIRE( b.use_count() == 0 );
		REQUIRE( c.use_count() == 0 );
	}

	SECTION("Animal Pointers") {
		xstd::intrusive_ptr<Animal> a = new Animal(3);
		xstd::intrusive_ptr<Animal> b = a;
		xstd::intrusive_ptr<Animal> c = nullptr;

		REQUIRE( a->age() == 3 );
		REQUIRE( b->age() == 3 );
		REQUIRE( a.use_count() == 2 );
		REQUIRE( b.use_count() == 2 );
		REQUIRE( c.use_count() == 0 );

		c = a;
		REQUIRE( a->age() == 3 );
		REQUIRE( b->age() == 3 );
		REQUIRE( c->age() == 3 );
		REQUIRE( a.use_count() == 3 );
		REQUIRE( b.use_count() == 3 );
		REQUIRE( c.use_count() == 3 );

		a.reset();
		REQUIRE( a.use_count() == 0 );
		REQUIRE( b.use_count() == 2 );
		REQUIRE( c.use_count() == 2 );

		c.reset();
		REQUIRE( a.use_count() == 0 );
		REQUIRE( b.use_count() == 1 );
		REQUIRE( c.use_count() == 0 );

		b.reset();
		REQUIRE( a.use_count() == 0 );
		REQUIRE( b.use_count() == 0 );
		REQUIRE( c.use_count() == 0 );
	}

	SECTION("Polymorphic Pointers") {
		xstd::intrusive_ptr<Animal> a = new Dog(3);
		xstd::intrusive_ptr<Animal> b = new Cat(5);
		xstd::intrusive_ptr<Animal> c = nullptr;

		REQUIRE( a->age() == 3 );
		REQUIRE( b->age() == 5 );
		REQUIRE( a.use_count() == 1 );
		REQUIRE( b.use_count() == 1 );
		REQUIRE( c.use_count() == 0 );
		REQUIRE( a->name() == "Dog" );
		REQUIRE( b->name() == "Cat" );

		c = a;
		REQUIRE( a->age() == 3 );
		REQUIRE( b->age() == 5 );
		REQUIRE( c->age() == 3 );
		REQUIRE( a.use_count() == 2 );
		REQUIRE( b.use_count() == 1 );
		REQUIRE( c.use_count() == 2 );
		REQUIRE( a->name() == "Dog" );
		REQUIRE( b->name() == "Cat" );
		REQUIRE( c->name() == "Dog" );

		a.reset();
		REQUIRE( a.use_count() == 0 );
		REQUIRE( b.use_count() == 1 );
		REQUIRE( c.use_count() == 1 );

		c.reset();
		REQUIRE( a.use_count() == 0 );
		REQUIRE( b.use_count() == 1 );
		REQUIRE( c.use_count() == 0 );

		b.reset();
		REQUIRE( a.use_count() == 0 );
		REQUIRE( b.use_count() == 0 );
		REQUIRE( c.use_count() == 0 );
	}
}


