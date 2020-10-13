/*
 * aligned.hpp
 *
 *  Created on: May 22, 2020
 *      Author: bflynt
 */

#ifndef INCLUDE_XSTD_MEMORY_ALIGNED_HPP_
#define INCLUDE_XSTD_MEMORY_ALIGNED_HPP_

#include "xstd/assert.hpp"
#include "xstd/config.hpp"

#include <cstdint>      // std::size_t, std::uintptr_t
#include <type_traits>


namespace xstd {


// =========================================================================
//                            xstd::is_aligned(...)
// =========================================================================

/** Test if pointer is aligned to byte boundary
 *
 * Function returns true if the pointer is aligned at
 * the provided bytes boundary. The cast to std::uintptr_t
 * is only guaranteed to be reversible from a void* so we can
 * either cast it or write the function to only accept void*
 * pointers.
 *
 * @param[in] ptr Pointer to test alignment for
 * @param[in] align_bytes Alignment boundary in bytes to test
 *
 * @tparam T Type of Pointer to test
 */
template<typename T>
constexpr
inline
typename std::enable_if<std::is_pointer<T>::value, bool>::type
is_aligned(T ptr, const std::size_t align_bytes) noexcept {
	const auto voided_ptr = reinterpret_cast<const void*>(ptr);
	const auto address    = reinterpret_cast<const std::uintptr_t>(voided_ptr);
	return ((address % align_bytes) == 0);
}


// =========================================================================
//                            xstd::assume_aligned(...)
// =========================================================================

/// Notify compiler this pointer is aligned
/**
 * Informs the compiler that the object ptr points to is
 * aligned to at least N. It may only make this assumption if the
 * object is accessed via the returned pointer. The behavior is
 * undefined if the input pointer is not of type T or is not aligned
 * to at least N.
 *
 * @param[in] ptr Pointer to inform compiler about
 *
 * @return The assumed aligned pointer
 */
template<std::size_t N, typename T>
constexpr
inline
T* assume_aligned(T* ptr){
	ASSERT(is_aligned(ptr,N));

#if (XSTD_GCC_VERSION >= 40700)
	ptr = static_cast<T*>(__builtin_assume_aligned(ptr,N));

#elif defined(XSTD_COMPILER_CLANG) && defined(__has_builtin)
#if __has_builtin(__builtin_assume_aligned)
	ptr = static_cast<T*>(__builtin_assume_aligned(ptr,N));
#endif

#elif defined(XSTD_COMPILER_INTEL)
	__assume_aligned(ptr,N);

#elif defined(XSTD_COMPILER_MSVC)
	__assume((reinterpret_cast<std::size_t>(ptr) & (N - 1)) == 0);

#endif
	return ptr;
}


} // end namespace xstd


#endif /* INCLUDE_XSTD_MEMORY_ALIGNED_HPP_ */
