/*
 * enumerate.hpp
 *
 *  Created on: Dec 21, 2019
 *      Author: bflynt
 */

#ifndef INCLUDE_XSTD_ITERATOR_ENUMERATE_HPP_
#define INCLUDE_XSTD_ITERATOR_ENUMERATE_HPP_


#include "xstd/detail/type_traits/container_checks.hpp" // xstd::has_begin_end


#include <initializer_list>
#include <iterator> // std::iterator_traits
#include <utility>  // std::pair


namespace xstd {


/** Enumerating Iterator
 *
 * Iterator which returns a std::pair with a counter and
 * reference value each time it is dereferenced. Can be
 * used to set values or get indices within std algorithms.
 *
 * \tparam Iterator The iterator type to hold
 * \tparam Incrementable Value type to be incremented by iterator
 *
 * \code{.cpp}
 * const int N = 100;
 * std::vector<int> vec(N);
 * std::for_each( enumerating_iterator(vec.begin()),
 *                enumerating_iterator(vec.end()),
 *                [](auto vec_pair){
 *                    vec_pair.second = vec_pair.first;
 *                });
 * \endcode
 *
 * Has the same behavior as
 *
 * \code{.cpp}
 * const int N = 100;
 * std::vector<int> vec(N);
 * for(int i = 0; i < vec.size(); ++i){
 * 	   vec[i] = i;
 * }
 * \endcode
 */
template<typename Iterator, typename Incrementable = std::size_t>
struct enumerating_iterator {

	// ====================================================
	// Types
	// ====================================================

	using iterator_type             = Iterator;
	using traits_type               = std::iterator_traits<iterator_type>;
	using iterator_difference_type  = typename traits_type::difference_type;
	using iterator_value_type       = typename traits_type::value_type;
	using iterator_pointer_type     = typename traits_type::pointer;
	using iterator_reference_type   = typename traits_type::reference;
	using iterator_category_type    = typename traits_type::iterator_category;
	using counter_type              = Incrementable;

	using value_type = std::pair<counter_type, iterator_reference_type>;

	// ====================================================
	// Constructors
	// ====================================================

	enumerating_iterator() = delete;


	enumerating_iterator(iterator_type iter, counter_type first = 0) :
		count_(first),iterator_(iter) {
	}


	// ====================================================
	// Operators
	// ====================================================

	enumerating_iterator& operator++(){
		count_++;
		iterator_++;
		return *this;
	}

	enumerating_iterator operator++(int){
		auto tmp = *this;
		count_++;
		iterator_++;
		return tmp;
	}

	enumerating_iterator& operator+=(const iterator_difference_type& inc){
		count_    += inc;
		iterator_ += inc;
		return *this;
	}

	enumerating_iterator& operator--(){
		count_--;
		iterator_--;
		return *this;
	}

	enumerating_iterator operator--(int){
		auto tmp = *this;
		count_--;
		iterator_--;
		return tmp;
	}

	enumerating_iterator& operator-=(const iterator_difference_type& inc){
		count_    -= inc;
		iterator_ -= inc;
		return *this;
	}

	value_type operator[](const iterator_difference_type& n) {
		return {count_+static_cast<counter_type>(n), iterator_[n]};
	}

	value_type operator*() {
		return {count_, *iterator_};
	}

	// ====================================================
	// Friend Operators
	// ====================================================

	friend bool operator==(const enumerating_iterator& x, const enumerating_iterator& y) {
		return (x.iterator_ == y.iterator_);
	}

	friend bool operator!=(const enumerating_iterator& x, const enumerating_iterator& y) {
		return not( x == y );
	}

	friend bool operator<(const enumerating_iterator& x, const enumerating_iterator& y) {
		return (x.iterator_ < y.iterator_);
	}

	friend iterator_difference_type operator-(const enumerating_iterator& x, const enumerating_iterator& y) {
		return (x.iterator_ - y.iterator_);
	}

	friend enumerating_iterator operator+(enumerating_iterator x, iterator_difference_type i) {
		x.count_    += static_cast<counter_type>(i);
		x.iterator_ += i;
		return x;
	}

	friend enumerating_iterator operator+(iterator_difference_type i, enumerating_iterator y) {
		y.count_    += static_cast<counter_type>(i);
		y.iterator_ += i;
		return y;
	}

private:
	counter_type  count_;
	iterator_type iterator_;
};



/**
 * @brief
 * Proxy returned by enumerate function
 *
 * This is the class returned by the enumerate() function
 * within an enumerated based for loop.
 */
template <typename Iterator, typename Incrementable = std::size_t>
struct enumerate_proxy {

	using iterator_type = Iterator;
	using counter_type  = Incrementable;

