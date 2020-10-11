/*
 * intrusive_derived.hpp
 *
 *  Created on: Oct 11, 2020
 *      Author: bflynt
 */

#ifndef INCLUDE_XSTD_DETAIL_MEMORY_INTRUSIVE_INTRUSIVE_DERIVED_HPP_
#define INCLUDE_XSTD_DETAIL_MEMORY_INTRUSIVE_INTRUSIVE_DERIVED_HPP_

#include "xstd/assert.hpp"

#include <atomic>


namespace xstd {

//
// Declaration of used classes and free functions
//
template <typename Base, typename Counter = std::atomic_uint32_t>
class intrusive_derived;

template< typename Base, typename Counter>
inline void intrusive_ptr_add_ref(intrusive_derived<Base,Counter>* const p) noexcept;

template< typename Base, typename Counter>
inline void intrusive_ptr_release(intrusive_derived<Base,Counter>* const p) noexcept;



/// Add reference counting to a derived class for usage by xstd::intrusive_ptr
/**
 * This is the wrapper for derived classes who intend to use
 * intrusive_ptr for tracking.  It adds reference counting
 * to any derived class but does not require reference counting
 * to be added to a base class when intrusive_ptr will not be used.
 * The inheritance order makes it more obscure to use for runtime
 * polymorphic types as the derived types still must inherit from
 * a intrusive_derived'ed base.
 *
 * Example:
 * \code
 * class MyBaseClass {
 *    ...
 * };
 *
 * class A : public intrusive_derived<MyBaseClass> {
 *    ...
 * };
 * class B : public intrusive_derived<MyBaseClass> {
 *    ...
 * };
 *
 * using reference_counted_base = intrusive_derived<MyBaseClass>;
 *
 * xstd::intrusive_ptr<reference_counted_base> vec[2];
 * vec[0] = new A();
 * vec[1] = new B();
 *
 * \endcode
 *
 * \tparam Base Class to add reference counting to
 * \tparam Counter Type of integer to use as a counter (std::atomic_uint32_t)
 */
template <typename Base, typename Counter>
class intrusive_derived : public Base {
public:

	template<typename... Args>
	intrusive_derived(Args&... args) : Base(args...), count_(0) {}

	template<typename... Args>
	intrusive_derived(Args&&... args) : Base(std::forward<Args>(args)...), count_(0) {}

	intrusive_derived& operator=(const intrusive_derived&) noexcept { return *this; }

    std::size_t use_count() const noexcept {
        return this->count_;
    }

protected:

    friend void intrusive_ptr_add_ref<Base,Counter>(intrusive_derived<Base, Counter>* const p) noexcept;
    friend void intrusive_ptr_release<Base,Counter>(intrusive_derived<Base, Counter>* const p) noexcept;

private:
    mutable Counter count_;
};

/// Increment the counter by one reference
template< typename Base, typename Counter>
inline void intrusive_ptr_add_ref(intrusive_derived<Base,Counter>* const p) noexcept{
	ASSERT( p != nullptr );
	++(p->count_);
}

/// Decrement the counter by one reference and delete if now zero
template< typename Base, typename Counter>
inline void intrusive_ptr_release(intrusive_derived<Base,Counter>* const p) noexcept{
	ASSERT( p != nullptr );
	--(p->count_);
    if( p->count_ == 0 ){
        delete static_cast<Base const*>(p);
    }
}


} /* namespace xstd */




#endif /* INCLUDE_XSTD_DETAIL_MEMORY_INTRUSIVE_INTRUSIVE_DERIVED_HPP_ */
