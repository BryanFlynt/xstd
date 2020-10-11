/*
 * allocator.hpp
 *
 *  Created on: Oct 7, 2020
 *      Author: bflynt
 */

#ifndef INCLUDE_XSTD_MEMORY_ALLOCATOR_ALLOCATOR_HPP_
#define INCLUDE_XSTD_MEMORY_ALLOCATOR_ALLOCATOR_HPP_


#include <cstddef>
#include <cstdlib>
#include <exception>
#include <limits>
#include <type_traits>


namespace xstd {

/**
 * A drop in replacement for the std::allocator<> but with statistics
 *
 * This class provided a drop in replacement for the std::allocator
 * but we can monitor allocations by inserting our own logging
 * functionality.
 *
 * @tparam T Type of object we are allocating for
 */
template<typename T>
class allocator {

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
	    static constexpr size_type default_alignment = alignof(std::max_align_t);
	    //static constexpr size_type default_alignment = __STDCPP_DEFAULT_NEW_ALIGNMENT__;
		static constexpr size_type value_size_bytes  = sizeof(value_type);
		static constexpr size_type value_alignment   = alignof(value_type);
		static constexpr auto      actual_alignment  = std::align_val_t(value_alignment);
	    static constexpr bool requeres_aligned_new   = value_alignment > default_alignment;


		//-------------------------------------------------------------------------
		// Types & Constants
		//-------------------------------------------------------------------------
public:

	    template<class U>
	    struct rebind {
	        using other = allocator<U>;
	    };

	    /**
	     * Allocate uninitialized storage for N objects
	     *
	     * @param n[in] Number of objects to allocate storage for
	     */
	    [[nodiscard]]
		constexpr pointer allocate(size_type n) {
	    	const size_type requested_size_bytes  = n * value_size_bytes;

	    	// If we are constexpr then skip remainder
	    	if(std::is_constant_evaluated()) {
	    		return static_cast<pointer>(::operator new(requested_size_bytes));
	    	}

	    	// Toss an error if we can't succeed
	        if (n > this->max_size()) {
	        	  throw std::bad_alloc();
	        }

	        // If the alignment is larger than the default then align it
	        void* p = nullptr;
	        if constexpr( requeres_aligned_new ){
	        	p = ::operator new(requested_size_bytes, actual_alignment);
	        }
	        else {
	        	p = ::operator new(requested_size_bytes);
	        }

	        // Test it worked & return
	        if( not p ){
	            throw std::bad_alloc();
	        }
	        return static_cast<pointer>(p);
	    }

	    /**
	     * Deallocates the storage referenced by the pointer
	     *
	     * This version (with the size provided) has the potential
	     * for optimizations eliminating the allocated size lookup.
	     */
	    constexpr void deallocate(T* p, size_type n){
	    	if(::std::is_constant_evaluated()) {
	    		::operator delete(static_cast<void*>(p));
	    		return;
	    	}

	    	const size_type requested_size_bytes  = n * value_size_bytes;

	    	if constexpr( requeres_aligned_new ){
	    		::operator delete(p, requested_size_bytes, actual_alignment);
	    	}
	    	else {
	    		::operator delete(p, requested_size_bytes);
	    	}
	    	return;
	    }

	    /**
	     * Deallocates the storage referenced by the pointer
	     *
	     * This version (without the size provided) requires the compiler
	     * to access additional memory and locate the allocated size.
	     */
	    constexpr void deallocate(T* p){
	    	if(::std::is_constant_evaluated()) {
	    		::operator delete(static_cast<void*>(p));
	    		return;
	    	}

	    	if constexpr( requeres_aligned_new ){
	    		::operator delete(p, actual_alignment);
	    	}
	    	else {
	    		::operator delete(p);
	    	}
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


template<class T, class U>
constexpr bool
operator==(const allocator<T>&, const allocator<U>&) noexcept {
    return true;
}

template<class T, class U, std::size_t Alignment>
constexpr bool
operator!=(const allocator<T>&, const allocator<U>&)  noexcept {
    return false;
}

} /* namespace xstd */

#endif /* INCLUDE_XSTD_MEMORY_ALLOCATOR_ALLOCATOR_HPP_ */
