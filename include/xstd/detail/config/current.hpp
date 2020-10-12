/*
 * current.hpp
 *
 *  Created on: Jun 4, 2019
 *      Author: bryan.flynt
 */

#ifndef INCLUDE_XSTD_CURRENT_HPP_
#define INCLUDE_XSTD_CURRENT_HPP_


#include "xstd/detail/config/compiler.hpp"

//
// Current file is pretty universal
//
#define XSTD_CURRENT_FILE __FILE__

//
// Current line is pretty universal
//
#define XSTD_CURRENT_LINE __LINE__

//
// GCC, Clang
//
#if defined(XSTD_COMPILER_GCC) || defined(XSTD_COMPILER_CLANG)
#define XSTD_CURRENT_FUNCTION __PRETTY_FUNCTION__

//
// Intel, IBM, MSVC
//
#elif defined(XSTD_COMPILER_INTEL) || (XSTD_COMPILER_IBM) || defined(XSTD_COMPILER_MSVC)
#define XSTD_CURRENT_FUNCTION __FUNCTION__

//
// Some define FUNCSIG
//
#elif defined(__FUNCSIG__)
#define XSTD_CURRENT_FUNCTION __FUNCSIG__

//
// Other C++
//
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define XSTD_CURRENT_FUNCTION __func__

//
// Everything Else
//
#else
#define XSTD_CURRENT_FUNCTION "(unknown)"

#endif



#endif /* INCLUDE_XSTD_CURRENT_HPP_ */
