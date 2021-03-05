/**
 * \file       dual_number.hpp
 * \author     Bryan Flynt
 * \date       Feb 19, 2021
 * \copyright  Copyright (C) 2021 Bryan Flynt - All Rights Reserved
 */
#pragma once

#include <cmath>
#include <type_traits>

namespace xstd {

/// Dual number for 1st Derivatives
/**
 * The DualNumber will calculate the 1st Derivative of 
 * any scalar valued function. 
 * 
 * Example:
 * \code
 * template<typename T>
 * T my_func(const T& x){
 *  using std::pow;
 *  using std::sin;
 *  return 3*pow(x,3) + 2*sin(x) + 4;
 * }
 * 
 * DualNumber<double> x = {2.1415, 1.0};
 * DualNumber<double> ans = my_func(x);
 * double v = x.value();  // resulting value
 * double d = x.deriv();  // derivative of calculation
 * \endcode
 * 
 * \tparam T Data type of computations
 */
template <typename T>
class DualNumber {
    // ====================================================
    // Types
    // ====================================================
   public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;

    // ====================================================
    // Constructors
    // ====================================================

    DualNumber() = default;
    DualNumber(const DualNumber& other) = default;
    DualNumber(DualNumber&& other) = default;
    ~DualNumber() = default;

    DualNumber(const value_type& a);
    DualNumber(const value_type& a, const value_type& b);

    DualNumber(value_type&& a);
    DualNumber(value_type&& a, value_type&& b);

    // ====================================================
    // Operators
    // ====================================================

    DualNumber& operator=(const DualNumber& other) = default;
    DualNumber& operator=(DualNumber&& other) = default;

    DualNumber& operator=(const value_type& a);
    DualNumber& operator=(value_type&& a);

    DualNumber& operator+() const noexcept;
    DualNumber& operator-();

    template <typename Scalar>
    std::enable_if_t<std::is_constructible<T, Scalar>::value, DualNumber&>
    operator+=(const Scalar& value);

    template <typename Scalar>
    std::enable_if_t<std::is_constructible<T, Scalar>::value, DualNumber&>
    operator-=(const Scalar& value);

    template <typename Scalar>
    std::enable_if_t<std::is_constructible<T, Scalar>::value, DualNumber&>
    operator*=(const Scalar& value);

    template <typename Scalar>
    std::enable_if_t<std::is_constructible<T, Scalar>::value, DualNumber&>
    operator/=(const Scalar& value);

    template <typename OT>
    std::enable_if_t<std::is_constructible<T, OT>::value, DualNumber&>
    operator+=(const DualNumber<OT>& other);

    template <typename OT>
    std::enable_if_t<std::is_constructible<T, OT>::value, DualNumber&>
    operator-=(const DualNumber<OT>& other);

    template <typename OT>
    std::enable_if_t<std::is_constructible<T, OT>::value, DualNumber&>
    operator*=(const DualNumber<OT>& other);

    template <typename OT>
    std::enable_if_t<std::is_constructible<T, OT>::value, DualNumber&>
    operator/=(const DualNumber<OT>& other);

    // ====================================================
    // Modifyers
    // ====================================================

    constexpr void value(const_reference a) noexcept;

    constexpr void deriv(const_reference b) noexcept;

    // ====================================================
    // Access
    // ====================================================

    constexpr const_reference value() const noexcept;

    constexpr const_reference deriv() const noexcept;

    // ====================================================
    // [Private]
    // ====================================================

   private:
    value_type value_;
    value_type deriv_;

