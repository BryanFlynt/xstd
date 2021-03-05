/**
 * \file       multivariate_polynomial.hpp
 * \author     Bryan Flynt
 * \date       Feb 07, 2021
 * \copyright  Copyright (C) 2021 Bryan Flynt - All Rights Reserved
 */
#pragma once

#include <cstddef>
#include <initializer_list>
#include <numeric>
#include <type_traits>
#include <vector>

namespace tbox {

/// Multivariate Polynomial
/**
 * 
 * \tparam NDim Number of polynomial dimensions
 * \tparam Coefficient Type to represent coefficients
 * \tparam Exponent Type to represent exponent powers
 */
template <std::size_t NDim, typename Coefficient = double, typename Exponent = std::size_t>
class MultiPolynomial {
    using exponent_array = std::array<Exponent, NDim>;
    using map_type = std::map<exponent_array, Coefficient>;

    // ====================================================
    // Types
    // ====================================================
   public:
    static constexpr std::size_t num_dimension = NDim;
    using exponent_type = typename exponent_array::value_type;
    using coefficient_type = typename map_type::mapped_type;
    using term_type = typename map_type::value_type; // std::pair<exponent_array,coefficient_type>

    // ====================================================
    // Constructors
    // ====================================================

    MultiPolynomial() = default;
    MultiPolynomial(const MultiPolynomial& other) = default;
    MultiPolynomial(MultiPolynomial&& other) = default;
    ~MultiPolynomial() = default;

    MultiPolynomial(const std::size_t& exp); // No 0 polynomial

    MultiPolynomial(std::initializer_list<term_type> terms);

    // ====================================================
    // Operators
    // ====================================================
    MultiPolynomial& operator=(const MultiPolynomial& other) = default;
    MultiPolynomial& operator=(MultiPolynomial&& other) = default;

    template <typename T>
    std::enable_if_t<std::is_constructible_v<Coefficient, T>, MultiPolynomial&>
    operator+=(const T& value);

    template <typename T>
    std::enable_if_t<std::is_constructible_v<Coefficient, T>, MultiPolynomial&>
    operator-=(const T& value);

    template <typename T>
    std::enable_if_t<std::is_constructible_v<Coefficient, T>, MultiPolynomial&>
    operator*=(const T& value);

    template <typename T>
    std::enable_if_t<std::is_constructible_v<Coefficient, T>, MultiPolynomial&>
    operator/=(const T& value);

    template <std::size_t DO, typename CO, typename EO>
    MultiPolynomial& operator+=(const MultiPolynomial<DO, CO, EO>& value);

    template <std::size_t DO, typename CO, typename EO>
    MultiPolynomial& operator-=(const MultiPolynomial<DO, CO, EO>& value);

    template <std::size_t DO, typename CO, typename EO>
    MultiPolynomial& operator*=(const MultiPolynomial<DO, CO, EO>& value);

    template <std::size_t DO, typename CO, typename EO>
    MultiPolynomial& operator/=(const MultiPolynomial<DO, CO, EO>& value);

    // ====================================================
    // Modifyers
    // ====================================================

    void clear();

    void resize(const std::size_t exp);

    void insert(const term_type& exps);

    void erase(const exponent_array& exps);

    void differentiate(const std::size_t dim);

    /// Integrate MultiPolynomial
    /**
     * Chooses integration constant such that P(0) = 0.
     */
    void integrate(const std::size_t dim);

    // ====================================================
    // Lookup
    // ====================================================

    size_type size() const;

    size_type degree() const;

    bool contains(const exponent_array& exp) const;

    coefficient_type coefficient(const exponent_array& exp) const;

    template<typename... Args>
    coefficient_type evaluate(const Args... args) const;

    // ====================================================
    // [Private]
    // ====================================================

   private:
    map_type terms_;

    // Make other MultiPolynomials friends of this one
    template <typename, typename>
    friend class MultiPolynomial;

