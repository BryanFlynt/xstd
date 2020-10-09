/*
 * radix_sort.hpp
 *
 *  Created on: Dec 4, 2019
 *      Author: bflynt
 */

#ifndef INCLUDE_XSTD_ALGORITHM_RADIX_SORT_HPP_
#define INCLUDE_XSTD_ALGORITHM_RADIX_SORT_HPP_

#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
#include <iterator>
#include <list>
#include <type_traits>



namespace xstd{


template< class RandomIt>
void radix_sort(RandomIt first, RandomIt last){
	using size_type  = std::size_t;
	using value_type = typename std::iterator_traits<RandomIt>::value_type;

	// Assert values are Integers and all values are Positive
	static_assert(std::is_integral<value_type>::value, "Integral required");
	assert( std::all_of(first,last,[](value_type val){return val >= value_type(0);}) );

	const size_type one = 1;
	const size_type ten = 10;

	// Get max number of digits in numbers
	auto max_value   = *std::max_element(first,last);
	auto max_digits  = static_cast<size_type>(std::log10(max_value)) + one;

	// Loop over each digit
	std::list<value_type> pocket[10];
	for(size_type i = 0; i < max_digits; ++i) {
		const size_type m = std::pow(ten, i+1);
		const size_type p = std::pow(ten, i);

		// Place values into bins
		auto iter = first;
		for(; iter != last; ++iter){
			auto temp = *iter % m;
			auto indx = temp / p;
			pocket[indx].push_back(*iter);
		}

		// Copy values back out
		iter = first;
		for(size_type j = 0; j < ten; ++j){
			iter = std::copy(pocket[j].cbegin(),pocket[j].cend(),iter);
			pocket[j].clear();
		}
	}
}

} /* namespace xstd */

#endif /* INCLUDE_XSTD_ALGORITHM_RADIX_SORT_HPP_ */
