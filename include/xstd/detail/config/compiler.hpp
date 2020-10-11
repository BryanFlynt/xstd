/*
 * compiler.hpp
 *
 *  Created on: May 25, 2020
 *      Author: bflynt
 */

#ifndef INCLUDE_XSTD_CONFIG_COMPILER_HPP_
#define INCLUDE_XSTD_CONFIG_COMPILER_HPP_

/**
 * \file
 * compiler.hpp
 *
 * \brief
 * Detect Type/Brand of Compiler
 *
 * \details
 * The order is important since some compilers will
 * emulate the flags from others. Where possible a
 * second comparison is made to differentiate between
 * the real deal and fakes.
 */

//
// Undefine all compiler flags
//
#undef XSTD_COMPILER_NVCC
#undef XSTD_COMPILER_CRAY
#undef XSTD_COMPILER_INTEL
#undef XSTD_COMPILER_CLANG
#undef XSTD_COMPILER_PGI
#undef XSTD_COMPILER_GCC
#undef XSTD_COMPILER_MSVC
#undef XSTD_COMPILER_UNKNOWN

//
// Use Preprocessor Directives to Determine Compiler
//

// NVIDIA CUDA Compiler Wrapper
#if defined(__CUDACC__)
#define XSTD_COMPILER_NVCC 1

// Cray Compiler
#elif defined(_CRAYC)
#define XSTD_COMPILER_CRAY 1

// Intel Compiler
#elif defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || defined(__ECC)
#define XSTD_COMPILER_INTEL 1

// Clang C++ emulates GCC, so it has to appear early.
#elif defined __clang__ && !defined(__ibmxl__)
#define XSTD_COMPILER_CLANG 1

// Portland Group
#elif defined(__PGI)
#define XSTD_COMPILER_PGI 1

//  GNU C++:
# elif defined(__GNUC__) && !defined(__ibmxl__)
#define XSTD_COMPILER_GCC 1
#define XSTD_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

//  Microsoft Visual C++
//  Must remain the last #elif since some other vendors also #define _MSC_VER
#elif defined _MSC_VER
#define XSTD_COMPILER_MSVC 1

// Display Warning that KeyWords will be OFF
#else
#define XSTD_COMPILER_UNKNOWN 1
#warning "Unrecognized Compiler: Default Values Will Used !!!"
#endif

#endif /* INCLUDE_XSTD_CONFIG_COMPILER_HPP_ */
