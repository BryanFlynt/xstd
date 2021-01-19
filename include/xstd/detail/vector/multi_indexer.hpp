/**
 * \file       multi_indexer.hpp
 * \author     Bryan Flynt
 * \date       Jan 18, 2021
 * \copyright  Copyright (C) 2021 Bryan Flynt - All Rights Reserved
 */

#ifndef INCLUDE_XSTD_DETAIL_VECTOR_MULTI_INDEXER_HPP_
#define INCLUDE_XSTD_DETAIL_VECTOR_MULTI_INDEXER_HPP_


#include "xstd/assert.hpp"

#include <array>
#include <cstddef>


namespace xstd {

template<std::size_t N>
class RowMajorIndex final {
	static_assert(N > 0, "Cannot have 0 Rank Index");

public:

	// ====================================================
	// Types
	// ====================================================

	using size_type  = std::size_t;

	// ====================================================
	// Constructors
	// ====================================================

	RowMajorIndex()                             = delete;
	RowMajorIndex(const RowMajorIndex& other)   = default;
	RowMajorIndex(RowMajorIndex&& other)        = default;
	~RowMajorIndex()                            = default;

	RowMajorIndex(const std::array<size_type,N> shape) :
		shapes_(shape),
		linear_index_(0){
		this->calc_stride_();
		this->calc_index_();
	}

	// ====================================================
	// Operators
	// ====================================================

	RowMajorIndex& operator=(const RowMajorIndex& other) = default;
	RowMajorIndex& operator=(RowMajorIndex&& other)      = default;


	RowMajorIndex& operator=(const size_type& index){
		ASSERT(index < size());
		linear_index_ = index;
		this->calc_index_();
		return *this;
	}

	RowMajorIndex& operator++(){
		ASSERT(linear_index_ < (size()-1));
		linear_index_++;
		this->calc_index_();
		return *this;
	}

	RowMajorIndex operator++(int){
		ASSERT(linear_index_ < (size()-1));
		auto tmp = *this;
		linear_index_++;
		this->calc_index_();
		return tmp;
	}

	RowMajorIndex& operator--(){
		ASSERT(linear_index_ > 0);
		linear_index_--;
		this->calc_index_();
		return *this;
	}

	RowMajorIndex operator--(int){
		ASSERT(linear_index_ > 0);
		auto tmp = *this;
		linear_index_--;
		this->calc_index_();
		return tmp;
	}

	template<typename... Dims>
	size_type operator()(const Dims... args) {
		static_assert(sizeof...(args) == N);
		indexes_ = {static_cast<size_type>(args)...};
		linear_index_ = 0;
		for(size_type i = 0; i < N; ++i) {
			ASSERT(indexes_[i] < shapes_[i]);
			linear_index_ += indexes_[i] * strides_[i];
		}
		return linear_index_;
	}

	size_type operator[](const size_type rank) const {
		ASSERT(rank < N);
		return indexes_[rank];
	}

	// ====================================================
	// Conversion
	// ====================================================

	/** Implicit conversion to single index
	 */
	operator size_type() const {
		return linear_index_;
	}


	// ====================================================
	// Query
	// ====================================================

	size_type size() const {
		size_type sz = 1;
		for(size_type i = 0; i < N; ++i) {
			sz *= shapes_[i];
		}
		return sz;
	}

	size_type shape(const size_type i) const {
		ASSERT(i < N);
		return shapes_[i];
	}

	size_type stride(const size_type i) const {
		ASSERT(i < N);
		return strides_[i];
	}

	size_type index(const size_type i) const {
		ASSERT(i < N);
		return indexes_[i];
	}


	// ====================================================
	// PRIVATE
	// ====================================================

private:
	std::array<size_type,N> shapes_;
	std::array<size_type,N> strides_;
	std::array<size_type,N> indexes_;
	size_type               linear_index_;


	void calc_stride_() {
		strides_[N-1] = 1;
		for(size_type i = (N-1); i--> 0;) {
			strides_[i] = strides_[i+1] * shapes_[i+1];
		}
	}

	void calc_index_() {
		size_type fac = 1;
		for(size_type i = N; i--> 1;) {
			indexes_[i] = (linear_index_ / fac) % shapes_[i];
			fac *= shapes_[i];
		}
		indexes_[0] = linear_index_ / fac;
	}

};


} /* namespace xstd */


#endif /* INCLUDE_XSTD_DETAIL_VECTOR_MULTI_INDEXER_HPP_ */
