/*
 * almost_equal.hpp
 *
 *  Created on: Jul 15, 2020
 *      Author: bflynt
 */

#ifndef INCLUDE_XSTD_ALGORITHM_ALMOST_EQUAL_TO_HPP_
#define INCLUDE_XSTD_ALGORITHM_ALMOST_EQUAL_TO_HPP_

#include <cmath>    // std::abs
#include <limits>

namespace xstd{

/** Almost Equal Function Object
 *
 * Function object for performing comparisons within
 * machine epsilon. If either value is 0 then the other
 * value must be less than epsilon away. Otherwise,
 * the comparison is performed using a percentage of
 * the largest value.
 */
template<typename T = void>
struct almost_equal_to {
	using result_type          = bool;
	using first_argument_type  = T;
	using second_argument_type = T;

	constexpr
	bool operator() (const T& x, const T& y) const {
		using std::abs;
		constexpr T eps  = std::numeric_limits<T>::epsilon();
		constexpr T zero = 0;
		const T abs_diff = abs(x - y);

		if( (x == zero) || (y == zero) ){
			if( abs_diff <= eps ){
				return true;
			}
		}
		else {
			if( (abs_diff <= eps*abs(x)) || (abs_diff <= eps*abs(y)) ) {
				return true;
			}
		}
		return false;
	}

};

} /* namespace xstd */


#endif /* INCLUDE_XSTD_ALGORITHM_ALMOST_EQUAL_TO_HPP_ */
