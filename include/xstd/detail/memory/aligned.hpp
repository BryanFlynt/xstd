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
#include <cstdlib>      // ::aligned_alloc
#include <memory>       // std::align
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


// =========================================================================
//                            xstd::align(...)
// =========================================================================
#ifdef USE_XSTD_ALIGN

/// Align Pointer within Provided Memory
/**
 *	Takes a pointer to a pre-allocated memory buffer of space
 *	and returns the first location within that buffer aligned to
 *	the requested alignment.  If the buffer is too small
 *	(i.e. size <= space) then a nullptr is returned.
 *
 * @param[in] alignment The desired alignment
 * @param[in] size The size of the storage to be aligned
 * @param[in] ptr Pointer to contiguous storage of at least space bytes
 * @param[in] space The size of the buffer in which to operate
 *
 * @return The adjusted value of ptr, or null pointer value if the space provided is too small.
 */
void*
align(std::size_t alignment,
	  std::size_t size,
      void*& ptr,
      std::size_t& space){

	ASSERT( alignment > 0 );
    ASSERT((alignment % 2) == 0 );
    ASSERT((alignment % sizeof(void*)) == 0 );

    if( size <= space ){
        char* p = reinterpret_cast<char*>(~(alignment - 1) &
            (reinterpret_cast<std::size_t>(ptr) + alignment - 1));
        std::size_t n = space - (p - static_cast<char*>(ptr));
        if( size <= n ){
            ptr = p;
            space = n;
            return p;
        }
    }
    return nullptr;
}

else // USE_XSTD_ALIGN

inline
void*
align(std::size_t alignment,
	  std::size_t size,
      void*& ptr,
      std::size_t& space){
	return ::std::align(alignment,size,ptr,space);
}

#endif // USE_XSTD_ALIGN


// =========================================================================
//                          xstd::aligned_malloc(...)
// =========================================================================
#ifdef USE_XSTD_ALIGNED_MALLOC

template<typename ReturnType = void*>
inline
ReturnType
aligned_malloc(std::size_t alignment, std::size_t size) noexcept {
	static_assert( std::is_pointer<ReturnType>::value );
	ASSERT( alignment > 0 );
    ASSERT((alignment % 2) == 0 );
    ASSERT((alignment % sizeof(void*)) == 0 );
    using iptr = std::uintptr_t; // uint large enough to hold pointer
    void*  p1  = nullptr;        // pointer to full memory
    void** p2  = nullptr;        // pointer offset to alignment
    const iptr offset = sizeof(void*) + alignment - 1;
    p1 = malloc(size + offset);
    if( p1 ){
    	p2 = (void**)((reinterpret_cast<iptr>(p1) + offset) & ~(alignment-1));
    	p2[-1] = p1; // store the allocated memory location before aligned memory
    }
    ASSERT(is_aligned(p2,alignment));
    return static_cast<ReturnType>(p2);
}

inline
void
aligned_free(void *p) noexcept {
	if( p ){
		::std::free(static_cast<void**>(p)[-1]);
	}
}

#else // USE_XSTD_ALIGNED_MALLOC

template<typename ReturnType = void*>
inline
ReturnType
aligned_malloc(std::size_t alignment, std::size_t size){
	static_assert( std::is_pointer<ReturnType>::value );
	return static_cast<ReturnType>(::aligned_alloc(alignment,size));
}

inline
void aligned_free(void *p){
	::std::free(p);
}

#endif // USE_XSTD_ALIGNED_MALLOC


// =========================================================================
//                          xstd::aligned_delete(...)
// =========================================================================

/// Deleter to pair with aligned_malloc
/**
 *	Functor to use when aligned_alloc is used to create a
 *	aligned pointer.
 *
 * Example:
 * Using a std::unique_ptr
 * \code
 * std::unique_ptr<double, aligned_delete> pointer;
 * \endcode
 */
struct aligned_delete {
    template<typename T>
    void operator()(T* ptr) const noexcept {
        if( ptr ){
            ptr->~T();
            aligned_free(ptr);
        }
    }
};

// =========================================================================
//                        class xstd::AlignedAllocator
// =========================================================================

/// Aligned Memory Allocator
/**
 * The AlignedAllocator is used to replace the
 * std::allocator for STL algorithms when aligned memory is
 * required.
 *
 * Example:
 * Uage with std::vector
 * \code
 * std::vector<double, AlignedAllocator<double,64>> avec;
 * \endcode
 *
 * @tparam T Data type to allocate for
 * @tparam N The desired alignment
 */
template <class T, std::size_t N>
class AlignedAllocator {

public:
  using value_type      = T;
  using pointer         = T*;
  using const_pointer   = const T*;
  using reference       = T&;
  using const_reference = const T&;
  using size_type       = std::size_t;
  using difference_type = std::ptrdiff_t;
  using is_always_equal = std::true_type;
  using propagate_on_container_move_assignment = std::true_type;

  /**
   * Constructor does nothing
   */
  constexpr AlignedAllocator() noexcept {}

  /**
   * Copy constructor does nothing
   */
  constexpr AlignedAllocator( const AlignedAllocator& other ) noexcept {}

  /**
   * Copy constructor to a different type does nothing
   */
  template<class U>
  constexpr AlignedAllocator( const AlignedAllocator<U,N>& other ) noexcept {}

  /**
   * Destructor does nothing
   */
  ~AlignedAllocator() {};

  /**
   * Allocate but don't initialize num elements of type T
   */
  pointer allocate(size_type n) {
    return aligned_malloc<pointer>(N,n*sizeof(value_type));
  }

  /**
   * Deallocate storage p of deleted elements
   */
  void deallocate(pointer p, size_type /* n */) {
    aligned_free(p);
  }

  // Required for C++11 but gone in C++17
  template <class U>
  struct rebind {
    typedef AlignedAllocator<U,N> other;
  };

};

/**
 * Return all specializations are true if they have same alignment
 */
template <class T1, std::size_t N1, class T2, std::size_t N2>
bool operator==(const AlignedAllocator<T1,N1>&,
                const AlignedAllocator<T2,N2>&) throw() {
  return N1==N2;
}
template <class T1, std::size_t N1, class T2, std::size_t N2>
bool operator!=(const AlignedAllocator<T1,N1>&,
                const AlignedAllocator<T2,N2>&) throw() {
  return N1!=N2;
}


} // end namespace xstd


#endif /* INCLUDE_XSTD_MEMORY_ALIGNED_HPP_ */
