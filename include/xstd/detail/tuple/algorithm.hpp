/*
 * algorithm.hpp
 *
 *  Created on: Dec 10, 2019
 *      Author: bflynt
 */

#ifndef INCLUDE_XSTD_TUPLE_ALGORITHM_HPP_
#define INCLUDE_XSTD_TUPLE_ALGORITHM_HPP_

#include "xstd/assert.hpp"

#include <cstddef>     // std::size_t
#include <algorithm>   // std::min, std::max
#include <tuple>       // std::tuple, etc.
#include <utility>     // std::index_sequence

/**
 * \file
 * algorithm.hpp
 *
 * \brief
 * Element wise algorithmic operations for Tuples
 *
 * \details
 * The functions within this file provide element by element
 * operators for tuples of values similar to the standard
 * library algorithm header does for standard container types.
 * Each function uses the +,-,*,/,%,min,max operators so user
 * types with properly overloaded operators can be used within
 * the tuples. If two tuples are different length the resulting
 * tuple provided as the answer will only be of the shortest
 * length.
 */
/// @cond SKIP_DETAIL
namespace xstd {
namespace detail {

template<class UnaryPredicate, class...Args>
constexpr UnaryPredicate
for_each_arg(UnaryPredicate f, Args&&...args) {
  std::initializer_list<int>{((void)f(std::forward<Args>(args)), 0)...};
  return f;
}

template <class Tuple, class UnaryPredicate, std::size_t... I>
constexpr UnaryPredicate
for_each_impl(Tuple&& t, UnaryPredicate&& f, std::index_sequence<I...>){
    return (void)std::initializer_list<int>{((void)std::forward<UnaryPredicate>(f)(std::get<I>(std::forward<Tuple>(t))),0)...}, f;
}

template <class Tuple1, class Tuple2, class UnaryPredicate, std::size_t... I>
constexpr UnaryPredicate
for_each_impl(Tuple1&& t1, Tuple2&& t2, UnaryPredicate&& f, std::index_sequence<I...>) {
    return (void)std::initializer_list<int>{((void)std::forward<UnaryPredicate>(f)(std::get<I>(std::forward<Tuple1>(t1)), std::get<I>(std::forward<Tuple2>(t2))),0)...}, f;
}

template<typename...Ts, typename UnaryPredicate, std::size_t... Is>
auto transform_impl(std::tuple<Ts...> const& inputs, UnaryPredicate pred, std::index_sequence<Is...>){
    return std::tuple<std::result_of_t<UnaryPredicate(Ts)>...>{pred(std::get<Is>(inputs))...};
}

template<typename...Ts1, typename...Ts2, typename BinaryPredicate, std::size_t... Is>
auto transform_impl(std::tuple<Ts1...> const& t1, std::tuple<Ts2...> const& t2, BinaryPredicate pred, std::index_sequence<Is...>){
    return std::tuple<std::result_of_t<BinaryPredicate(Ts1,Ts2)>...>{pred(std::get<Is>(t1),std::get<Is>(t2))...};
}

} /* namespace detail */
} /* namespace xstd */
/// @endcond


