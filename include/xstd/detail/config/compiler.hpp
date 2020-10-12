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
 * Where possible a second comparison is made to differentiate between
 * the real deal and fakes.
 */


//  GNU C++
#if defined(__GNUC__) && !defined(__ibmxl__)
#define XSTD_COMPILER_GCC
#define XSTD_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

// Clang C++ emulates GCC, so it has to appear early
#elif defined(__clang__) && !defined(__ibmxl__)
#define XSTD_COMPILER_CLANG
#define XSTD_CLANG_VERSION (__clang_major__ * 10000 + __clang_minor__ * 100 +  	__clang_patchlevel__)

// Intel Compiler
#elif defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || defined(__ECC)
#define XSTD_COMPILER_INTEL
#define XSTD_INTEL_VERSION __INTEL_COMPILER

// Portland Group
#elif defined(__PGI)
#define XSTD_COMPILER_PGI
#define XSTD_PGI_VERSION (__PGIC__ * 10000 + __PGIC_MINOR__ * 100 + __PGIC_PATCHLEVEL__)

// Cray Compiler
#elif defined(_CRAYC)
#define XSTD_COMPILER_CRAY 1
#define XSTD_CRAY_VERSION (_RELEASE * 100 + _RELEASE_MINOR)

// IBM Compiler (Clang Version)
#elif defined(__ibmxl__) || defined(__xlC__)
#define XSTD_COMPILER_IBM
#define XSTD_IBM_VERSION (1000000 * __ibmxl_version__ + 10000 * __ibmxl_release__ + 100 * __ibmxl_modification__ + __ibmxl_ptf_fix_level__)

//  Microsoft Visual C++
#elif defined(_MSC_VER)
#define XSTD_COMPILER_MSVC
#define XSTD_MSVC_VERSION _MSC_FULL_VER

// NVIDIA CUDA Compiler Wrapper
#elif defined(__CUDACC__)
#define XSTD_COMPILER_NVCC

// Display Warning that KeyWords will be OFF
#else
#define XSTD_COMPILER_UNKNOWN 1
#warning "Unrecognized Compiler: Default Values Will Used !!!"
#endif



#endif /* INCLUDE_XSTD_CONFIG_COMPILER_HPP_ */
