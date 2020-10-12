/*
 * cpu_time.cpp
 *
 *  Created on: Oct 12, 2020
 *      Author: bflynt
 */


#include "xstd/detail/chrono/cpu_time.hpp"

#include <iostream>

int main() {

	xstd::chrono::cpu_times current;
	xstd::chrono::get_cpu_times(current);
	xstd::chrono::show_time(current, std::cout );

	return 0;
}



