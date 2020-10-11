/*
 * test_intrusive.cpp
 *
 *  Created on: Apr 25, 2019
 *      Author: bryan.flynt
 */



#include <iostream>

#include "reference_count.hpp"
#include "intrusive_ptr.hpp"




class Entity {
public:
	Entity() : data_(0){}
	Entity(const Entity& other) : data_(other.data_){}
	Entity(Entity&& other) : data_(other.data_){other.data_=0;}

	Entity(int data) : data_(data){}
	virtual ~Entity(){}

	Entity& operator=(const Entity& other){
		if (&other != this){
			data_ = other.data_;
		}
		return *this;
	}

	Entity& operator=(Entity&& other){
		if (&other != this){
			data_ = other.data_;
			other.data_ = 0;
		}
		return *this;
	}

	int get_data() const {
		return data_;
	}

	void set_data(int value){
		data_ = value;
	}

	virtual std::string name() const{
		return "Entity";
	}

private:
	int data_;
};



class Tri : public xstd::reference_count<Entity> {
public:
	std::string name() const{
		return "Tri";
	}
};


class Quad : public xstd::reference_count<Entity> {
public:
	std::string name() const{
		return "Quad";
	}
};







int main() {

	using intrusive_entity = xstd::reference_count<Entity>;

	{
		intrusive_entity a(3);
		intrusive_entity b(a);
		intrusive_entity c;

		std::cout << "a.get_data() = " << a.get_data() << std::endl;
		std::cout << "b.get_data() = " << b.get_data() << std::endl;
		std::cout << "c.get_data() = " << c.get_data() << std::endl;
		std::cout << "a.use_count() = " << a.use_count() << std::endl;
		std::cout << "b.use_count() = " << b.use_count() << std::endl;
		std::cout << "c.use_count() = " << c.use_count() << std::endl;

		c = a;
		b.set_data(1);
		std::cout << "a.get_data() = " << a.get_data() << std::endl;
		std::cout << "b.get_data() = " << b.get_data() << std::endl;
		std::cout << "c.get_data() = " << c.get_data() << std::endl;
		std::cout << "a.use_count() = " << a.use_count() << std::endl;
		std::cout << "b.use_count() = " << b.use_count() << std::endl;
		std::cout << "c.use_count() = " << c.use_count() << std::endl;
	}

	std::cout << std::endl;
	{
		xstd::intrusive_ptr<intrusive_entity> a = new intrusive_entity(3);
		xstd::intrusive_ptr<intrusive_entity> b(a);
		xstd::intrusive_ptr<intrusive_entity> c;

		std::cout << "a.get_data() = " << a->get_data() << std::endl;
		std::cout << "b.get_data() = " << b->get_data() << std::endl;
		//std::cout << "c.get_data() = " << c->get_data() << std::endl;
		std::cout << "a.use_count() = " << a->use_count() << std::endl;
		std::cout << "b.use_count() = " << b->use_count() << std::endl;
		//std::cout << "c.use_count() = " << c->use_count() << std::endl;

		c = a;
		b->set_data(1);
		std::cout << "a.get_data() = " << a->get_data() << std::endl;
		std::cout << "b.get_data() = " << b->get_data() << std::endl;
		std::cout << "c.get_data() = " << c->get_data() << std::endl;
		std::cout << "a.use_count() = " << a->use_count() << std::endl;
		std::cout << "b.use_count() = " << b->use_count() << std::endl;
		std::cout << "c.use_count() = " << c->use_count() << std::endl;

		c->set_data(0);
		a.reset();
		b.reset();
		std::cout << "c.get_data() = " << c->get_data() << std::endl;
		std::cout << "c.use_count() = " << c->use_count() << std::endl;
		c.reset();

		std::cout << "reset a = " << a << std::endl;
		std::cout << "reset b = " << b << std::endl;
		std::cout << "reset c = " << c << std::endl;
	}

	std::cout << std::endl;
	{

		xstd::intrusive_ptr<intrusive_entity> vec[3];
		vec[0] = new Tri();
		vec[1] = new Quad();
		vec[2] = new Tri();

		std::cout << "Name 0 = " << vec[0]->name() << std::endl;
		std::cout << "Name 1 = " << vec[1]->name() << std::endl;
		std::cout << "Name 2 = " << vec[2]->name() << std::endl;
	}

	return 0;
}
