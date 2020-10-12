/*
 * test_profiler.cpp
 *
 *  Created on: May 23, 2019
 *      Author: bflynt
 */


#define XSTD_USE_PROFILE // Force Profiler to run
#include "xstd/detail/utility/profiler.hpp"

#include <iostream>



long fibonacci(unsigned n){
    if (n < 2) return n;
    return fibonacci(n-1) + fibonacci(n-2);
}

struct A {
	A();
	A(const A& a);
	~A();
	void do_work() const;
};

A::A(){
	PROFILE();
	fibonacci(10);
	}
A::A(const A& a){
	PROFILE();
	fibonacci(20);
}
A::~A(){
	PROFILE();
	fibonacci(30);
}

void A::do_work() const{
	PROFILE();
	fibonacci(40);
}



struct B {
	B();
	B(const B& b);
	~B();
	void do_work() const;
	A a_;
};

B::B(){
	PROFILE();
	fibonacci(15);
}
B::B(const B& a){
	PROFILE();
	fibonacci(25);
}
B::~B(){
	PROFILE();
	fibonacci(35);
}

void B::do_work() const{
	PROFILE();
	fibonacci(5);
	a_.do_work();
}

int main(){
	PROFILE();

	static const int N = 1;

	A a;
	B b;

	for(int i = 0; i < N; ++i){
		a.do_work();
		b.do_work();
	}

	PROFILE_TO_STREAM(std::cout);
	return 0;
}



