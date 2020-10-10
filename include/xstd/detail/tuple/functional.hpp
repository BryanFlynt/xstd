/*
 * functional.hpp
 *
 *  Created on: Dec 10, 2019
 *      Author: bflynt
 */

#ifndef INCLUDE_XSTD_TUPLE_FUNCTIONAL_HPP_
#define INCLUDE_XSTD_TUPLE_FUNCTIONAL_HPP_


#include <algorithm> // std::min, std::max, etc.
#include <cstddef>   // std::size_t
#include <tuple>     // std::tuple, etc.


/**
 * \file
 * functional.hpp
 *
 * \brief
 * Element wise functional operations for Tuples.
 *
 * \details
 * The functions within this file provide element by element
 * operators for tuples of values similar to the standard
 * library functional header does for standard types.
 * Each function uses the +,-,*,/,% operators so user types
 * with properly overloaded operators can be used within the
 * tuples. If two tuples are different length the resulting
 * tuple provided as the answer will only be of the shortest
 * length.
 */
/// @cond SKIP_DETAIL
namespace xstd {
namespace detail {

template <typename Tuple1, typename Tuple2>
struct minimum_tuple_size {
	static constexpr std::size_t value = std::min(
			std::tuple_size_v<std::remove_reference_t<Tuple1>>,
			std::tuple_size_v<std::remove_reference_t<Tuple2>>);
};

template<class T1, class T2>
inline constexpr std::size_t minimum_tuple_size_v = minimum_tuple_size<T1,T2>::value;


template <typename Tuple1, typename Tuple2, std::size_t ... Is>
constexpr auto
plus(Tuple1&& t1, Tuple2&& t2, std::index_sequence<Is...>) {
	return std::make_tuple((std::get<Is>(std::forward<Tuple1>(t1)) + std::get<Is>(std::forward<Tuple2>(t2)))...);
}

template <typename Tuple1, typename Tuple2, std::size_t ... Is>
constexpr auto
minus(Tuple1&& t1, Tuple2&& t2, std::index_sequence<Is...>) {
	return std::make_tuple((std::get<Is>(std::forward<Tuple1>(t1)) - std::get<Is>(std::forward<Tuple2>(t2)))...);
}

template <typename Tuple1, typename Tuple2, std::size_t ... Is>
constexpr auto
multiplies(Tuple1&& t1, Tuple2&& t2, std::index_sequence<Is...>) {
	return std::make_tuple((std::get<Is>(std::forward<Tuple1>(t1)) * std::get<Is>(std::forward<Tuple2>(t2)))...);
}

template <typename Tuple1, typename Tuple2, std::size_t ... Is>
constexpr auto
divides(Tuple1&& t1, Tuple2&& t2, std::index_sequence<Is...>) {
	return std::make_tuple((std::get<Is>(std::forward<Tuple1>(t1)) / std::get<Is>(std::forward<Tuple2>(t2)))...);
}

template <typename Tuple1, typename Tuple2, std::size_t ... Is>
constexpr auto
modulus(Tuple1&& t1, Tuple2&& t2, std::index_sequence<Is...>) {
	return std::make_tuple((std::get<Is>(std::forward<Tuple1>(t1)) % std::get<Is>(std::forward<Tuple2>(t2)))...);
}

template <typename Tuple1, std::size_t ... Is>
constexpr auto
negate(Tuple1&& t1, std::index_sequence<Is...>) {
	return std::make_tuple( (-std::get<Is>(t1))...);
}

} /* namespace detail */
} /* namespace xstd */
/// @endcond

