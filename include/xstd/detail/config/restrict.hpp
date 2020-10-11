/*
 * restrict.hpp
 *
 *  Created on: May 25, 2020
 *      Author: bflynt
 */

#ifndef INCLUDE_XSTD_CONFIG_RESTRICT_HPP_
#define INCLUDE_XSTD_CONFIG_RESTRICT_HPP_

#include "xstd/detail/config/compiler.hpp"

/**
 * \file
 * restrict.hpp
 *
 * \brief
 * Define compiler specific restrict keyword
 *
 * \details
 * Attempts to determine/set the compiler "restrict" keyword
 */

//
// =====================================================
// XSTD Restrict if Compiler Allows
// =====================================================
//
#if !defined(XSTD_RESTRICT)

#if defined(XSTD_COMPILER_GCC)
#define XSTD_RESTRICT __restrict__

#elif defined(XSTD_COMPILER_INTEL)
#define XSTD_RESTRICT restrict

#elif defined(XSTD_COMPILER_CLANG)
#define XSTD_RESTRICT __restrict__

#elif defined(XSTD_COMPILER_MSVC)
#define XSTD_RESTRICT __restrict

#else
#define XSTD_RESTRICT
#endif

#endif // !defined(XSTD_RESTRICT)



#endif /* INCLUDE_XSTD_CONFIG_RESTRICT_HPP_ */
