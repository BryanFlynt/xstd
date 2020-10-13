/*
 * pretty_print.cpp
 *
 *  Created on: Oct 12, 2020
 *      Author: bflynt
 */



#include "xstd/detail/iostream/pretty_print.hpp"

#include <array>
#include <iostream>
#include <tuple>
#include <vector>
#include <utility>

int main() {
	using namespace xstd;

	std::array<double, 3> my_array;
	my_array.fill(3.1415);
	std::vector<double> my_vector(5, 3.1415);
	std::tuple<int,double,std::string> my_tuple{1, 3.1415, "my tuple"};
	std::pair<int,double> my_pair{1, 3.1415};
	std::set<int> my_set{3,4,2,1};

	double arr[5];

	std::cout << my_array  << std::endl;
	std::cout << my_vector << std::endl;
	std::cout << my_tuple  << std::endl;
	std::cout << my_pair   << std::endl;
	std::cout << my_set    << std::endl;
	std::cout << xstd::pretty_print_array(arr, 5) << std::endl;

	return 0;
}

