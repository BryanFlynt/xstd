/*
 * vector_math.hpp
 *
 *  Created on: May 21, 2019
 *      Author: bflynt
 */

#ifndef VECTOR_MATH_HPP_
#define VECTOR_MATH_HPP_

#include "xstd/assert.hpp"

#include <cstdlib>
#include <vector>
#include <type_traits>


/**
 * \file
 * vector_math.hpp
 *
 * \brief
 * Adds math operators to std::vector
 *
 * \details
 * Provides template functions for math operations on std::vector<>.
 * The operations are inefficient and the user should consider some
 * type of TMP lazy evaluation library if performance is a concern.
 * Furthermore, the std::vector<> operations have assume the default
 * allocator when returning a std::vector<> irrespective of the
 * allocator types of the std::vector passed in. *
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
template<typename T, typename A>
std::vector<T,A> operator -(const std::vector<T,A>& a) noexcept{
	std::vector<T,A> ans(a.size());
	for(std::size_t i = 0; i < a.size(); ++i){
		ans[i] = -a[i];
	}
	return ans;
}

template<typename T1, typename A1>
std::vector<T1,A1> operator +(const std::vector<T1,A1>& a) noexcept{
	return a;
}

// ============================================================
//                Vector / Scalar Operations
// ============================================================
//
// Note:
// Tested these loops using a standard index for loop (shown below)
// and the assembly was identical for gnu & clang.
// for(std::size_t i = 0; i < a.size(); ++i){
// }
//
template<typename T1, typename A1, typename T2>
void operator +=(std::vector<T1,A1>& a, const T2& b) noexcept{
	STATIC_ASSERT(std::is_convertible<T2,T1>::value, "Type Mismatch");
	for(auto& val : a){ // Compiler explorer shows exact code as loop
		val += b;
	}
}

template<typename T1, typename A1, typename T2>
void operator -=(std::vector<T1,A1>& a, const T2& b) noexcept{
	STATIC_ASSERT(std::is_convertible<T2,T1>::value, "Type Mismatch");
	for(auto& val : a){
		val -= b;
	}
}

template<typename T1, typename A1, typename T2>
void operator *=(std::vector<T1,A1>& a, const T2& b) noexcept{
	STATIC_ASSERT(std::is_convertible<T2,T1>::value, "Type Mismatch");
	for(auto& val : a){
		val *= b;
	}
}

template<typename T1, typename A1, typename T2>
void operator /=(std::vector<T1,A1>& a, const T2& b) noexcept{
	STATIC_ASSERT(std::is_convertible<T2,T1>::value, "Type Mismatch");
	for(auto& val : a){
		val /= b;
	}
}

template<typename T1, typename A1, typename T2>
std::vector<std::common_type_t<T1,T2>>
operator +(const std::vector<T1,A1>& a, const T2& b) noexcept{
	std::vector<std::common_type_t<T1,T2>> ans(a);
	ans += b;
	return ans;
}

template<typename T1, typename A1, typename T2>
std::vector<std::common_type_t<T1,T2>>
operator +(const T2& b, const std::vector<T1,A1>& a) noexcept{
	std::vector<std::common_type_t<T1,T2>> ans(a);
	ans += b;
	return ans;
}

template<typename T1, typename A1, typename T2>
std::vector<std::common_type_t<T1,T2>>
operator -(const std::vector<T1,A1>& a, const T2& b) noexcept{
	std::vector<std::common_type_t<T1,T2>> ans(a);
	ans -= b;
	return ans;
}

template<typename T1, typename A1, typename T2>
std::vector<std::common_type_t<T1,T2>>
operator -(const T2& b, const std::vector<T1,A1>& a) noexcept{
	std::vector<std::common_type_t<T1,T2>> ans(a.size(),b);
	ans -= a;
	return ans;
}

template<typename T1, typename A1, typename T2>
std::vector<std::common_type_t<T1,T2>>
operator *(const std::vector<T1,A1>& a, const T2& b) noexcept{
	std::vector<std::common_type_t<T1,T2>> ans(a);
	ans *= b;
	return ans;
}

template<typename T1, typename A1, typename T2>
std::vector<std::common_type_t<T1,T2>>
operator *(const T2& b, const std::vector<T1,A1>& a) noexcept{
	std::vector<std::common_type_t<T1,T2>> ans(a);
	ans *= b;
	return ans;
}

template<typename T1, typename A1, typename T2>
std::vector<std::common_type_t<T1,T2>>
operator /(const std::vector<T1,A1>& a, const T2& b) noexcept{
	std::vector<std::common_type_t<T1,T2>> ans(a);
	ans /= b;
	return ans;
}

template<typename T1, typename A1, typename T2>
std::vector<std::common_type_t<T1,T2>>
operator /(const T2& b, const std::vector<T1,A1>& a) noexcept{
	std::vector<std::common_type_t<T1,T2>> ans(a.size(),b);
	ans /= a;
	return ans;
}

// ============================================================
//                Vector / Vector Operations
// ============================================================


template<typename T1, typename A1, typename T2, typename A2>
void operator +=(std::vector<T1,A1>& a, const std::vector<T2,A2>& b) noexcept{
    STATIC_ASSERT(std::is_convertible<T2,T1>::value, "Type Mismatch");
    ASSERT(a.size() == b.size());
    for(std::size_t i = 0; i < a.size(); ++i){
        a[i] += b[i];
    }
}

template<typename T1, typename A1, typename T2, typename A2>
void operator -=(std::vector<T1,A1>& a, const std::vector<T2,A2>& b) noexcept{
    STATIC_ASSERT(std::is_convertible<T2,T1>::value, "Type Mismatch");
    ASSERT(a.size() == b.size());
    for(std::size_t i = 0; i < a.size(); ++i){
        a[i] -= b[i];
    }
}

template<typename T1, typename A1, typename T2, typename A2>
void operator *=(std::vector<T1,A1>& a, const std::vector<T2,A2>& b) noexcept{
    STATIC_ASSERT(std::is_convertible<T2,T1>::value, "Type Mismatch");
    ASSERT(a.size() == b.size());
    for(std::size_t i = 0; i < a.size(); ++i){
        a[i] *= b[i];
    }
}

template<typename T1, typename A1, typename T2, typename A2>
void operator /=(std::vector<T1,A1>& a, const std::vector<T2,A2>& b) noexcept{
    STATIC_ASSERT(std::is_convertible<T2,T1>::value, "Type Mismatch");
    ASSERT(a.size() == b.size());
    for(std::size_t i = 0; i < a.size(); ++i){
        a[i] /= b[i];
    }
}

template<typename T1, typename A1, typename T2, typename A2>
std::vector<std::common_type_t<T1,T2>>
operator +(const std::vector<T1,A1>& a, const std::vector<T2,A2>& b) noexcept{
	ASSERT(a.size() == b.size());
	std::vector<std::common_type_t<T1,T2>> ans(a);
	ans += b;
	return ans;
}

template<typename T1, typename A1, typename T2, typename A2>
std::vector<std::common_type_t<T1,T2>>
operator -(const std::vector<T1,A1>& a, const std::vector<T2,A2>& b) noexcept{
	ASSERT(a.size() == b.size());
	std::vector<std::common_type_t<T1,T2>> ans(a);
	ans -= b;
	return ans;
}

template<typename T1, typename A1, typename T2, typename A2>
std::vector<std::common_type_t<T1,T2>>
operator *(const std::vector<T1,A1>& a, const std::vector<T2,A2>& b) noexcept{
	ASSERT(a.size() == b.size());
	std::vector<std::common_type_t<T1,T2>> ans(a);
	ans *= b;
	return ans;
}

template<typename T1, typename A1, typename T2, typename A2>
std::vector<std::common_type_t<T1,T2>>
operator /(const std::vector<T1,A1>& a, const std::vector<T2,A2>& b) noexcept{
	ASSERT(a.size() == b.size());
	std::vector<std::common_type_t<T1,T2>> ans(a);
	ans /= b;
	return ans;
}

// ============================================================
//                Linear Algebra Operations
// ============================================================


template<typename T1, typename A1, typename T2, typename A2>
std::common_type_t<T1,T2>
dot_product(const std::vector<T1,A1>& a, const std::vector<T2,A2>& b) noexcept{
    ASSERT(a.size() == b.size());
    std::common_type_t<T1,T2> ans(0);
	for(std::size_t i = 0; i < a.size(); ++i){
		ans += (a[i] * b[i]);
	}
	return ans;
}

template<typename T1, typename A1, typename T2, typename A2>
std::vector<std::common_type_t<T1,T2>>
cross_product(const std::vector<T1,A1>& a, const std::vector<T2,A2>& b) noexcept{
    ASSERT(a.size() == b.size());
	ASSERT(a.size() == 3);
	std::vector<std::common_type_t<T1,T2>> ans(3);
	ans[0] = a[1]*b[2] - a[2]*b[1];
	ans[1] = a[2]*b[0] - a[0]*b[2];
	ans[2] = a[0]*b[1] - a[1]*b[0];
	return ans;
}

template<typename T, typename A>
T
norm1(const std::vector<T,A>& a) noexcept{
	using std::abs;
	T ans(0);
	for(std::size_t i = 0; i < a.size(); ++i){
		ans += abs(a[i]);
	}
	return ans;
}

template<typename T, typename A>
T
norm2(const std::vector<T,A>& a) noexcept{
	using std::sqrt;
	return sqrt(dot_product(a,a));
}

template<typename T, typename A>
T
norm_inf(const std::vector<T,A>& a) noexcept{
	using std::abs;
	using std::max;
	T ans(0);
	for(std::size_t i = 0; i < a.size(); ++i){
		ans = max(ans, abs(a[i]));
	}
	return ans;
}


} /* namespace xstd */



#endif /* VECTOR_MATH_HPP_ */
