/*
 * fortran_io.cpp
 *
 *  Created on: Jun 5, 2019
 *      Author: bryan.flynt
 */


// bitset operators


#include "xstd/detail/fstream/fortran_io.hpp"

#include <array>
#include <iostream>       // std::cout
#include <vector>


int main () {

	// Data
	std::vector<double>  vec_out(100, 1.2358);
	std::array<double,3> arr_out; arr_out.fill(3.1415);

	xstd::iofort mystream;

	mystream.open("test.out", xstd::iofort::ios::write);
	if( not mystream.is_open() ){
		std::cout << "ERROR: NOT Open" << std::endl;
		return -1;
	}
	mystream.write_record( std::make_pair(vec_out.size(), vec_out.data()),
			               std::make_pair(arr_out.size(), arr_out.data()) );
	mystream.close();

	std::vector<double>  vec_in( vec_out.size(), 0);
	std::array<double,3> arr_in; arr_in.fill(0);

	mystream.open("test.out", xstd::iofort::ios::read);
	if( not mystream.is_open() ){
		std::cout << "ERROR: NOT Open" << std::endl;
		return -1;
	}
	mystream.read_record( std::make_pair(vec_in.size(), vec_in.data()),
	                      std::make_pair(arr_in.size(), arr_in.data()) );
	mystream.close();


  return 0;
}