	using value_type = enumerating_iterator<iterator_type,counter_type>;


	enumerate_proxy() = delete;

	enumerate_proxy(Iterator first, Iterator last, Incrementable init = 0) :
		first_(first), last_(last), init_value_(init) {
	}

    ~enumerate_proxy() = default;

    value_type begin() const {
    	return {first_, init_value_};
    }

    value_type end() const {
    	return {last_, init_value_};
    }

    const value_type cbegin() const {
    	return {first_, init_value_};
    }

    const value_type cend() const {
    	return {last_, init_value_};
    }

private:
    const iterator_type first_;
    const iterator_type last_;
    const counter_type  init_value_;
}; // struct range_proxy




/**
 * @brief
 * Creates a enumerated iterator to iterator from first to last
 *
 *
 * \param first Starting iterator
 * \param last  End iterator which will not be included
 * \param initial Starting offset to start iterating from
 *
 * \tparam Iterator The iterator type of the container
 *
 * \returns A enumerate_proxy over all the iterators
 *
 * \code{.cpp}
 * std::vector<dtype> pvec(sz);
 * for (auto i : enumerate(pvec.begin(),pvec.end()) ){
 * 		int   index = std::get<0>(i);
 * 		dtype myval = std::get<1>(i);
 * }
 * \endcode
 *
 * \code{.cpp}
 * std::vector<dtype> pvec(sz);
 * for (auto i : enumerate(pvec.begin(),pvec.end(),start) ){
 * 		int   index = std::get<0>(i);
 * 		dtype myval = std::get<1>(i);
 * }
 * \endcode
 */
template<typename Iterator, typename Incrementable = std::size_t>
enumerate_proxy<Iterator, Incrementable>
enumerate(Iterator first, Iterator last, Incrementable initial = 0) {
	return {first, last, initial};
}

/**
 * @brief
 * Creates an enumerated iterator over all elements of type
 * with begin and end methods.
 *
 * \param content Container instance to iterate over
 *
 * \tparam Container type to iterate over
 *
 * \returns A enumerate_proxy over all the iterators
 *
 * \code{.cpp}
 * std::vector<dtype> pvec(sz);
 * for (auto i : enumerate(pvec) ){
 * 		int   index = std::get<0>(i);
 * 		dtype myval = std::get<1>(i);
 * }
 * \endcode
 *
 * \code{.cpp}
 * std::vector<dtype> pvec(sz);
 * for (auto i : enumerate(pvec).step(inc) ){
 * 		int   index = std::get<0>(i);
 * 		dtype myval = std::get<1>(i);
 * }
 * \endcode
 */
template<typename Container, typename = typename std::enable_if<::xstd::has_begin_end<Container>::value> >
auto enumerate(Container &content) -> enumerate_proxy<decltype(content.begin())> {
	return {content.begin(), content.end()};
}

/**
 * @brief
 * Creates an enumerated iterator over all elements of an initializer_list.
 *
 * \param content Container type to iterate over
 *
 * \tparam T Type contained within initializer_list
 *
 * \returns A enumerate_proxy over all the iterators
 *
 * \code{.cpp}
 * for (auto i : enumerate( {val1, val2, val3} ) ){
 * 		int   index = std::get<0>(i);
 * 		dtype myval = std::get<1>(i);
 * }
 * \endcode
 *
 * \code{.cpp}
 * for (auto i : enumerate( {val1, val2, val3} ).step(inc) ){
 * 		int   index = std::get<0>(i);
 * 		dtype myval = std::get<1>(i);
 * }
 * \endcode
 */
template<typename T>
auto enumerate(std::initializer_list<T>&& content) -> enumerate_proxy<decltype(std::begin(content))>{
	return {std::begin(content), std::end(content)};
}

/**
 * @brief
 * Creates an enumerated iterator over all elements of a static array.
 *
 * \param x Static array to iterate over
 *
 * \tparam T Type contained within static array
 *
 * \returns A enumerate_proxy over all the iterators
 *
 * \code{.cpp}
 * dtype vec[100];
 * for (auto i : enumerate(vec) ){
 * 		int   index = std::get<0>(i);
 * 		dtype myval = std::get<1>(i);
 * }
 * \endcode
 *
 * \code{.cpp}
 * dtype vec[100];
 * for (auto i : enumerate(vec).step(inc) ){
 * 		int   index = std::get<0>(i);
 * 		dtype myval = std::get<1>(i);
 * }
 * \endcode
 */
template <typename T, std::size_t N>
enumerate_proxy<T*> enumerate(T (&x)[N]){
	return {&(x[0]), &(x[0])+N};
}


} /* namespace xstd */


#endif /* INCLUDE_XSTD_ITERATOR_ENUMERATE_HPP_ */