    // Make other DualNumber's friends of this one
    template <typename>
    friend class DualNumber;
};

// ****************************************************
// ****************************************************
// Implementation
// ****************************************************
// ****************************************************

// ====================================================
// Constructors
// ====================================================

template <typename T>
DualNumber<T>::DualNumber(const value_type& a)
    : value_(a), deriv_(T(0)) {
}

template <typename T>
DualNumber<T>::DualNumber(const value_type& a, const value_type& b)
    : value_(a), deriv_(b) {
}

template <typename T>
DualNumber<T>::DualNumber(value_type&& a)
    : value_(std::forward<value_type>(a)), deriv_(T(0)) {
}

template <typename T>
DualNumber<T>::DualNumber(value_type&& a, value_type&& b)
    : value_(std::forward<value_type>(a)), deriv_(std::forward<value_type>(b)) {
}

// ====================================================
// Operators
// ====================================================

template <typename T>
DualNumber<T>& DualNumber<T>::operator=(const value_type& a) {
    value_ = a;
    deriv_ = value_type(0);
}

template <typename T>
DualNumber<T>& DualNumber<T>::operator=(value_type&& a) {
    value_ = std::forward<value_type>(a);
    deriv_ = value_type(0);
}

template <typename T>
template <typename Scalar>
std::enable_if_t<std::is_constructible<T, Scalar>::value, DualNumber<T>&>
DualNumber<T>::operator+=(const Scalar& value) {
    value_ += value_type(value);
    return *this;
}

template <typename T>
template <typename Scalar>
std::enable_if_t<std::is_constructible<T, Scalar>::value, DualNumber<T>&>
DualNumber<T>::operator-=(const Scalar& value) {
    value_ -= value_type(value);
    return *this;
}

template <typename T>
template <typename Scalar>
std::enable_if_t<std::is_constructible<T, Scalar>::value, DualNumber<T>&>
DualNumber<T>::operator*=(const Scalar& value) {
    deriv_ *= value_type(value);
    value_ *= value_type(value);
    return *this;
}

template <typename T>
template <typename Scalar>
std::enable_if_t<std::is_constructible<T, Scalar>::value, DualNumber<T>&>
DualNumber<T>::operator/=(const Scalar& value) {
    deriv_ /= value_type(value);
    value_ /= value_type(value);
    return *this;
}

template <typename T>
template <typename OT>
std::enable_if_t<std::is_constructible<T, OT>::value, DualNumber<T>&> DualNumber<T>::operator+=(const DualNumber<OT>& other) {
    value_ += value_type(other.value_);
    deriv_ += value_type(other.deriv_);
    return *this;
}

template <typename T>
template <typename OT>
std::enable_if_t<std::is_constructible<T, OT>::value, DualNumber<T>&> DualNumber<T>::operator-=(const DualNumber<OT>& other) {
    value_ -= value_type(other.value_);
    deriv_ -= value_type(other.deriv_);
    return *this;
}

template <typename T>
template <typename OT>
std::enable_if_t<std::is_constructible<T, OT>::value, DualNumber<T>&> DualNumber<T>::operator*=(const DualNumber<OT>& other) {
    deriv_ *= value_type(other.value_);
    deriv_ += (value_ * value_type(other.deriv_));
    value_ *= value_type(other.value_);
    return *this;
}

template <typename T>
template <typename OT>
std::enable_if_t<std::is_constructible<T, OT>::value, DualNumber<T>&> DualNumber<T>::operator/=(const DualNumber<OT>& other) {
    deriv_ *= value_type(1) / value_type(other.value_);
    deriv_ += value_ * -value_type(other.deriv_ / (other.value_ * other.value_));
    value_ /= value_type(other.value_);
    return *this;
}

// ====================================================
// Modifyers
// ====================================================

template <typename T>
constexpr void DualNumber<T>::value(const_reference a) noexcept {
    value_ = a;
}

template <typename T>
constexpr void DualNumber<T>::deriv(const_reference b) noexcept {
    deriv_ = b;
}

// ====================================================
// Access
// ====================================================

template <typename T>
constexpr typename DualNumber<T>::const_reference DualNumber<T>::value() const noexcept {
    return value_;
}

template <typename T>
constexpr typename DualNumber<T>::const_reference DualNumber<T>::deriv() const noexcept {
    return deriv_;
}

// ****************************************************
// ****************************************************
// Non-Member Functions
// ****************************************************
// ****************************************************

template <typename S, typename T>
bool operator<(const S& value, const DualNumber<T>& rhs) {
    return value < rhs.value();
}

template <typename T, typename S>
bool operator<(const DualNumber<T>& rhs, const S& value) {
    return rhs.value() < value;
}

template <typename T1, typename T2>
bool operator<(const DualNumber<T1>& lhs, const DualNumber<T2>& rhs) {
    return lhs.value() < rhs.value();
}

template <typename S, typename T>
bool operator<=(const S& value, const DualNumber<T>& rhs) {
    return value <= rhs.value();
}

template <typename T, typename S>
bool operator<=(const DualNumber<T>& rhs, const S& value) {
    return rhs.value() <= value;
}

template <typename T1, typename T2>
bool operator<=(const DualNumber<T1>& lhs, const DualNumber<T2>& rhs) {
    return lhs.value() <= rhs.value();
}

template <typename S, typename T>
bool operator==(const S& value, const DualNumber<T>& rhs) {
    return value == rhs.value();
}

template <typename T, typename S>
bool operator==(const DualNumber<T>& rhs, const S& value) {
    return rhs.value() == value;
}

template <typename T1, typename T2>
bool operator==(const DualNumber<T1>& lhs, const DualNumber<T2>& rhs) {
    return lhs.value() == rhs.value();
}

template <typename S, typename T>
bool operator!=(const S& value, const DualNumber<T>& rhs) {
    return value != rhs.value();
}

template <typename T, typename S>
bool operator!=(const DualNumber<T>& rhs, const S& value) {
    return rhs.value() != value;
}

template <typename T1, typename T2>
bool operator!=(const DualNumber<T1>& lhs, const DualNumber<T2>& rhs) {
    return lhs.value() != rhs.value();
}

template <typename S, typename T>
bool operator>(const S& value, const DualNumber<T>& rhs) {
    return value > rhs.value();
}

template <typename T, typename S>
bool operator>(const DualNumber<T>& rhs, const S& value) {
    return rhs.value() > value;
}

template <typename T1, typename T2>
bool operator>(const DualNumber<T1>& lhs, const DualNumber<T2>& rhs) {
    return lhs.value() > rhs.value();
}

template <typename S, typename T>
bool operator>=(const S& value, const DualNumber<T>& rhs) {
    return value >= rhs.value();
}

template <typename T, typename S>
bool operator>=(const DualNumber<T>& rhs, const S& value) {
    return rhs.value() >= value;
}

template <typename T1, typename T2>
bool operator>=(const DualNumber<T1>& lhs, const DualNumber<T2>& rhs) {
    return lhs.value() >= rhs.value();
}

template <typename T>
DualNumber<T> operator+(const DualNumber<T>& rhs) {
    return rhs;
}

template <typename T>
DualNumber<T> operator-(const DualNumber<T>& rhs) {
    return DualNumber<T>(-rhs.value(), -rhs.deriv());
}

template <typename S, typename T>
DualNumber<typename std::common_type<T, S>::type> operator+(const S& value, const DualNumber<T>& rhs) {
    DualNumber<typename std::common_type<T, S>::type> ans(value, 0);
    ans += rhs;
    return ans;
}

template <typename T, typename S>
DualNumber<typename std::common_type<T, S>::type> operator+(const DualNumber<T>& lhs, const S& value) {
    DualNumber<typename std::common_type<T, S>::type> ans(lhs);
    ans += value;
    return ans;
}

template <typename T1, typename T2>
DualNumber<typename std::common_type<T1, T2>::type> operator+(const DualNumber<T1>& lhs, const DualNumber<T2>& rhs) {
    DualNumber<typename std::common_type<T1, T2>::type> ans(lhs);
    ans += rhs;
    return ans;
}

template <typename S, typename T>
DualNumber<typename std::common_type<T, S>::type> operator-(const S& value, const DualNumber<T>& rhs) {
    DualNumber<typename std::common_type<T, S>::type> ans(value, 0);
    ans -= rhs;
    return ans;
}

template <typename T, typename S>
DualNumber<typename std::common_type<T, S>::type> operator-(const DualNumber<T>& lhs, const S& value) {
    DualNumber<typename std::common_type<T, S>::type> ans(lhs);
    ans -= value;
    return ans;
}

template <typename T1, typename T2>
DualNumber<typename std::common_type<T1, T2>::type> operator-(const DualNumber<T1>& lhs, const DualNumber<T2>& rhs) {
    DualNumber<typename std::common_type<T1, T2>::type> ans(lhs);
    ans -= rhs;
    return ans;
}

template <typename S, typename T>
DualNumber<typename std::common_type<T, S>::type> operator*(const S& value, const DualNumber<T>& rhs) {
    DualNumber<typename std::common_type<T, S>::type> ans(value, 0);
    ans *= rhs;
    return ans;
}

template <typename T, typename S>
DualNumber<typename std::common_type<T, S>::type> operator*(const DualNumber<T>& lhs, const S& value) {
    DualNumber<typename std::common_type<T, S>::type> ans(lhs);
    ans *= value;
    return ans;
}

template <typename T1, typename T2>
DualNumber<typename std::common_type<T1, T2>::type> operator*(const DualNumber<T1>& lhs, const DualNumber<T2>& rhs) {
    DualNumber<typename std::common_type<T1, T2>::type> ans(lhs);
    ans *= rhs;
    return ans;
}

template <typename S, typename T>
DualNumber<typename std::common_type<T, S>::type> operator/(const S& value, const DualNumber<T>& rhs) {
    DualNumber<typename std::common_type<T, S>::type> ans(value, 0);
    ans /= rhs;
    return ans;
}

template <typename T, typename S>
DualNumber<typename std::common_type<T, S>::type> operator/(const DualNumber<T>& lhs, const S& value) {
    DualNumber<typename std::common_type<T, S>::type> ans(lhs);
    ans /= value;
    return ans;
}

template <typename T1, typename T2>
DualNumber<typename std::common_type<T1, T2>::type> operator/(const DualNumber<T1>& lhs, const DualNumber<T2>& rhs) {
    DualNumber<typename std::common_type<T1, T2>::type> ans(lhs);
    ans /= rhs;
    return ans;
}

template <typename T>
DualNumber<T> abs(const DualNumber<T>& dual) {
    return (dual.value() < T(0)) ? dual : -dual;
}

template <typename T>
DualNumber<T> exp(const DualNumber<T>& dual) {
    using std::exp;
    auto r = exp(dual.value());
    auto i = r * dual.deriv();
    return DualNumber<T>(std::move(r), std::move(i));
}

template <typename T>
DualNumber<T> log(const DualNumber<T>& dual) {
    using std::log;
    auto r = log(dual.value());
    auto i = dual.deriv() / dual.value();
    return DualNumber<T>(std::move(r), std::move(i));
}

template <typename T>
DualNumber<T> log10(const DualNumber<T>& dual) {
    using std::log;
    using std::log10;
    auto r = log10(dual.value());
    auto i = dual.deriv() / (dual.value() * log(T(10)));
    return DualNumber<T>(std::move(r), std::move(i));
}

template <typename T, typename S>
DualNumber<T> pow(const DualNumber<T>& dual, const S& exponent) {
    using std::pow;
    auto r = pow(dual.value(), exponent);
    auto i = dual.deriv() * exponent * pow(dual.value(), exponent - 1);
    return DualNumber<T>(std::move(r), std::move(i));
}

template <typename T, typename S>
DualNumber<T> sqrt(const DualNumber<T>& dual) {
    using std::sqrt;
    auto r = sqrt(dual.value());
    auto i = dual.deriv() / (T(2) * r);
    return DualNumber<T>(std::move(r), std::move(i));
}

template <typename T, typename S>
DualNumber<T> sin(const DualNumber<T>& dual) {
    using std::cos;
    using std::sin;
    auto r = sin(dual.value());
    auto i = dual.deriv() * cos(dual.value());
    return DualNumber<T>(std::move(r), std::move(i));
}

template <typename T, typename S>
DualNumber<T> cos(const DualNumber<T>& dual) {
    using std::cos;
    using std::sin;
    auto r = cos(dual.value());
    auto i = dual.deriv() * -sin(dual.value());
    return DualNumber<T>(std::move(r), std::move(i));
}

template <typename T, typename S>
DualNumber<T> tan(const DualNumber<T>& dual) {
    using std::tan;
    auto r = tan(dual.value());
    auto i = dual.deriv() * (T(1) + r * r);
    return DualNumber<T>(std::move(r), std::move(i));
}

template <typename T, typename S>
DualNumber<T> asin(const DualNumber<T>& dual) {
    using std::asin;
    using std::sqrt;
    auto r = asin(dual.value());
    auto i = dual.deriv() / sqrt(T(1) - dual.value() * dual.value());
    return DualNumber<T>(std::move(r), std::move(i));
}

template <typename T, typename S>
DualNumber<T> acos(const DualNumber<T>& dual) {
    using std::acos;
    auto r = acos(dual.value());
    auto i = -dual.deriv() / sqrt(T(1) - dual.value() * dual.value());
    return DualNumber<T>(std::move(r), std::move(i));
}

template <typename T, typename S>
DualNumber<T> atan(const DualNumber<T>& dual) {
    using std::atan;
    auto r = atan(dual.value());
    auto i = dual.deriv() / (T(1) + dual.value() * dual.value());
    return DualNumber<T>(std::move(r), std::move(i));
}

template <typename T, typename S>
DualNumber<T> sinh(const DualNumber<T>& dual) {
    using std::cosh;
    using std::sinh;
    auto r = sinh(dual.value());
    auto i = dual.deriv() * cosh(dual.value());
    return DualNumber<T>(std::move(r), std::move(i));
}

template <typename T, typename S>
DualNumber<T> cosh(const DualNumber<T>& dual) {
    using std::cosh;
    using std::sinh;
    auto r = cosh(dual.value());
    auto i = dual.deriv() * sinh(dual.value());
    return DualNumber<T>(std::move(r), std::move(i));
}

template <typename T, typename S>
DualNumber<T> tanh(const DualNumber<T>& dual) {
    using std::tanh;
    auto r = tanh(dual.value());
    auto i = dual.deriv() * (T(1) - r * r);
    return DualNumber<T>(std::move(r), std::move(i));
}

template <typename T, typename S>
DualNumber<T> asinh(const DualNumber<T>& dual) {
    using std::asinh;
    using std::sqrt;
    auto r = asinh(dual.value());
    auto i = dual.deriv() / sqrt(dual.value() * dual.value() + T(1));
    return DualNumber<T>(std::move(r), std::move(i));
}

template <typename T, typename S>
DualNumber<T> acosh(const DualNumber<T>& dual) {
    using std::acosh;
    auto r = acosh(dual.value());
    auto i = dual.deriv() / sqrt(dual.value() * dual.value() - T(1));
    return DualNumber<T>(std::move(r), std::move(i));
}

template <typename T, typename S>
DualNumber<T> atanh(const DualNumber<T>& dual) {
    using std::atanh;
    auto r = atanh(dual.value());
    auto i = dual.deriv() / (T(1) - dual.value() * dual.value());
    return DualNumber<T>(std::move(r), std::move(i));
}

}  // namespace xstd