/*
 * auto_timer.cpp
 *
 *  Created on: Oct 12, 2020
 *      Author: bflynt
 */



#include "xstd/detail/chrono/auto_timer.hpp"

#include <iostream>


long fibonacci(unsigned n){
    if (n < 2) return n;
    return fibonacci(n-1) + fibonacci(n-2);
}


void auto_time_fibonacci(unsigned n){
	xstd::chrono::AutoTimer a(std::cout);
	fibonacci(n);
}


int main() {
	xstd::chrono::AutoTimer a(std::cout);
	auto_time_fibonacci(35);
	return 0;
}