namespace xstd {

//
// Forward Declarations
//
template<typename... Ts, typename Function>
constexpr auto
transform(std::tuple<Ts...> const& inputs, Function function);

template<typename... Ts1, typename... Ts2, typename Function>
constexpr auto
transform(std::tuple<Ts1...> const& t1, std::tuple<Ts2...> const& t2, Function function);



/// Return minimum value within a tuple
/**
 * Returns the minimum value within the tuple as the
 * std::common_type of all values.
 *
 * \param t[in] Tuple to evaluate
 *
 * \return Minimum value within the tuple
 */
template<typename... Ts>
constexpr auto
min(const std::tuple<Ts...>& t) {
	using std::min;
	using return_type = std::common_type_t<Ts...>;
	return std::apply([](auto&& ...xs){ return min({static_cast<return_type>(xs)...}); }, std::tuple<Ts...>(t));
}

/// Return minimum value within a tuple
/**
 * Returns the minimum value within the tuple as the
 * std::common_type of all values.
 *
 * \param t[in] Tuple to move in and evaluate
 *
 * \return Minimum value within the tuple
 */
template<typename... Ts>
constexpr auto
min(std::tuple<Ts...>&& t) {
	using std::min;
	using return_type = std::common_type_t<Ts...>;
	return std::apply([](auto&& ...xs){ return min({static_cast<return_type>(xs)...}); }, std::forward<std::tuple<Ts...>>(t));
}

/// Return maximum value within a tuple
/**
 * Returns the maximum value within the tuple as the
 * std::common_type of all values.
 *
 * \param t[in] Tuple to evaluate
 *
 * \return Maximum value within the tuple
 */
template<typename... Ts>
constexpr auto
max(const std::tuple<Ts...>& t) {
	using std::max;
	using return_type  = std::common_type_t<Ts...>;
	return std::apply([](auto&& ...xs){ return max({static_cast<return_type>(xs)...}); }, std::tuple<Ts...>(t));
}

/// Return maximum value within a tuple
/**
 * Returns the maximum value within the tuple as the
 * std::common_type of all values.
 *
 * \param t[in] Tuple to move in and evaluate
 *
 * \return Maximum value within the tuple
 */
template<typename... Ts>
constexpr auto
max(std::tuple<Ts...>&& t) {
	using std::max;
	using return_type  = std::common_type_t<Ts...>;
	return std::apply([](auto&& ...xs){ return max({static_cast<return_type>(xs)...}); }, std::forward<std::tuple<Ts...>>(t));
}

/// Test if unary predicate is true for all
/**
 * Returns true if unary pred returns true for all the elements
 * in the tuple.
 *
 * \param t[in] Tuple to move in and evaluate
 * \param p[in] Unary predicate to evaluate at each entry
 *
 * \return true if pred returns true for all the elements
 */
template<typename Tuple, typename UnaryPredicate>
constexpr bool
all_of(Tuple&& t, UnaryPredicate&& p) noexcept {
    return std::apply([&p](auto&& ...xs){ return (p(std::forward<decltype(xs)>(xs)) && ...); }, std::forward<Tuple>(t));
}

/// Test if binary predicate is true for all
/**
 * Returns true if binary pred returns true for all the elements
 * in the tuples.
 *
 * \param t1[in] Tuple 1 to move in and evaluate
 * \param t2[in] Tuple 2 to move in and evaluate
 * \param p[in] Binary predicate to evaluate at each entry
 *
 * \return true if pred returns true for all the elements
 */
template<typename Tuple1, typename Tuple2, typename BinaryPredicate>
constexpr bool
all_of(Tuple1&& t1, Tuple2&& t2, BinaryPredicate&& p) noexcept {
	auto result = xstd::transform(std::forward<Tuple1>(t1),std::forward<Tuple2>(t2), std::forward<BinaryPredicate>(p));
	return xstd::all_of(result, [](auto&& val){return val;});
}

/// Test if unary predicate is true for any
/**
 * Returns true if unary pred returns true for any of the
 * elements in the tuple.
 *
 * \param t[in] Tuple to move in and evaluate
 * \param p[in] Unary predicate to evaluate at each entry
 *
 * \return true if pred returns true for any of the elements
 */
template<typename Tuple, typename UnaryPredicate>
constexpr bool
any_of(Tuple&& t, UnaryPredicate&& p) noexcept {
    return std::apply([&p](auto&& ...xs){ return (p(std::forward<decltype(xs)>(xs)) || ...); }, std::forward<Tuple>(t));
}

/// Test if binary predicate is true for any
/**
 * Returns true if binary pred returns true for any of the
 * elements in the tuples.
 *
 * \param t1[in] Tuple 1 to move in and evaluate
 * \param t2[in] Tuple 2 to move in and evaluate
 * \param p[in] Binary predicate to evaluate at each entry
 *
 * \return true if pred returns true for any of the elements
 */
template<typename Tuple1, typename Tuple2, typename BinaryPredicate>
constexpr bool
any_of(Tuple1&& t1, Tuple2&& t2, BinaryPredicate&& p) noexcept {
	auto result = xstd::transform(std::forward<Tuple1>(t1),std::forward<Tuple2>(t2), std::forward<BinaryPredicate>(p));
    return xstd::any_of(result, [](auto&& val){return val;});
}

/// Test if unary predicate is false for all
/**
 * Returns true if unary pred returns false for all of the
 * elements in the tuple.
 *
 * \param t[in] Tuple to move in and evaluate
 * \param p[in] Unary predicate to evaluate at each entry
 *
 * \return true if pred returns false for all of the elements
 */
template<typename Tuple, typename UnaryPredicate>
constexpr bool
none_of(Tuple&& t, UnaryPredicate&& p) noexcept {
    return std::apply([&p](auto&& ...xs){ return !(p(std::forward<decltype(xs)>(xs)) || ...); }, std::forward<Tuple>(t));
}

/// Test if binary predicate is false for all
/**
 * Returns true if binary pred returns false for all of the
 * elements in the tuples.
 *
 * \param t1[in] Tuple 1 to move in and evaluate
 * \param t2[in] Tuple 2 to move in and evaluate
 * \param p[in] Binary predicate to evaluate at each entry
 *
 * \return true if pred returns false for all of the elements
 */
template<typename Tuple1, typename Tuple2, typename BinaryPredicate>
constexpr bool
none_of(Tuple1&& t1, Tuple2&& t2, BinaryPredicate&& p) noexcept {
	auto result = xstd::transform(std::forward<Tuple1>(t1),std::forward<Tuple2>(t2), std::forward<BinaryPredicate>(p));
    return xstd::none_of(result, [](auto&& val){return val;});
}


/// Apply predicate to each index of tuple
/**
 * Apply predicate to each index of tuple returning the
 * predicate in case it holds a state.
 *
 * \param t1[in] Tuple 1 to move in and evaluate
 * \param p[in] Unary predicate to evaluate at each entry
 *
 * \return Unary predicate that was used
 */
template<typename Tuple, typename UnaryPredicate>
constexpr UnaryPredicate
for_each(Tuple&& t, UnaryPredicate p) noexcept {
	std::apply([&p](auto&& ...xs){(..., static_cast<void>(p(std::forward<decltype(xs)>(xs))));}, std::forward<Tuple>(t));
	return p;
}

/// Apply predicate to each index of tuples
/**
 * Apply predicate to each index of the provided tuples
 * returning the predicate in case it holds a state.
 *
 * \param t1[in] Tuple 1 to move in and evaluate
 * \param t2[in] Tuple 2 to move in and evaluate
 * \param p[in] Binary predicate to evaluate at each entry
 *
 * \return Binary predicate that was used
 */
template <class Tuple1, class Tuple2, class BinaryPredicate>
constexpr BinaryPredicate
for_each(Tuple1&& t1, Tuple2&& t2, BinaryPredicate&& f) {
	constexpr auto t1_size = std::tuple_size_v<std::remove_reference_t<Tuple1>>;
	constexpr auto t2_size = std::tuple_size_v<std::remove_reference_t<Tuple2>>;
	constexpr auto size = std::min(t1_size,t2_size);
    detail::for_each_impl(std::forward<Tuple1>(t1),
    		              std::forward<Tuple2>(t2),
						  std::forward<BinaryPredicate>(f),
                          std::make_index_sequence<size>{});
    return f;
}


/// Count if predicate evaluates to true
/**
 * Apply unary predicate to each index of tuple and count
 * if the predicate evaluates to true
 *
 * \param t1[in] Tuple 1 to move in and evaluate
 * \param p[in] Unary predicate to evaluate at each entry
 *
 * \return Number of predicates that evaluated to true
 */
template<typename Tuple, typename UnaryPredicate>
constexpr std::size_t
count_if(Tuple&& t, UnaryPredicate p) noexcept {
    std::size_t count = 0;
    ::xstd::for_each(t, [&](auto&& value){
            if( p(value) ) {
                ++count;
            }
        });
    return count;
}

/// Count if predicate evaluates to true
/**
 * Apply binary predicate to each index of tuple and count
 * if the predicate evaluates to true
 *
 * \param t1[in] Tuple 1 to move in and evaluate
 * \param t2[in] Tuple 2 to move in and evaluate
 * \param p[in] Binary predicate to evaluate at each entry
 *
 * \return Number of predicates that evaluated to true
 */
template<typename T1, typename T2, typename BinaryPredicate>
constexpr std::size_t
count_if(T1&& t1, T2&& t2, BinaryPredicate p) noexcept {
	STATIC_ASSERT(std::tuple_size<std::remove_reference_t<T1>>::value ==
                  std::tuple_size<std::remove_reference_t<T2>>::value,
                  "Tuples must be same length");
    std::size_t count = 0;
    ::xstd::for_each(t1, t2, [&](auto&& v1, auto&& v2){
        if( p(v1,v2) ) {
            ++count;
        }
    });
    return count;
}

/// Return index where unary predicate returns true
/**
 * Apply unary predicate to each index of tuple and
 * return the index of the first entry that evaluates
 * to true
 *
 * \param t1[in] Tuple 1 to move in and evaluate
 * \param p[in] Unary predicate to evaluate at each entry
 *
 * \return Index of first predicate that evaluated to true
 */
template<typename Tuple, typename UnaryPredicate>
constexpr std::size_t
find_if(Tuple&& t, UnaryPredicate p) noexcept {
	std::size_t index = std::tuple_size<std::remove_reference_t<Tuple>>::value;
	std::size_t current_index = 0;
	bool found = false;
	::xstd::for_each(t, [&](auto&& value){
		if (!found && p(value)) {
			index = current_index;
			found = true;
		}
		++current_index;
	});
	return index;
}

/// Return index where binary predicate returns true
/**
 * Apply binary predicate to each index of tuple and
 * return the index of the first entry that evaluates
 * to true
 *
 * \param t1[in] Tuple 1 to move in and evaluate
 * \param t2[in] Tuple 2 to move in and evaluate
 * \param p[in] Binary predicate to evaluate at each entry
 *
 * \return Index of first predicate that evaluated to true
 */
template<typename T1, typename T2, typename BinaryPredicate>
constexpr std::size_t
find_if(T1&& t1, T2&& t2, BinaryPredicate p) noexcept {
	STATIC_ASSERT(std::tuple_size<std::remove_reference_t<T1>>::value ==
			      std::tuple_size<std::remove_reference_t<T2>>::value,
				  "Tuples must be same length");
	std::size_t index = std::tuple_size<std::remove_reference_t<T1>>::value;
	std::size_t current_index = 0;
	bool found = false;
	::xstd::for_each(t1, t2, [&](auto&& v1, auto&& v2){
		if (!found && p(v1,v2)) {
			index = current_index;
			found = true;
		}
		++current_index;
	});
	return index;
}

/// Transform tuple using the provided function
/**
 * Transforms the provided tuple using the provided
 * function returning a new tuple of same element type
 * and size
 *
 * \param t1[in] Tuple 1 to move in and evaluate
 * \param p[in] Unary predicate to evaluate at each entry
 *
 * \return Transformed tuple
 */
template<typename... Ts, typename Function>
constexpr auto
transform(std::tuple<Ts...> const& inputs, Function function){
    return detail::transform_impl(inputs, function, std::make_index_sequence<sizeof...(Ts)>{});
}

/// Transform tuples using the provided function
/**
 * Transforms the provided tuples using the provided
 * function returning a new tuple of same element type
 * and size
 *
 * \param t1[in] Tuple 1 to move in and evaluate
 * \param t2[in] Tuple 2 to move in and evaluate
 * \param p[in] Binary predicate to evaluate at each entry
 *
 * \return Transformed tuple
 */
template<typename... Ts1, typename... Ts2, typename Function>
constexpr auto
transform(std::tuple<Ts1...> const& t1, std::tuple<Ts2...> const& t2, Function function){
    return detail::transform_impl(t1,t2, function, std::make_index_sequence<sizeof...(Ts1)>{});
}

/// Perform action on single index of tuple
/**
 * Perform the provided action on the index
 * specified.
 *
 * \param t[in] Tuple 1 to move in and evaluate
 * \param index[in] Index to perform action on
 * \param action[in] Action to perform
 */
template<typename Tuple, typename Action>
constexpr void
perform(Tuple&& t, const std::size_t index, Action action){
	std::size_t current_index = 0;
	for_each(t, [action = std::move(action), index, &current_index](auto&& value){
		if (index == current_index){
			action(std::forward<decltype(value)>(value));
		}
		++current_index;
	});
}


} /* namespace xstd */


#endif /* INCLUDE_XSTD_TUPLE_ALGORITHM_HPP_ */
