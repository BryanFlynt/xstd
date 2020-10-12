/*
 * Filename:	assert.hpp
 * Author:      bflynt
 * Created:		Jun 11, 2016
 * Copyright:   2016, Bryan Flynt, All rights reserved.
 */
#ifndef STDX_ASSERT_H_
#define STDX_ASSERT_H_

/*
 * Macros for:
 * ASSERT - Assert condition evaluates to true at
 * runtime, otherwise exit program
 *
 * STATIC_ASSERT - Assert condition evaluates to true at
 * compile time.
 */

//
// Use the C standard assert
//
#if defined(XSTD_USE_CASSERT)
#include <cassert>
#define ASSERT(EXP) assert(EXP);

//
// Use the xstd assert
//
#else


#if !defined(NDEBUG)
#include "xstd/detail/config/current.hpp"
#include <cstdlib>   // std::exit(EXIT_FAILURE)
#include <iostream>  // std::cerr
#define ASSERT(EXP)                                                          \
		do {                                                                 \
			if (!(EXP)) {                                                    \
				std::cerr << std::endl;                                      \
				std::cerr << "***** Failed Assertion *****"    << std::endl; \
				std::cerr << "Failed expression: " << # EXP    << std::endl; \
				std::cerr << "File: " << XSTD_CURRENT_FILE     << std::endl; \
				std::cerr << "Func: " << XSTD_CURRENT_FUNCTION << std::endl; \
				std::cerr << "Line: " << XSTD_CURRENT_LINE     << std::endl; \
				std::cerr << std::endl;                                      \
				std::exit(EXIT_FAILURE);                                     \
			}                                                                \
		} while (0);

#else
#define ASSERT(EXP)
#endif

#endif // if defined(XSTD_USE_CASSERT)


// ------------------------------------------------------------------- //
//                           STATIC_ASSERT                             //
// ------------------------------------------------------------------- //
//
// Simply forwards to the C++ static_assert function
//
#define STATIC_ASSERT(...) static_assert(__VA_ARGS__)



#endif /* STDX_ASSERT_H_ */
