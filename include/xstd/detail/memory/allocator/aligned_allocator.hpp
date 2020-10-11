/*
 * aligned_allocator.hpp
 *
 *  Created on: Oct 7, 2020
 *      Author: bflynt
 */

#ifndef INCLUDE_XSTD_MEMORY_ALLOCATOR_ALIGNED_ALLOCATOR_HPP_
#define INCLUDE_XSTD_MEMORY_ALLOCATOR_ALIGNED_ALLOCATOR_HPP_


#include <algorithm>
#include <cstdlib>
#include <exception>
#include <limits>
#include <type_traits>


namespace xstd {


/**
 * An aligned implementation of std::allocator<>
 *
 * Since the concept is primarily used for SSE, AVX instructions
 * the final alignment may be larger than requested but will always
 * be a multiple of the requested. This larger alignment occurs when
 * the requested alignment is less than the type naturally requires
 * as determined by alignof().
 *
 * @tparam T Type of object we are allocating for
 * @tparam Alignment Bytes of alignment all allocations should follow
 */
template<typename T, std::size_t Alignment>
class aligned_allocator {

	//-------------------------------------------------------------------------
	// Types & Constants
	//-------------------------------------------------------------------------
public:

		using value_type         = T;
		using pointer            = value_type*;
		using const_pointer      = const pointer;
		using void_pointer       = void*;
		using const_void_pointer = const void_pointer;
		using reference          = typename ::std::add_lvalue_reference<value_type>::type;
		using const_reference    = typename ::std::add_lvalue_reference<const value_type>::type;
		using size_type          = std::size_t;
		using difference_type    = std::ptrdiff_t;
		using propagate_on_container_move_assignment = std::true_type;
		using is_always_equal    = std::true_type;

private:
		static constexpr size_type value_size_bytes  = sizeof(value_type);
		static constexpr size_type value_alignment   = alignof(value_type);
		static constexpr size_type minimum_alignment = ((value_alignment-1)/Alignment + 1)*Alignment;
		static constexpr auto      actual_alignment  = std::align_val_t(minimum_alignment);

		//-------------------------------------------------------------------------
		// Types & Constants
		//-------------------------------------------------------------------------
public:

	    template<class U>
	    struct rebind {
	        using other = aligned_allocator<U, Alignment>;
	    };


	    /**
	     * Allocate uninitialized aligned storage for N objects
	     *
	     * @param n[in] Number of objects to allocate storage for
	     */
	    [[nodiscard]]
		 constexpr pointer allocate(size_type n) {
	    	const size_type requested_size_bytes = n * value_size_bytes;

	    	// If we are constexpr then skip remainder
	    	if(std::is_constant_evaluated()) {
	    		return static_cast<pointer>(::operator new(requested_size_bytes, actual_alignment));
	    	}

	    	// Toss an error if we can't succeed
	        if (n > this->max_size()) {
	        	  throw std::bad_alloc();
	        }

	        // Allocate memory aligned storage
	       	void* p = ::operator new(requested_size_bytes, actual_alignment);
	        if( not p ){
	            throw std::bad_alloc();
	        }
	        return static_cast<pointer>(p);
	    }

	    /**
	     * Deallocates the storage referenced by the pointer ptr
	     */
	    void deallocate(pointer ptr, size_type n) {
	    	::operator delete(ptr, n*value_size_bytes, actual_alignment);
	    	return;
	    }

	    /**
	     * Deallocates the storage referenced by the pointer ptr
	     */
	    void deallocate(pointer ptr) {
	    	::operator delete(ptr, actual_alignment);
	    	return;
	    }

	    /**
	     * Returns the maximum theoretically possible number of T that
	     * could be allocated
	     *
	     * For standard allocators this has been depreciated in
	     * C++17 and eliminated in C++20
	     */
	    constexpr size_type max_size() const noexcept {
	        return ::std::numeric_limits<size_type>::max() / sizeof(value_type);
	    }

};


template<class T, class U, std::size_t Alignment>
constexpr bool
operator==(const aligned_allocator<T, Alignment>&, const aligned_allocator<U, Alignment>&) noexcept {
    return true;
}

template<class T, class U, std::size_t Alignment>
constexpr bool
operator!=(const aligned_allocator<T, Alignment>&, const aligned_allocator<U, Alignment>&)  noexcept {
    return false;
}



} /* namespace xstd */


#endif /* INCLUDE_XSTD_MEMORY_ALLOCATOR_ALIGNED_ALLOCATOR_HPP_ */
