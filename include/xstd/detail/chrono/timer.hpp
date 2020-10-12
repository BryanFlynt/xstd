/*
 * timer.hpp
 *
 *  Created on: Oct 12, 2020
 *      Author: bflynt
 */

#ifndef INCLUDE_XSTD_DETAIL_CHRONO_TIMER_HPP_
#define INCLUDE_XSTD_DETAIL_CHRONO_TIMER_HPP_

#include "xstd/detail/chrono/cpu_time.hpp"

#include <chrono>
#include <cstdint>

namespace xstd {
namespace chrono {

/// Timer class for calculating the time of things
/**
 * Timer class which calculates the real, user and system
 * times between start/stop calls.
 */
class Timer {

public:

	Timer() {
		this->start();
	}

	/**
	 * Return if the timer is currently stopped
	 */
	bool is_stopped() const noexcept {
		return is_stopped_;
	}

	/**
	 * Start Timer from Zero
	 */
	void start() noexcept {
		is_stopped_ = false;
		get_cpu_times(times_);
	}

	/**
	 * Stop Timer
	 */
	void stop() noexcept {
		if(this->is_stopped()) {
			return;
		}

		cpu_times current;
		get_cpu_times(current);
		times_ = (current - times_);
		is_stopped_ = true;
	}

	/**
	 * Resume Timer from current count
	 */
	void resume() noexcept {
		if(this->is_stopped()) {
			cpu_times current(times_);
			this->start();
			times_ -= current;
		}
	}

	/**
	 * Returns current timings but does not stop timer
	 */
	cpu_times elapsed() const noexcept {
		if(this->is_stopped()) {
			return times_;
		}
		cpu_times current;
		get_cpu_times(current);
		current -= times_;
		return current;
	}

	/**
	 * Display the current elapsed timings
	 */
	void display(std::ostream& os) noexcept {
		show_time(this->elapsed(),os);
	}

protected:
	cpu_times  times_;
	bool       is_stopped_;
};

} /* namespace chrono */
} /* namespace xstd */

#endif /* INCLUDE_XSTD_DETAIL_CHRONO_TIMER_HPP_ */
