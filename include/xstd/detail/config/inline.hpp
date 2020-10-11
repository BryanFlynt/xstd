/*
 * inline.hpp
 *
 *  Created on: May 25, 2020
 *      Author: bflynt
 */

#ifndef INCLUDE_XSTD_CONFIG_INLINE_HPP_
#define INCLUDE_XSTD_CONFIG_INLINE_HPP_

#include "xstd/detail/config/compiler.hpp"


/**
 * \file
 * inline.hpp
 *
 * \brief
 * Define Compiler specific "inline" keywords
 *
 * \details
 * Define macros for inline, force inline and
 * never inline flags if they exist
 */

// =====================================================
// XSTD Inline if Compiler Allows
// =====================================================
#if !defined(XSTD_INLINE)

#define XSTD_INLINE inline

#endif // !defined(XSTD_INLINE)


// =====================================================
// XSTD Force Inline if Compiler Allows
// =====================================================
#if !defined(XSTD_FORCE_INLINE)

#if (XSTD_GCC_VERSION > 30000)
#define XSTD_FORCE_INLINE inline __attribute__ ((__always_inline__))

#elif defined(XSTD_COMPILER_MSVC)
#define XSTD_FORCE_INLINE __forceinline

#else
#define XSTD_FORCE_INLINE inline
#endif

#endif // !defined(XSTD_FORCE_INLINE)


// =====================================================
// XSTD NO Inline if Compiler Allows
// =====================================================
#if !defined(XSTD_NO_INLINE)

#if (XSTD_GCC_VERSION > 30000) && defined(XSTD_COMPILER_NVCC)
#define XSTD_NO_INLINE __attribute__ ((noinline))

#elif (XSTD_GCC_VERSION > 30000)
#define XSTD_NO_INLINE __attribute__ ((__noinline__))

#elif defined(XSTD_COMPILER_MSVC)
#define XSTD_NO_INLINE __declspec(noinline)

#else
#define XSTD_NO_INLINE
#endif

#endif // !defined(XSTD_NO_INLINE)


#endif /* INCLUDE_XSTD_CONFIG_INLINE_HPP_ */
