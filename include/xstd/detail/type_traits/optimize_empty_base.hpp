/*
 * optimize_empty_base.hpp
 *
 *  Created on: Apr 14, 2020
 *      Author: bflynt
 */

#pragma once


#include <type_traits> // std::is_empty, std::is_final
#include <utility>     // std::forward

namespace xstd {

/** Tests if T can be used for empty base class
 */
template<typename T>
struct is_empty_value_base {
	static constexpr bool value = std::is_empty<T>::value && !std::is_final<T>::value;
};

/** Empty type to use as flag
 */
struct empty_init_t {
};

/** Wrapper to simplify empty base optimization
 *
 * This wrapper is used to create a type which stores
 * the base type where the base may be empty and the
 * parent can take advantage of the empty base class
 * optimization.  Both non-empty and empty class use
 * the same interface which frees the programmer from
 * needing to know if the type is empty.
 * 
 * If the base type is empty the optimize_empty_base wrapper
 * will inherit from it thereby taking advantage of the 
 * empty base class optimization.
 * 
 * If the base class is not empty the optimize_empty_base 
 * wrapper will encapsulate a member of the type. 
 *
 * In both cases the interface the derived type
 * accesses the methods of the base class remain 
 * the same.
 * 
 * This frees the developer from needing to know 
 * if a base class is empty or contains data.
 *  
 *
 * Example of use:
 * \code{.cpp}
 *
 * //
 * // Parent Class Inherits from Base
 * //
 * template<typename T, typename Compare, typename Allocator>
 * class Parent
 *    : optimize_empty_base<Compare, 0>
 *    , optimize_empty_base<Allocator, 1> {
 *
 *    Parent()
 *       : optimize_empty_base<Compare,0>(empty_init_t())
 *       , optimize_empty_base<Allocator, 1>(empty_init_t())
 *       , ptr_() {
 *    }
 *
 *    Parent(const Compare& c, const Allocator& a)
 *       : optimize_empty_base<Compare,0>(empty_init_t(), c)
 *       , optimize_empty_base<Allocator, 1>(empty_init_t(), a)
 *       , ptr_() {
 *    }
 *
 *    const Compare& compare() const {
 *       return optimize_empty_base<Compare,0>::get();
 *    }
 *
 *    Compare& compare() {
 *       return optimize_empty_base<Compare,0>::get();
 *    }
 *
 *    const Allocator& allocator() const {
 *       return optimize_empty_base<Allocator,1>::get();
 *    }
 *
 *    Allocator& allocator() {
 *       return optimize_empty_base<Allocator,1>::get();
 *    }
 *
 * private:
 *    T* ptr_;
 * };
 * \endcode
 *
 * \tparam T The type to inherit
 * \tparam N Optional: Specify to create a distinct base type
 * \tparam E Optional: Specify to force inheritance from type
 */
template<typename T, std::size_t N = 0, bool E = is_empty_value_base<T>::value>
class optimize_empty_base {
public:
	using type = T;

	optimize_empty_base() = default;

	optimize_empty_base(empty_init_t) : value_() {
	}

    template<class... Args>
    explicit optimize_empty_base(empty_init_t, Args&&... args) : value_(std::forward<Args>(args)...) {
    }

    const T& get() const noexcept {
        return value_;
    }

    T& get() noexcept {
        return value_;
    }

private:
	type value_;
};


/** Wrapper to simplify empty inheritance optimization
*/
template<typename T, std::size_t N>
class optimize_empty_base<T, N, true> : T {
public:
	using type = T;

	optimize_empty_base() = default;

	optimize_empty_base(empty_init_t) : T() {
	}

    template<class... Args>
    explicit optimize_empty_base(empty_init_t, Args&&... args) : T(std::forward<Args>(args)...) {
    }

    const T& get() const noexcept {
        return *this;
    }

    T& get() noexcept {
        return *this;
    }
};


} // namespace xstd
