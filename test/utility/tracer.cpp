/*
 * tracer.cpp
 *
 *  Created on: Oct 11, 2020
 *      Author: bflynt
 */


#define XSTD_USE_TRACER // Force Tracer to Insert
#include "xstd/detail/config/current.hpp"
#include "xstd/detail/utility/tracer.hpp"

struct A {
	A() {
		TRACER(XSTD_CURRENT_FUNCTION);
	}
	~A() {
		TRACER(XSTD_CURRENT_FUNCTION);
	}

	double get_pi() const {
		TRACER(XSTD_CURRENT_FUNCTION);
		return 3.14;
	}
};

struct B {
	B() {
		TRACER(XSTD_CURRENT_FUNCTION);
	}
	~B() {
		TRACER(XSTD_CURRENT_FUNCTION);
	}

	double get_pi2() const {
		TRACER(XSTD_CURRENT_FUNCTION);
		return a_.get_pi() + a_.get_pi();
	}

	A a_;
};



int main() {
	TRACER(XSTD_CURRENT_FUNCTION);
	B b;
	auto value = b.get_pi2();
}

