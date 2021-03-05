/**
 * \file       polynomial.hpp
 * \author     Bryan Flynt
 * \date       Feb 07, 2021
 * \copyright  Copyright (C) 2021 Bryan Flynt - All Rights Reserved
 */
#pragma once

#include <cassert>
#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <numeric>
#include <ostream>
#include <type_traits>
#include <vector>

namespace xstd {

/// Polynomial
/**
 * Dense polynomial where very few coefficients are zero.
 * 
 * \tparam Coefficient Type to represent coefficients
 */
template <typename Coefficient>
class Polynomial {
    // ====================================================
    // Types
    // ====================================================
   private:
    using array_type = std::vector<Coefficient>;

   public:
    using size_type = typename array_type::size_type;
    using value_type = typename array_type::value_type;
    using exponent_type = typename array_type::size_type;
    using coefficient_type = typename array_type::value_type;

    // ====================================================
    // Constructors
    // ====================================================

    Polynomial();
    Polynomial(const Polynomial& other) = default;
    Polynomial(Polynomial&& other) = default;
    ~Polynomial() = default;

    Polynomial(const exponent_type& exp, const bool add_one = false);

    Polynomial(std::initializer_list<coefficient_type> terms);

    // ====================================================
    // Operators
    // ====================================================
    Polynomial& operator=(const Polynomial& other) = default;
    Polynomial& operator=(Polynomial&& other) = default;

    template <typename T>
    std::enable_if_t<std::is_constructible_v<Coefficient, T>, Polynomial&>
    operator+=(const T& value);

    template <typename T>
    std::enable_if_t<std::is_constructible_v<Coefficient, T>, Polynomial&>
    operator-=(const T& value);

    template <typename T>
    std::enable_if_t<std::is_constructible_v<Coefficient, T>, Polynomial&>
    operator*=(const T& value);

    template <typename T>
    std::enable_if_t<std::is_constructible_v<Coefficient, T>, Polynomial&>
    operator/=(const T& value);

    template <typename C>
    Polynomial& operator+=(const Polynomial<C>& value);

    template <typename C>
    Polynomial& operator-=(const Polynomial<C>& value);

    template <typename C>
    Polynomial& operator*=(const Polynomial<C>& value);

    template <typename C>
    Polynomial& operator/=(const Polynomial<C>& value);

    template <typename C>
    bool operator==(const Polynomial<C>& value);

    template <typename C>
    bool operator!=(const Polynomial<C>& value);

    // ====================================================
    // Modifyers
    // ====================================================

    void clear();

    void resize(const exponent_type& exp, const bool add_one = false);

    void insert(std::initializer_list<coefficient_type> terms);

    void erase(const exponent_type& exp);

    void differentiate();

    /// Integrate Polynomial
    /**
     * Chooses integration constant such that P(0) = 0.
     */
    void integrate();

    // ====================================================
    // Lookup
    // ====================================================

    size_type size() const;

    size_type degree() const;

    bool contains(const exponent_type& exp) const;

    coefficient_type coefficient(const exponent_type& exp) const;

    coefficient_type evaluate(const coefficient_type& value) const;

    void display(std::ostream& os) const noexcept;

    // ====================================================
    // [Private]
    // ====================================================

   private:
    array_type terms_;

    // Make other Polynomials friends of this one
    template <typename>
    friend class Polynomial;

