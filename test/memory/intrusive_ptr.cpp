/*
 * intrusive_ptr.cpp
 *
 *  Created on: Oct 11, 2020
 *      Author: bflynt
 */


// Let Catch provide main():
#define CATCH_CONFIG_MAIN
#include "../../test/catch.hpp"

#include "xstd/detail/memory/intrusive/intrusive_ptr.hpp"

//
// Test Animal Class
// The Animal class has the required function use_count()
// to return the number of reference counts.
//
class Animal {
public:
	Animal() : age_(-999), count_(0) {
	}
	Animal(const int value) : age_(value), count_(0){
	}
	virtual ~Animal(){
	}
	virtual std::string name() const{
		return "Animal";
	}
	std::size_t use_count() const noexcept {
		return count_;
	}

	int age() const noexcept {
		return age_;
	}

	friend void intrusive_ptr_add_ref(Animal* const p) noexcept;
    friend void intrusive_ptr_release(Animal* const p) noexcept;

protected:
	int count_;
	int age_;
};

//
// The Animal class requires intrusive_ptr_add_ref
// to increase the reference count.
//
inline void intrusive_ptr_add_ref(Animal* const p) noexcept{
	++(p->count_);
}

//
// The Animal class requires intrusive_ptr_release
// to decrease the reference count.
//
inline void intrusive_ptr_release(Animal* const p) noexcept{
	--(p->count_);
    if( p->count_ == 0 ){
    	delete p;
    }
}

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



TEST_CASE("Intrusive Pointer", "[default]") {

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


