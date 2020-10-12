/*
 * maxres_steady_clock.hpp
 *
 *  Created on: Oct 12, 2020
 *      Author: bflynt
 */

#ifndef INCLUDE_XSTD_DETAIL_CHRONO_MAXRES_STEADY_CLOCK_HPP_
#define INCLUDE_XSTD_DETAIL_CHRONO_MAXRES_STEADY_CLOCK_HPP_


#include <chrono>
#include <type_traits>

namespace xstd {

/**
 * @brief
 * Detect the highest resolution clock between the system and steady
 */
using maxres_sys_or_steady =
		std::conditional<
		std::chrono::steady_clock::period::den <= std::chrono::system_clock::period::den,
		std::chrono::steady_clock, std::chrono::system_clock>::type;

/**
 * @brief
 * Detect the highest resolution non-sleeping clock
 */
using maxres_nonsleep_clock =
		std::conditional<
		std::chrono::high_resolution_clock::is_steady,
		std::chrono::high_resolution_clock, maxres_sys_or_steady>::type;

} /* namespace xstd */

#endif /* INCLUDE_XSTD_DETAIL_CHRONO_MAXRES_STEADY_CLOCK_HPP_ */
