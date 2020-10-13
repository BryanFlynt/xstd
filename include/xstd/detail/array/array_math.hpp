/*
 * array_math.hpp
 *
 *  Created on: May 21, 2019
 *      Author: bflynt
 */

#ifndef ARRAY_MATH_HPP_
#define ARRAY_MATH_HPP_

#include "xstd/assert.hpp"


#include <array>
#include <cstdlib>
#include <type_traits>

/**
 * \file
 * array_math.hpp
 *
 * \brief
 * Adds math operators to std::array
 *
 * \details
 * Provides template functions for math operations on std::array<>.
 * The operations are inefficient and the user should consider some
 * type of TMP lazy evaluation library if performance is a concern.
 *
 * Note:
 * All functions are added into xstd namespace so any code using the
 * operations should declare "using namespace xstd;" within the
 * function where they are used.
 */

namespace xstd {

// ============================================================
//                    Unary Operations
// ============================================================
template<typename T, std::size_t N>
std::array<T,N> operator -(const std::array<T,N>& a) noexcept{
	std::array<T,N> ans;
	for(std::size_t i = 0; i < N; ++i){
		ans[i] = -a[i];
	}
	return ans;
}

template<typename T1, std::size_t N>
std::array<T1,N> operator +(const std::array<T1,N>& a) noexcept{
	return a;
}

// ============================================================
//                Array / Scalar Operations
// ============================================================

template<typename T1, typename T2, std::size_t N>
void operator +=(std::array<T1,N>& a, const T2& b) noexcept{
	STATIC_ASSERT(std::is_convertible<T2,T1>::value, "Type Mismatch");
	for(std::size_t i = 0; i < N; ++i){
		a[i] += b;
	}
}

template<typename T1, typename T2, std::size_t N>
void operator -=(std::array<T1,N>& a, const T2& b) noexcept{
	STATIC_ASSERT(std::is_convertible<T2,T1>::value, "Type Mismatch");
	for(std::size_t i = 0; i < N; ++i){
		a[i] -= b;
	}
}

template<typename T1, typename T2, std::size_t N>
void operator *=(std::array<T1,N>& a, const T2& b) noexcept{
	STATIC_ASSERT(std::is_convertible<T2,T1>::value, "Type Mismatch");
	for(std::size_t i = 0; i < N; ++i){
		a[i] *= b;
	}
}

template<typename T1, typename T2, std::size_t N>
void operator /=(std::array<T1,N>& a, const T2& b) noexcept{
	STATIC_ASSERT(std::is_convertible<T2,T1>::value, "Type Mismatch");
	for(std::size_t i = 0; i < N; ++i){
		a[i] /= b;
	}
}

template<typename T1, typename T2, std::size_t N>
std::array<std::common_type_t<T1,T2>,N>
operator +(const std::array<T1,N>& a, const T2& b) noexcept{
	std::array<std::common_type_t<T1,T2>,N> ans(a);
	ans += b;
	return ans;
}

template<typename T1, typename T2, std::size_t N>
std::array<std::common_type_t<T1,T2>,N>
operator +(const T2& b, const std::array<T1,N>& a) noexcept{
	std::array<std::common_type_t<T1,T2>,N> ans(a);
	ans += b;
	return ans;
}

template<typename T1, typename T2, std::size_t N>
std::array<std::common_type_t<T1,T2>,N>
operator -(const std::array<T1,N>& a, const T2& b) noexcept{
	std::array<std::common_type_t<T1,T2>,N> ans(a);
	ans -= b;
	return ans;
}

template<typename T1, typename T2, std::size_t N>
std::array<std::common_type_t<T1,T2>,N>
operator -(const T2& b, const std::array<T1,N>& a) noexcept{
	std::array<std::common_type_t<T1,T2>,N> ans(-a);
	ans += b;
	return ans;
}

template<typename T1, typename T2, std::size_t N>
std::array<std::common_type_t<T1,T2>,N>
operator *(const std::array<T1,N>& a, const T2& b) noexcept{
	std::array<std::common_type_t<T1,T2>,N> ans(a);
	ans *= b;
	return ans;
}

template<typename T1, typename T2, std::size_t N>
std::array<std::common_type_t<T1,T2>,N>
operator *(const T2& b, const std::array<T1,N>& a) noexcept{
	std::array<std::common_type_t<T1,T2>,N> ans(a);
	ans *= b;
	return ans;
}

template<typename T1, typename T2, std::size_t N>
std::array<std::common_type_t<T1,T2>,N>
operator /(const std::array<T1,N>& a, const T2& b) noexcept{
	std::array<std::common_type_t<T1,T2>,N> ans(a);
	ans /= b;
	return ans;
}

template<typename T1, std::size_t N, typename T2>
std::array<std::common_type_t<T1,T2>,N>
operator /(const T2& b, const std::array<T1,N>& a) noexcept{
	std::array<std::common_type_t<T1,T2>,N> ans;
	ans.fill(b);
	ans /= a;
	return ans;
}


// ============================================================
//                Array / Array Operations
// ============================================================


template<typename T1, typename T2, std::size_t N>
void operator +=(std::array<T1,N>& a, const std::array<T2,N>& b) noexcept{
	STATIC_ASSERT(std::is_convertible<T2,T1>::value, "Type Mismatch");
	for(std::size_t i = 0; i < N; ++i){
		a[i] += b[i];
	}
}

template<typename T1, typename T2, std::size_t N>
void operator -=(std::array<T1,N>& a, const std::array<T2,N>& b) noexcept{
	STATIC_ASSERT(std::is_convertible<T2,T1>::value, "Type Mismatch");
	for(std::size_t i = 0; i < N; ++i){
		a[i] -= b[i];
	}
}

template<typename T1, typename T2, std::size_t N>
void operator *=(std::array<T1,N>& a, const std::array<T2,N>& b) noexcept{
	STATIC_ASSERT(std::is_convertible<T2,T1>::value, "Type Mismatch");
	for(std::size_t i = 0; i < N; ++i){
		a[i] *= b[i];
	}
}

template<typename T1, typename T2, std::size_t N>
void operator /=(std::array<T1,N>& a, const std::array<T2,N>& b) noexcept{
	STATIC_ASSERT(std::is_convertible<T2,T1>::value, "Type Mismatch");
	for(std::size_t i = 0; i < N; ++i){
		a[i] /= b[i];
	}
}

template<typename T1, typename T2, std::size_t N>
std::array<std::common_type_t<T1,T2>,N>
operator +(const std::array<T1,N>& a, const std::array<T2,N>& b) noexcept{
	std::array<std::common_type_t<T1,T2>,N> ans(a);
	ans += b;
	return ans;
}

template<typename T1, typename T2, std::size_t N>
std::array<std::common_type_t<T1,T2>,N>
operator -(const std::array<T1,N>& a, const std::array<T2,N>& b) noexcept{
	std::array<std::common_type_t<T1,T2>,N> ans(a);
	ans -= b;
	return ans;
}

template<typename T1, typename T2, std::size_t N>
std::array<std::common_type_t<T1,T2>,N>
operator *(const std::array<T1,N>& a, const std::array<T2,N>& b) noexcept{
	std::array<std::common_type_t<T1,T2>,N> ans(a);
	ans *= b;
	return ans;
}

template<typename T1, typename T2, std::size_t N>
std::array<std::common_type_t<T1,T2>,N>
operator /(const std::array<T1,N>& a, const std::array<T2,N>& b) noexcept{
	std::array<std::common_type_t<T1,T2>,N> ans(a);
	ans /= b;
	return ans;
}


// ============================================================
//                Linear Algebra Operations
// ============================================================


template<typename T1, typename T2, std::size_t N>
std::common_type_t<T1,T2>
dot_product(const std::array<T1,N>& a, const std::array<T2,N>& b) noexcept{
	std::common_type_t<T1,T2> ans(0);
	for(std::size_t i = 0; i < N; ++i){
		ans += (a[i] * b[i]);
	}
	return ans;
}

template<typename T1, typename T2, std::size_t N>
std::array<std::common_type_t<T1,T2>,N>
cross_product(const std::array<T1,N>& a, const std::array<T2,N>& b) noexcept{
	STATIC_ASSERT(3 == N, "Must be Size 3");
	std::array<std::common_type_t<T1,T2>,N> ans;
	ans[0] = a[1]*b[2] - a[2]*b[1];
	ans[1] = a[2]*b[0] - a[0]*b[2];
	ans[2] = a[0]*b[1] - a[1]*b[0];
	return ans;
}

template<typename T, std::size_t N>
T
norm1(const std::array<T,N>& a) noexcept{
	using std::abs;
	T ans(0);
	for(std::size_t i = 0; i < N; ++i){
		ans += abs(a[i]);
	}
	return ans;
}

template<typename T, std::size_t N>
T
norm2(const std::array<T,N>& a) noexcept{
	using std::sqrt;
	return sqrt(dot_product(a,a));
}

template<typename T, std::size_t N>
T
norm_inf(const std::array<T,N>& a) noexcept{
	using std::abs;
	using std::max;
	T ans(0);
	for(std::size_t i = 0; i < N; ++i){
		ans = max(ans, abs(a[i]));
	}
	return ans;
}




} /* namespace xstd */


#endif /* ARRAY_MATH_HPP_ */
