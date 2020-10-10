/*
 * empty_value.hpp
 *
 *  Created on: Apr 14, 2020
 *      Author: bflynt
 */

#ifndef INCLUDE_XSTD_TYPE_TRAITS_EMPTY_VALUE_HPP_
#define INCLUDE_XSTD_TYPE_TRAITS_EMPTY_VALUE_HPP_


#include <type_traits> // std::is_empty, std::is_final
#include <utility>     // std::forward

namespace xstd {

/** Tests if T can be used for empty base class
 */
template<typename T>
struct use_empty_value_base {
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
 * Example of use:
 * \code{.cpp}
 *
 * //
 * // Parent Class Inherits from Base
 * //
 * template<typename T, typename Compare, typename Allocator>
 * class Parent
 *    : empty_value<Compare, 0>
 *    , empty_value<Allocator, 1> {
 *
 *    Parent()
 *       : empty_value<Compare,0>(empty_init_t())
 *       , empty_value<Allocator, 1>(empty_init_t())
 *       , ptr_() {
 *    }
 *
 *    Parent(const Compare& c, const Allocator& a)
 *       : empty_value<Compare,0>(empty_init_t(), c)
 *       , empty_value<Allocator, 1>(empty_init_t(), a)
 *       , ptr_() {
 *    }
 *
 *    const Compare& compare() const {
 *       return empty_value<Compare,0>::get();
 *    }
 *
 *    Compare& compare() {
 *       return empty_value<Compare,0>::get();
 *    }
 *
 *    const Allocator& allocator() const {
 *       return empty_value<Allocator,1>::get();
 *    }
 *
 *    Allocator& allocator() {
 *       return empty_value<Allocator,1>::get();
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
template<typename T, std::size_t N = 0, bool E = use_empty_value_base<T>::value>
class empty_value {
public:
	using type = T;

	empty_value() = default;

	empty_value(empty_init_t) : value_() {
	}

    template<class... Args>
    explicit empty_value(empty_init_t, Args&&... args) : value_(std::forward<Args>(args)...) {
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
class empty_value<T, N, true> : T {
public:
	using type = T;

	empty_value() = default;

	empty_value(empty_init_t) : T() {
	}

    template<class... Args>
    explicit empty_value(empty_init_t, Args&&... args) : T(std::forward<Args>(args)...) {
    }

    const T& get() const noexcept {
        return *this;
    }

    T& get() noexcept {
        return *this;
    }
};


} // namespace xstd


#endif /* INCLUDE_XSTD_TYPE_TRAITS_EMPTY_VALUE_HPP_ */