    /// Remove all terms with zero coefficients
    void normalize();
};

// ****************************************************
// Implementation
// ****************************************************

namespace detail {

}  // namespace detail


// ====================================================
// Constructors
// ====================================================
template <std::size_t D, typename C, typename E>
MultiPolynomial<D, C, E>::MultiPolynomial(const std::size_t& exp) {
    this->resize(exp);
}

template <std::size_t D, typename C, typename E>
MultiPolynomial<D, C, E>::MultiPolynomial(std::initializer_list<term_type> terms) {
    this->insert(terms);
}

// ====================================================
// Operators
// ====================================================
template <std::size_t D, typename C, typename E>
template <typename T>
std::enable_if_t<is_constructible_v<C, T>, MultiPolynomial&> MultiPolynomial<D, C, E>& MultiPolynomial<D, C, E>::operator+=(const T& value) {
    if (value != static_cast<coefficient_type>(0)) {
        exponent_array zero_exps;
        zero_exps.fill(0);
        auto term_iter = terms_.find(zero_exps);
        if (terms_.cend() == term_iter) {
            terms_.emplace(zero_exps, value);
        } else {
            term_iter->second += value;
            if (term_iter->second == static_cast<coefficient_type>(0)) {
                terms_.erase(term_iter);
            }
        }
    }
    return *this;
}

template <std::size_t D, typename C, typename E>
template <typename T>
std::enable_if_t<std::is_constructible_v<C, T>, MultiPolynomial&> MultiPolynomial<D, C, E>& MultiPolynomial<D, C, E>::operator-=(const T& value) {
    if (value != static_cast<coefficient_type>(0)) {
        exponent_array zero_exps;
        zero_exps.fill(0);
        auto term_iter = terms_.find(zero_exps);
        if (terms_.cend() == term_iter) {
            terms_.emplace(zero_exps, -value);
        } else {
            term_iter->second -= value;
            if (term_iter->second == static_cast<coefficient_type>(0)) {
                terms_.erase(term_iter);
            }
        }
    }
    return *this;
}

template <std::size_t D, typename C, typename E>
template <typename T>
std::enable_if_t<std::is_constructible_v<C, T>, MultiPolynomial&> MultiPolynomial<D, C, E>& MultiPolynomial<D, C, E>::operator*=(const T& value) {
    if (value == coefficient_type(0)) {
        this->clear();
    } else {
        for (auto& [exp, coef] : terms_) {
            coef *= coefficient_type(value);
        }
    }
    return *this;
}

template <std::size_t D, typename C, typename E>
template <typename T>
std::enable_if_t<std::is_constructible_v<C, T>, MultiPolynomial&> MultiPolynomial<D, C, E>& MultiPolynomial<D, C, E>::operator/=(const T& value) {
    assert(value != coefficient_type(0));
    for (auto& [exp, coef] : terms_) {
        coef /= coefficient_type(value);
    }
}

template <std::size_t D, typename C, typename E>
template <std::size_t DO, typename CO, typename EO>
MultiPolynomial<D, C, E>& MultiPolynomial<D, C, E>::operator+=(const MultiPolynomial<DO,CO, EO>& value) {
    static_assert(DO == D, "Cannot add different dimensional polynomial");
    for (const auto& [exp, coef] : value.terms_) {
        auto term_iter = terms_.find(exp);
        if (terms_.cend() == term_iter) {
            terms_.emplace(exp, coef);
        } else {
            term_iter->second += coef;
            if (term_iter->second == coefficient_type(0)) {
                terms_.erase(term_iter);
            }
        }
    }
}

template <std::size_t D, typename C, typename E>
template <std::size_t DO, typename CO, typename EO>
MultiPolynomial<D, C, E>& MultiPolynomial<D, C, E>::operator-=(const MultiPolynomial<DO,CO, EO>& value) {
    static_assert(DO == D, "Cannot subtract different dimensional polynomial");
    for (const auto& [exp, coef] : value.terms_) {
        auto term_iter = terms_.find(exp);
        if (terms_.cend() == term_iter) {
            terms_.emplace(exp, -coef);
        } else {
            term_iter->second -= coef;
            if (term_iter->second == coefficient_type(0)) {
                terms_.erase(term_iter);
            }
        }
    }
}

template <std::size_t D, typename C, typename E>
template <std::size_t DO, typename CO, typename EO>
MultiPolynomial<D, C, E>& MultiPolynomial<D, C, E>::operator*=(const MultiPolynomial<DO, CO, EO>& value) {
    static_assert(DO == D, "Cannot multiply different dimensional polynomial");
    map_type new_terms;
    for (const auto& [orig_exp, orig_coef] : terms_) {
        for (const auto& [other_exp, other_coef] : value.terms_) {
            exponent_array new_exp = orig_exp;
            for(std::size_t i = 0; i < new_exp.size(); ++i){
                new_exp[i] += exponent_type(other_exp[i]);
            }
            auto term_iter = new_terms.find(new_exp);
            if (new_terms.cend() == term_iter) {
                new_terms_.emplace(std::move(new_exp), orig_coef * other_coef);
            } else {
                term_iter->second += orig_coef * coefficient_type(other_coef);
                if (term_iter->second == coefficient_type(0)) {
                    new_terms.erase(term_iter);
                }
            }
        }
    }
    std::swap(new_terms, terms_);
    return *this;
}

template <std::size_t D, typename C, typename E>
template <std::size_t DO, typename CO, typename EO>
MultiPolynomial<D, C, E>& MultiPolynomial<D, C, E>::operator/=(const MultiPolynomial<DO,CO, EO>& value) {
    static_assert(DO == D, "Cannot divide different dimensional polynomial");
    assert(value.size() > 0);  // Divide by Zero
    assert( false && "Not Implemented Yet"); // TODO
    return *this;
}

// ====================================================
// Modifyers
// ====================================================
template <std::size_t D, typename C, typename E>
void MultiPolynomial<D, C, E>::clear() {
    terms_.clear();
}

template <std::size_t D, typename C, typename E>
void MultiPolynomial<D, C, E>::resize(const std::size_t exp) {
    this->clear();

    // STOPPED WORK HERE



    for (exponent_type i = start; i <= exp; ++i) {
        terms_.emplace(i, static_cast<coefficient_type>(1));
    }
}

template <std::size_t D, typename C, typename E>
void MultiPolynomial<D, C, E>::insert(term_type&& term) {
    terms_.insert(std::forward<node_type>(nh));
    this->normalize();
}

template <std::size_t D, typename C, typename E>
void MultiPolynomial<D, C, E>::insert(std::initializer_list<term_type> terms) {
    terms_.insert(terms);
    this->normalize();
}

template <std::size_t D, typename C, typename E>
void MultiPolynomial<D, C, E>::insert(node_type&& nh) {
    terms_.insert(std::forward<node_type>(nh));
    this->normalize();
}

template <std::size_t D, typename C, typename E>
void MultiPolynomial<D, C, E>::erase(const exponent_type& exp) {
    terms_.erase(exp);
}

template <std::size_t D, typename C, typename E>
void MultiPolynomial<D, C, E>::differentiate() {
    constexpr exponent_type one = 1;
    map_type dterms;
    this->erase(static_cast<exponent_type>(0));
    for (const auto& [exp, coef]) {
        dterms.emplace(exp - one, coef * coefficient_type(exp));
    }
    std::swap(dterms, terms_);
}

template <std::size_t D, typename C, typename E>
void MultiPolynomial<D, C, E>::integrate() {
    constexpr exponent_type one = 1;
    map_type iterms;
    for (const auto& [exp, coef] : terms_) {
        iterms.emplace(exp + one, coef / static_cast<coefficient_type>(exp + one));
    }
}
// ====================================================
// Lookup
// ====================================================
template <std::size_t D, typename C, typename E>
typename MultiPolynomial<D, C, E>::size_type
MultiPolynomial<D, C, E>::size() const {
    return terms_.size();
}

template <std::size_t D, typename C, typename E>
typename MultiPolynomial<D, C, E>::size_type
MultiPolynomial<D, C, E>::degree() const {
    exponent_type answer(0);
    for (const auto& [exp, coef] : terms_) {
        answer = std::max(answer, exp);
    }
    return static_cast<size_type>(answer);
}

template <std::size_t D, typename C, typename E>
bool MultiPolynomial<D, C, E>::contains(const exponent_type& exp) const {
    return terms_.contains(exp);
}

template <std::size_t D, typename C, typename E>
typename MultiPolynomial<D, C, E>::coefficient_type
MultiPolynomial<D, C, E>::coefficient(const exponent_type& exp) const {
    auto term_iter = terms_.find(exp);
    if (terms_.cend() == term_iter) {
        return static_cast<coefficient_type>(0);
    }
    return term_iter->second;
}

template <std::size_t D, typename C, typename E>
typename MultiPolynomial<D, C, E>::coefficient_type
MultiPolynomial<D, C, E>::evaluate(const coefficient_type& value) const {
    using std::pow;
    coefficient_type answer(0);
    for (const auto& [exp, coef] : terms_) {
        answer += coef * pow(value, exp);
    }
    return answer;
}

// ====================================================
// [Private]
// ====================================================

// Remove all terms with zero coefficients
template <std::size_t D, typename C, typename E>
void MultiPolynomial<D, C, E>::normalize() {
    std::erase_if(terms_, [](const auto& item) {
        return (item.second == coefficient_type(0));
    });
}

}  // namespace tbox
