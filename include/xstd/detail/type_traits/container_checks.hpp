/*
 * container_checks.hpp
 *
 *  Created on: Dec 8, 2019
 *      Author: bflynt
 */

#ifndef INCLUDE_XSTD_TYPE_TRAITS_CONTAINER_CHECKS_HPP_
#define INCLUDE_XSTD_TYPE_TRAITS_CONTAINER_CHECKS_HPP_


#include <type_traits>


namespace xstd {


/// Instantiate the template types passed
/**
 * Empty TMP to instantiate the template types passed
 * but never use them.  Used to replace std::void_t
 * from C++17 since this name makes more sense.
 * Primarily used for SFINAE to detect if expression
 * is valid.
 */
template<typename...>
using try_to_instantiate = void;

/// Dummy argument
using disregard_this = void;

/// Default type if instantiation fails
template<template<typename...> class Expression, typename Attempt, typename... Ts>
struct is_detected_impl : std::false_type{};

/// Type if instantiation succeeds
template<template<typename...> class Expression, typename... Ts>
struct is_detected_impl<Expression, try_to_instantiate<Expression<Ts...>>, Ts...> : std::true_type{};


/// Usable TMP function to detect features
/**
 * This is the final function a user should use to detect if
 * a type has the features required.  Can except single or
 * multiple types depending on the feature needing detection.
 *
 * Example:
 * To detect if one type can be assigned to another.
 * \code
 * template<typename T, typename U>
 * using assign_expression = decltype(std::declval<T&>() = std::declval<U&>());
 *
 * template<typename T, typename U>
 * using is_assignable = is_detected<assign_expression, T, U>;
 *
 * template<typename T, typename U>
 * constexpr bool is_assignable_v = is_assignable<T,U>::value;
 * \endcode
 *
 * Example:
 * To detect if type has a size method.
 * \code
 * template<typename T>
 * using size_expression = decltype(std::declval<T&>().size());
 *
 * template<typename T>
 * using has_size = is_detected<size_expression, T>;
 *
 * template<typename T>
 * constexpr bool has_size_v = has_size<T>::value;
 * \endcode
 *
 */
template<template<typename...> class Expression, typename... Ts>
struct is_detected : is_detected_impl<Expression, disregard_this, Ts...>{};



template<typename T>
using size_expression = decltype(std::declval<T&>().size());

template<typename T>
using iterator_expression = typename T::iterator;

template<typename T>
using begin_expression = decltype(std::declval<T&>().begin());

template<typename T>
using end_expression = decltype(std::declval<T&>().end());


/// Test if class has size() method
/**
 * Test if template type T has size() method.
 *
 * Usage:
 * \code
 * has_size<MyClass>::type
 * has_size<MyClass>::value
 * \endcode
 *
 * \tparam T Class to check
 */
template<typename T>
using has_size = is_detected<size_expression, T>;

template<typename T>
constexpr bool has_size_v = has_size<T>::value;


/// Test if class has begin() method
/**
 * Test if template type T has begin() method.
 *
 * Usage:
 * \code
 * has_begin<MyClass>::type
 * has_begin<MyClass>::value
 * \endcode
 *
 * \tparam T Class to check
 */
template<typename T>
using has_begin = is_detected<begin_expression, T>;

template<typename T>
constexpr bool has_begin_v = has_begin<T>::value;

/// Test if class has end() method
/**
 * Test if template type T has end() method.
 *
 * Usage:
 * \code
 * has_end<MyClass>::type
 * has_end<MyClass>::value
 * \endcode
 *
 * \tparam T Class to check
 */
template<typename T>
using has_end = is_detected<end_expression, T>;

template<typename T>
constexpr bool has_end_v = has_end<T>::value;


/// Test if class has begin() and end() method
/**
 * Test if template type T has begin() and end() method.
 *
 * Usage:
 * \code
 * has_begin_end<MyClass>::type
 * has_begin_end<MyClass>::value
 * \endcode
 *
 * \tparam T Class to check
 */
template<typename T>
using has_begin_end = std::conjunction< is_detected<begin_expression, T>,
		                                is_detected<end_expression, T> >;

template<typename T>
constexpr bool has_begin_end_v = has_begin_end<T>::value;



/// Test if class has iterator type
/**
 * Test if template type T has iterator type.
 *
 * Usage:
 * \code
 * has_iterator<MyClass>::type
 * has_iterator<MyClass>::value
 * \endcode
 *
 * \tparam T Class to check
 */
template<typename T>
using has_iterator = is_detected<iterator_expression, T>;

template<typename T>
constexpr bool has_iterator_v = has_iterator<T>::value;



/// Get the proper constantness iterator for the class
/**
 * Return the proper iterator for a class instance based
 * on the constantness of the template type.  Used when a function,
 * etc. does not know if the variable passed in a constant or not.
 *
 * Results:
 * | Input Type | Result Type    |
 * | :--------: | :------------: |
 * | T          | iterator       |
 * | const T    | const_iterator |
 *
 * Usage:
 * \code
 *	template<typename T>
 *	int do_something(T value){
 *		get_iterator<T>::type it   = T.begin();
 *		get_iterator<T>::type last = T.end();
 *		while( it != last ){
 *		   ...
 *		   ++it;
 *		}
 *	};
 * \endcode
 *
 * \tparam T Class to check
 */
template<typename T>
struct get_iterator {
  using type = typename std::conditional< std::is_const<T>::value,
      typename std::remove_reference<T>::type::const_iterator,
      typename std::remove_reference<T>::type::iterator >::type;
};

} /* namespace xstd */



#endif /* INCLUDE_XSTD_TYPE_TRAITS_CONTAINER_CHECKS_HPP_ */
