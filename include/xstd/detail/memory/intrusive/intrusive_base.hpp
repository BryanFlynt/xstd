/*
 * intrusive_base.hpp
 *
 *  Created on: Apr 25, 2019
 *      Author: bryan.flynt
 */

#ifndef INCLUDE_XSTD_DETAIL_MEMORY_INTRUSIVE_INTRUSIVE_BASE_HPP_
#define INCLUDE_XSTD_DETAIL_MEMORY_INTRUSIVE_INTRUSIVE_BASE_HPP_

#include "xstd/assert.hpp"

#include <atomic>


namespace xstd {

//
// Declaration of used classes and free functions
//
template <class Derived, typename Counter = std::atomic_uint32_t>
class intrusive_base;

template< typename Derived, typename Counter>
inline void intrusive_ptr_add_ref(intrusive_base<Derived,Counter>* const p) noexcept;

template< typename Derived, typename Counter>
inline void intrusive_ptr_release(intrusive_base<Derived,Counter>* const p) noexcept;

/// Base class to add reference counting for usage by xstd::intrusive_ptr
/**
 * This is the class that all base classes who intend to use
 * intrusive_ptr for all derived types should inherit from.
 * It adds reference counting to any class using the CRTM
 * to add functionality without a penalty of virtual methods.
 *
 * Example:
 * \code
 * class base : public xstd::intrusive_base<base>{
 *    ...
 * };
 *
 * class A : public base {
 *    ...
 * };
 * class B : public base {
 *    ...
 * };
 *
 * xstd::intrusive_ptr<base> vec[2];
 * vec[0] = new A();
 * vec[1] = new B();
 * \endcode
 *
 * \tparam Derived Class who is inheriting from this one in the CRTM pattern
 * \tparam Counter Type of integer to use as a counter (std::atomic_uint32_t)
 */
template <class Derived, typename Counter>
class intrusive_base {
public:

	/// Return the current count of references to data
    std::size_t use_count() const noexcept {
        return this->count_;
    }

protected:
    intrusive_base() noexcept: count_(0) {}
    intrusive_base(const intrusive_base&) noexcept : count_(0) {}
    intrusive_base& operator=(const intrusive_base&) noexcept { return *this; }

    friend void intrusive_ptr_add_ref<Derived,Counter>(intrusive_base<Derived, Counter>* const p) noexcept;
    friend void intrusive_ptr_release<Derived,Counter>(intrusive_base<Derived, Counter>* const p) noexcept;

private:
    using counter_type = Counter;

    mutable counter_type count_;
};

/// Increment the counter by one reference
template< typename Derived, typename Counter>
inline void intrusive_ptr_add_ref(intrusive_base<Derived,Counter>* const p) noexcept {
	ASSERT( p != nullptr );
	++(p->count_);
}

/// Decrement the counter by one reference and delete if now zero
template< typename Derived, typename Counter>
inline void intrusive_ptr_release(intrusive_base<Derived,Counter>* const p) noexcept {
	ASSERT( p != nullptr );
	--(p->count_);
    if( p->count_ == 0 ){
    	delete static_cast<Derived const*>(p);
    }
}

} /* namespace xstd */

#endif /* INCLUDE_XSTD_DETAIL_MEMORY_INTRUSIVE_INTRUSIVE_BASE_HPP_ */
