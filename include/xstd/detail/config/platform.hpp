/*
 * platform.hpp
 *
 *  Created on: May 25, 2020
 *      Author: bflynt
 */

#ifndef INCLUDE_XSTD_CONFIG_PLATFORM_HPP_
#define INCLUDE_XSTD_CONFIG_PLATFORM_HPP_

/**
 * \file
 * platform.hpp
 *
 * \brief
 * Define platform specific flags
 *
 * \details
 * Attempts to determine the platform (Linux, Windows, etc.)
 * we are compiling on.
 */

//
// Undefine Flags
//
#undef XSTD_PLATFORM_LINUX
#undef XSTD_PLATFORM_APPLE
#undef XSTD_PLATFORM_WIN64
#undef XSTD_PLATFORM_CRAY
#undef XSTD_PLATFORM_CYGWIN
#undef XSTD_PLATFORM_UNKNOWN

/** Detect Platform
 */
// Linux Platform
#if (defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNU__) || defined(__GLIBC__)) && !defined(_CRAYC)
#define XSTD_PLATFORM_LINUX 1

// Darwin (Mac OS X and iOS)
#elif defined(__APPLE__) || defined(__APPLE_CC__) || defined(macintosh)
#define XSTD_PLATFORM_APPLE 1

// Windows 64 bit
#elif defined(_WIN64)
#define XSTD_PLATFORM_WIN64 1

// Cray System
#elif defined(_CRAYC)
#define XSTD_PLATFORM_CRAY 1

// CYGWIN (Not Win64)
#elif defined(__CYGWIN__)
#define XSTD_PLATFORM_CYGWIN 1

#else
#define XSTD_PLATFORM_UNKNOWN 1
#warning "Unrecognized Platform: Default Values Will Used !!!"
#endif

#endif /* INCLUDE_XSTD_CONFIG_PLATFORM_HPP_ */