    // Trim off 0's at end
    void normalize();
};

// ****************************************************
// Implementation
// ****************************************************

// ====================================================
// Constructors
// ====================================================

template <typename C>
Polynomial<C>::Polynomial() : terms_({0}){
}

template <typename C>
Polynomial<C>::Polynomial(const exponent_type& exp, const bool add_one) {
    this->resize(exp, add_one);
}

template <typename C>
Polynomial<C>::Polynomial(std::initializer_list<coefficient_type> terms) {
    this->insert(terms);
}

// ====================================================
// Operators
// ====================================================
template <typename C>
template <typename T>
std::enable_if_t<std::is_constructible_v<C, T>, Polynomial<C>&> Polynomial<C>::operator+=(const T& value) {
    terms_[0] += value_type(value);
    return *this;
}

template <typename C>
template <typename T>
std::enable_if_t<std::is_constructible_v<C, T>, Polynomial<C>&> Polynomial<C>::operator-=(const T& value) {
    terms_[0] -= value_type(value);
    return *this;
}

template <typename C>
template <typename T>
std::enable_if_t<std::is_constructible_v<C, T>, Polynomial<C>&> Polynomial<C>::operator*=(const T& value) {
    for (auto& c : terms_) {
        c *= coefficient_type(value);
    }
    this->normalize();
    return *this;
}

template <typename C>
template <typename T>
std::enable_if_t<std::is_constructible_v<C, T>, Polynomial<C>&> Polynomial<C>::operator/=(const T& value) {
    assert(value != coefficient_type(0));
    for (auto& c : terms_) {
        c /= coefficient_type(value);
    }
    return *this;
}

template <typename C>
template <typename CO>
Polynomial<C>& Polynomial<C>::operator+=(const Polynomial<CO>& value) {
    if (value.size() > this->size()) {
        terms_.resize(value.size(), value_type(0));
    }
    for (size_type i = 0; i < value.size(); ++i) {
        terms_[i] += value.terms_[i];
    }
    this->normalize();
    return *this;
}

template <typename C>
template <typename CO>
Polynomial<C>& Polynomial<C>::operator-=(const Polynomial<CO>& value) {
    if (value.size() > this->size()) {
        terms_.resize(value.size(), value_type(0));
    }
    for (size_type i = 0; i < value.size(); ++i) {
        terms_[i] -= value.terms_[i];
    }
    this->normalize();
    return *this;
}

template <typename C>
template <typename CO>
Polynomial<C>& Polynomial<C>::operator*=(const Polynomial<CO>& value) {
    array_type new_terms(this->degree() + value.degree()+1, 0);
    exponent_type this_exp(0);
    for (const auto& this_coef : terms_) {
        exponent_type other_exp(0);
        for (const auto& other_coef : value.terms_) {
            new_terms[this_exp + other_exp] += this_coef * other_coef;
            ++other_exp;
        }
        ++this_exp;
    }
    std::swap(new_terms, terms_);
    this->normalize();
    return *this;
}

template <typename C>
template <typename CO>
Polynomial<C>& Polynomial<C>::operator/=(const Polynomial<CO>& value) {
    assert(value.size() > 0);  // Divide by Zero

    this->normalize();
    return *this;
}

template <typename C>
template <typename CO>
bool Polynomial<C>::operator==(const Polynomial<CO>& value) {
    return (terms_ == value.terms_);
}

template <typename C>
template <typename CO>
bool Polynomial<C>::operator!=(const Polynomial<CO>& value) {
    return (terms_ != value.terms_);
}

// ====================================================
// Modifyers
// ====================================================
template <typename C>
void Polynomial<C>::clear() {
    terms_.clear();
}

template <typename C>
void Polynomial<C>::resize(const exponent_type& exp, const bool add_one) {
    terms_.clear();
    terms_.resize(exp + 1, 1);
    if (not add_one) {
        terms_[0] = coefficient_type(0);
    }
}

template <typename C>
void Polynomial<C>::insert(std::initializer_list<coefficient_type> terms) {
    terms_.insert(terms_.end(), terms.begin(), terms.end());
    this->normalize();
}

template <typename C>
void Polynomial<C>::erase(const exponent_type& exp) {
    if (exp <= this->degree()) {
        terms_[exp] = coefficient_type(0);
    }
    this->normalize();
}

template <typename C>
void Polynomial<C>::differentiate() {
    for (std::size_t i = 0; i < terms_.size() - 1; ++i) {
        terms_[i] = terms_[i + 1] * coefficient_type(i + 1);
    }
    terms_.pop_back();
    this->normalize();
}

template <typename C>
void Polynomial<C>::integrate() {
    terms_.push_back(0);
    for (std::size_t i = terms_.size(); i-- > 1;) {
        terms_[i] = terms_[i - 1] / coefficient_type(i);
    }
    terms_[0] = 0;
}

// ====================================================
// Lookup
// ====================================================
template <typename C>
typename Polynomial<C>::size_type
Polynomial<C>::size() const {
    return terms_.size();
}

template <typename C>
typename Polynomial<C>::size_type
Polynomial<C>::degree() const {
    return terms_.size() - 1;
}

template <typename C>
bool Polynomial<C>::contains(const exponent_type& exp) const {
    return ((degree() <= exp) and (terms_[exp] != coefficient_type(0)));
}

template <typename C>
typename Polynomial<C>::coefficient_type
Polynomial<C>::coefficient(const exponent_type& exp) const {
    return contains(exp) ? terms_[exp] : 0;
}

template <typename C>
typename Polynomial<C>::coefficient_type
Polynomial<C>::evaluate(const coefficient_type& value) const {
    using std::pow;
    coefficient_type answer(0);
    for (std::size_t i = 0; i < terms_.size(); ++i) {
        answer += terms_[i] * pow(value, i);
    }
    return answer;
}

template <typename C>
void Polynomial<C>::display(std::ostream& os) const noexcept {
    if (this->size() == 0) {
        os << coefficient_type(0.0);

    } else {
        os << terms_[0];
        for (std::size_t i = 1; i < terms_.size(); ++i) {
            os << " + " << terms_[i] << "x^" << i;
        }
    }
}

// ====================================================
// [Private]
// ====================================================

template <typename C>
void Polynomial<C>::normalize() {
    while( (terms_.back() == coefficient_type(0)) && (terms_.size() > 1) ){
        terms_.pop_back();
    }
}

// ====================================================
// [External Functions]
// ====================================================

template <typename C>
std::ostream& operator<<(std::ostream& os, const Polynomial<C>& poly) {
    poly.display(os);
    return os;
}

// Unary minus (negate).
template <class T>
Polynomial<T> operator-(Polynomial<T> a) {
    using coefficient_type = typename Polynomial<T>::coefficient_type;
    const coefficient_type negative_one(-1);
    a *= negative_one;
    return a;
}

template <class T>
inline Polynomial<T> operator+(const Polynomial<T>& a, const Polynomial<T>& b) {
    Polynomial<T> result(a);
    result += b;
    return result;
}

template <class T>
inline Polynomial<T> operator-(const Polynomial<T>& a, const Polynomial<T>& b) {
    Polynomial<T> result(a);
    result -= b;
    return result;
}

template <class T>
inline Polynomial<T> operator*(const Polynomial<T>& a, const Polynomial<T>& b) {
    Polynomial<T> result(a);
    result *= b;
    return result;
}

template <class T>
inline Polynomial<T> operator/(const Polynomial<T>& a, const Polynomial<T>& b) {
    Polynomial<T> result(a);
    result /= b;
    return result;
}

template <class T>
Polynomial<T> pow(const Polynomial<T>& base, int exp) {
    assert(exp >= 0);
    Polynomial<T> result({1});
    while(exp > 0) {
        result *= base;
        --exp;
    }
    return result;
}

}  // namespace xstd
