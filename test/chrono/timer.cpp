/*
 * timer.cpp
 *
 *  Created on: Oct 12, 2020
 *      Author: bflynt
 */


#include "xstd/detail/chrono/timer.hpp"

#include <iostream>


long fibonacci(unsigned n){
    if (n < 2) return n;
    return fibonacci(n-1) + fibonacci(n-2);
}


int main() {

	xstd::chrono::Timer timer;
	auto answer = fibonacci(35);
	timer.display(std::cout);

	timer.stop();
	timer.resume();
	answer = fibonacci(35);
	timer.display(std::cout);

	return 0;
}





