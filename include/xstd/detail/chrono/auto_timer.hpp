/*
 * auto_timer.hpp
 *
 *  Created on: Oct 12, 2020
 *      Author: bflynt
 */

#ifndef INCLUDE_XSTD_DETAIL_CHRONO_AUTO_TIMER_HPP_
#define INCLUDE_XSTD_DETAIL_CHRONO_AUTO_TIMER_HPP_


#include "xstd/detail/chrono/timer.hpp"

#include <ostream>

namespace xstd {
namespace chrono {

/// Automatic Timer for calculating the time of functions
/**
 * Automatic Timer class which calculates the real, user and system
 * times between construction and destruction.  Intended to be a simple
 * timer for singular functions.  Not intended for complex nested
 * timings.
 *
 * \code{.cpp}
 * int my_timed_function() {
 *
 * 	  // Starts timing when constructed
 *    xstd::chrono::AutoTimer auto_timer(std::cout);
 *
 *    ... Calculations ...
 *
 *	  return 0;
 *    // Will automatically display when destructed
 * }
 * \endcode
 *
 */
class AutoTimer : public Timer {
public:

	AutoTimer(std::ostream& os) : os_(os) {
	}

	~AutoTimer() {
		this->display(os_);
	}

private:
	std::ostream& os_;
};

} /* namespace chrono */
} /* namespace xstd */




#endif /* INCLUDE_XSTD_DETAIL_CHRONO_AUTO_TIMER_HPP_ */