namespace xstd {

/// Add two tuples together element by element
/**
 * Takes two tuples and generates a new tuple by applying the
 * operator+ to each entry. The returned tuple is the same
 * length as the tuple with the fewest elements. Types are not
 * checked and rely on a suitable operator+ being found for the
 * data types.
 *
 * \param t1[in] Tuple 1
 * \param t1[in] Tuple 2
 *
 * \return Tuple = t1 + t2
 */
template <typename Tuple1, typename Tuple2>
constexpr auto
operator+(Tuple1&& t1, Tuple2&& t2) {
	constexpr auto min_size = ::xstd::detail::minimum_tuple_size_v<Tuple1,Tuple2>;
	return ::xstd::detail::plus(std::forward<Tuple1>(t1),
			                    std::forward<Tuple2>(t2),
						        std::make_index_sequence<min_size>{});
}

/// Subtract two tuples from each other element by element
/**
 * Takes two tuples and generates a new tuple by applying the
 * operator- to each entry. The returned tuple is the same
 * length as the tuple with the fewest elements. Types are not
 * checked and rely on a suitable operator- being found for the
 * data types.
 *
 * \param t1[in] Tuple 1
 * \param t2[in] Tuple 2
 *
 * \return Tuple = t1 - t2
 */
template <typename Tuple1, typename Tuple2>
constexpr auto
operator-(Tuple1&& t1, Tuple2&& t2) {
	constexpr auto min_size = ::xstd::detail::minimum_tuple_size_v<Tuple1,Tuple2>;
	return ::xstd::detail::minus(std::forward<Tuple1>(t1),
			                     std::forward<Tuple2>(t2),
						         std::make_index_sequence<min_size>{});
}

/// Multiply two tuples together element by element
/**
 * Takes two tuples and generates a new tuple by applying the
 * operator* to each entry. The returned tuple is the same
 * length as the tuple with the fewest elements. Types are not
 * checked and rely on a suitable operator* being found for the
 * data types.
 *
 * \param t1[in] Tuple 1
 * \param t2[in] Tuple 2
 *
 * \return Tuple = t1 * t2
 */
template <typename Tuple1, typename Tuple2>
constexpr auto
operator*(Tuple1&& t1, Tuple2&& t2) {
	constexpr auto min_size = ::xstd::detail::minimum_tuple_size_v<Tuple1,Tuple2>;
	return ::xstd::detail::multiplies(std::forward<Tuple1>(t1),
			                          std::forward<Tuple2>(t2),
						              std::make_index_sequence<min_size>{});
}

/// Divide two tuples element by element
/**
 * Takes two tuples and generates a new tuple by applying the
 * operator/ to each entry. The returned tuple is the same
 * length as the tuple with the fewest elements. Types are not
 * checked and rely on a suitable operator/ being found for the
 * data types.
 *
 * \param t1[in] Tuple 1
 * \param t2[in] Tuple 2
 *
 * \return Tuple = t1 / t2
 */
template <typename Tuple1, typename Tuple2>
constexpr auto
operator/(Tuple1&& t1, Tuple2&& t2) {
	constexpr auto min_size = ::xstd::detail::minimum_tuple_size_v<Tuple1,Tuple2>;
	return ::xstd::detail::divides(std::forward<Tuple1>(t1),
			                       std::forward<Tuple2>(t2),
						           std::make_index_sequence<min_size>{});
}

/// Modulo two tuples element by element
/**
 * Takes two tuples and generates a new tuple by applying the
 * operator% to each entry. The returned tuple is the same
 * length as the tuple with the fewest elements. Types are not
 * checked and rely on a suitable operator% being found for the
 * data types.
 *
 * \param t1[in] Tuple 1
 * \param t2[in] Tuple 2
 *
 * \return Tuple = t1 % t2
 */
template <typename Tuple1, typename Tuple2>
constexpr auto
operator%(Tuple1&& t1, Tuple2&& t2) {
	constexpr auto min_size = ::xstd::detail::minimum_tuple_size_v<Tuple1,Tuple2>;
	return ::xstd::detail::modulus(std::forward<Tuple1>(t1),
			                       std::forward<Tuple2>(t2),
						           std::make_index_sequence<min_size>{});
}

/// Negate a single tuple element by element
/**
 * Takes a single tuple and generates a new tuple by applying the
 * negate operator to each entry. The returned tuple is the same
 * length as the tuple passed in. Types are not checked and rely
 * on a suitable operator- being found for the data types.
 *
 * \param t1[in] Tuple 1
 *
 * \return Tuple = -t1
 */
template <typename Tuple1>
constexpr auto
operator-(Tuple1&& t1){
	constexpr auto size = std::tuple_size_v<std::remove_reference_t<Tuple1>>;
	return ::xstd::detail::negate(t1, std::make_index_sequence<size>{});
}

} /* namespace xstd */

#endif /* INCLUDE_XSTD_TUPLE_FUNCTIONAL_HPP_ */
