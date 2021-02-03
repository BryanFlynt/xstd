/**
 * \file       bounded_vector.hpp
 * \author     Bryan Flynt
 * \date       Feb 01, 2021
 * \copyright  Copyright (C) 2021 Bryan Flynt - All Rights Reserved
 */
#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <memory>
#include <type_traits>
#include <vector>

namespace xstd {

namespace detail {
template <class It>
using require_input_iter = typename std::enable_if<std::is_convertible<typename std::iterator_traits<It>::iterator_category, std::input_iterator_tag>::value>::type;
}

/// Stack allocated vector that models std::vector
/**
 * A std::vector like class with all data on the stack.
 * The container cannot be resized larger than the initial
 * parameter passed at compile time.
 * 
 * \tparam T Type held within vector
 * \tparam N Maximum size at compile time
 * \tparam Init Flag to initialize data
 */
template <class T, std::size_t N, bool Init = true>
class bounded_vector {
    // ================================================================
    //                      Types & Constants
    // ================================================================
   public:
    using allocator_type = std::allocator<T>;
    using allocator_traits = std::allocator_traits<allocator_type>;
    using size_type = typename allocator_traits::size_type;
    using value_type = T;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using difference_type = typename allocator_traits::difference_type;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    constexpr static std::size_t alignment = alignof(T);

    // ================================================================
    //                        Member Functions
    // ================================================================
   public:
    bounded_vector() noexcept;

    constexpr bounded_vector(size_type count, const T& value);

    constexpr explicit bounded_vector(size_type count);

    template <class InputIt>
    constexpr bounded_vector(InputIt first, InputIt last);

    constexpr bounded_vector(const bounded_vector& other);

    constexpr bounded_vector(bounded_vector&& other) noexcept(std::is_nothrow_move_constructible<value_type>::value);

    constexpr bounded_vector(std::initializer_list<T> init);

    bounded_vector(const std::vector<T>& vec);

    ~bounded_vector() = default;

    template <std::size_t N2, bool I2, class = std::enable_if_t<N != N2, void>>
    constexpr bounded_vector(const bounded_vector<T, N2, I2>& other);

    constexpr bounded_vector& operator=(const bounded_vector& other);
    constexpr bounded_vector& operator=(bounded_vector&& other) noexcept(std::is_nothrow_move_assignable<value_type>::value);
    constexpr bounded_vector& operator=(std::initializer_list<T> ilist);
    bounded_vector& operator=(const std::vector<T>& other);

    template <std::size_t N2, bool I2, class = std::enable_if_t<N != N2, void>>
    bounded_vector& operator=(const bounded_vector<T, N2, I2>& other);

    constexpr void assign(size_type count, const T& value);

    template <class InputIt>
    constexpr void assign(InputIt first, InputIt last);

    constexpr void assign(std::initializer_list<T> ilist);

    constexpr allocator_type get_allocator() const noexcept;

    // ================================================================
    //                        Element Access
    // ================================================================

    constexpr reference at(size_type pos);

    constexpr const_reference at(size_type pos) const;

    constexpr reference operator[](size_type pos);

    constexpr const_reference operator[](size_type pos) const;

    constexpr reference front();

    constexpr const_reference front() const;

    constexpr reference back();

    constexpr const_reference back() const;

    constexpr T* data() noexcept;

    constexpr const T* data() const noexcept;

    // ================================================================
    //                           Iterators
    // ================================================================

    constexpr iterator begin() noexcept;

    constexpr const_iterator begin() const noexcept;

    constexpr const_iterator cbegin() const noexcept;

    constexpr iterator end() noexcept;

    constexpr const_iterator end() const noexcept;

    constexpr const_iterator cend() const noexcept;

    constexpr reverse_iterator rbegin() noexcept;

    constexpr const_reverse_iterator rbegin() const noexcept;

    constexpr const_reverse_iterator crbegin() const noexcept;

    constexpr reverse_iterator rend() noexcept;

    constexpr const_reverse_iterator rend() const noexcept;

    constexpr const_reverse_iterator crend() const noexcept;

    // ================================================================
    //                           Capacity
    // ================================================================

    [[nodiscard]] constexpr bool empty() const noexcept;

    constexpr size_type size() const noexcept;

    constexpr size_type max_size() const noexcept;

    constexpr void reserve(size_type new_cap);

    constexpr size_type capacity() const noexcept;

    constexpr void shrink_to_fit();

    // ================================================================
    //                           Modifiers
    // ================================================================

    constexpr void clear() noexcept;

    constexpr iterator insert(const_iterator pos, const T& value);

    constexpr iterator insert(const_iterator pos, T&& value);

    constexpr iterator insert(const_iterator pos, size_type count,
                              const T& value);

    template <class InputIt>
    constexpr iterator insert(const_iterator pos, InputIt first, InputIt last);

    constexpr iterator insert(const_iterator pos, std::initializer_list<T> ilist);

    template <class... Args>
    constexpr iterator emplace(const_iterator pos, Args&&... args);

    constexpr iterator erase(const_iterator pos);

    constexpr iterator erase(const_iterator first, const_iterator last);

    constexpr void push_back(const T& value);

    constexpr void push_back(T&& value);

    template <class... Args>
    constexpr reference emplace_back(Args&&... args);

    constexpr void pop_back();

    constexpr void resize(size_type count);

    constexpr void resize(size_type count, const value_type& value);

    constexpr void swap(bounded_vector& other) noexcept;

    template <std::size_t N2, bool I2, class = std::enable_if_t<N != N2, void>>
    void swap(bounded_vector<T, N2, I2>& other);

   protected:
    alignas(alignment) T data_[N > 0 ? N : 1];
    size_type size_;
};

// ****************************************************************
//                        Implementation
// ****************************************************************

// ================================================================
//                        Member Functions
// ================================================================

template <class T, std::size_t N, bool I>
bounded_vector<T, N, I>::bounded_vector() noexcept : size_(0) {
}

template <class T, std::size_t N, bool I>
constexpr bounded_vector<T, N, I>::bounded_vector(size_type count, const T& value) {
    assign(count, value);
}

template <class T, std::size_t N, bool I>
constexpr bounded_vector<T, N, I>::bounded_vector(size_type count) {
    assert(count <= N);
    if constexpr (I) {
        assign(count, T(0));
    } else {
        resize(count);
    }
}

template <class T, std::size_t N, bool I>
template <class InputIt>
constexpr bounded_vector<T, N, I>::bounded_vector(InputIt first, InputIt last) {
    assign(first, last);
}

template <class T, std::size_t N, bool I>
constexpr bounded_vector<T, N, I>::bounded_vector(const bounded_vector& other) {
    assign(other.begin(), other.end());
}

template <class T, std::size_t N, bool I>
constexpr bounded_vector<T, N, I>::bounded_vector(bounded_vector&& other) noexcept(std::is_nothrow_move_constructible<value_type>::value) {
    *this = std::move(other); // Forward to move assignment
}

template <class T, std::size_t N, bool I>
constexpr bounded_vector<T, N, I>::bounded_vector(std::initializer_list<T> init) {
    assign(init.begin(), init.end());
}

template <class T, std::size_t N, bool I>
bounded_vector<T, N, I>::bounded_vector(const std::vector<T>& vec) {
    assign(vec.begin(), vec.end());
}

template <class T, std::size_t N, bool I>
template <std::size_t N2, bool I2, class Ans>
constexpr bounded_vector<T, N, I>::bounded_vector(const bounded_vector<T, N2, I2>& other) {
    assign(other.begin(), other.end());
}

template <class T, std::size_t N, bool I>
constexpr bounded_vector<T, N, I>& bounded_vector<T, N, I>::operator=(const bounded_vector& other) {
    assign(other.begin(), other.end());
    return *this;
}

template <class T, std::size_t N, bool I>
constexpr bounded_vector<T, N, I>& bounded_vector<T, N, I>::operator=(bounded_vector&& other) noexcept(std::is_nothrow_move_assignable<value_type>::value) {
    this->swap(other);
    return *this;
}

template <class T, std::size_t N, bool I>
constexpr bounded_vector<T, N, I>& bounded_vector<T, N, I>::operator=(std::initializer_list<T> ilist) {
    assign(ilist.begin(), ilist.end());
    return *this;
}

template <class T, std::size_t N, bool I>
bounded_vector<T, N, I>& bounded_vector<T, N, I>::operator=(const std::vector<T>& other) {
    assign(other.begin(), other.end());
    return *this;
}

template <class T, std::size_t N, bool I>
template <std::size_t N2, bool I2, class>
bounded_vector<T, N, I>& bounded_vector<T, N, I>::operator=(const bounded_vector<T, N2, I2>& other) {
    assign(other.begin(), other.end());
    return *this;
}

template <class T, std::size_t N, bool I>
constexpr void bounded_vector<T, N, I>::assign(size_type count, const T& value) {
    assert(count <= max_size());
    std::fill_n(begin(), count, value);
    size_ = count;
}

template <class T, std::size_t N, bool I>
template <class InputIt>
constexpr void bounded_vector<T, N, I>::assign(InputIt first, InputIt last) {
    assert(std::distance(first, last) <= N);
    std::copy(first, last, begin());
    size_ = std::distance(first, last);
}

template <class T, std::size_t N, bool I>
constexpr void bounded_vector<T, N, I>::assign(std::initializer_list<T> ilist) {
    assert(std::distance(ilist.begin(), ilist.end()) <= N);
    std::copy(ilist.begin(), ilist.end(), begin());
    size_ = ilist.size();
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::allocator_type bounded_vector<T, N, I>::get_allocator() const noexcept {
    return allocator_type();
}

// ================================================================
//                        Element Access
// ================================================================

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::reference bounded_vector<T, N, I>::at(size_type pos) {
    assert(pos < this->size());
    return data_[pos];
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::const_reference bounded_vector<T, N, I>::at(size_type pos) const {
    assert(pos < this->size());
    return data_[pos];
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::reference bounded_vector<T, N, I>::operator[](size_type pos) {
    assert(pos < this->size());
    return data_[pos];
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::const_reference bounded_vector<T, N, I>::operator[](size_type pos) const {
    assert(pos < this->size());
    return data_[pos];
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::reference bounded_vector<T, N, I>::front() {
    assert(0 < this->size());
    return data_[0];
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::const_reference bounded_vector<T, N, I>::front() const {
    assert(0 < this->size());
    return data_[0];
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::reference bounded_vector<T, N, I>::back() {
    assert(0 < this->size());
    return data_[size_ - 1];
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::const_reference bounded_vector<T, N, I>::back() const {
    assert(0 < this->size());
    return data_[size_ - 1];
}

template <class T, std::size_t N, bool I>
constexpr T* bounded_vector<T, N, I>::data() noexcept {
    return data_;
}

template <class T, std::size_t N, bool I>
constexpr const T* bounded_vector<T, N, I>::data() const noexcept {
    return data_;
}

// ================================================================
//                           Iterators
// ================================================================

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::iterator bounded_vector<T, N, I>::begin() noexcept {
    return data_;
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::const_iterator bounded_vector<T, N, I>::begin() const noexcept {
    return data_;
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::const_iterator bounded_vector<T, N, I>::cbegin() const noexcept {
    return begin();
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::iterator bounded_vector<T, N, I>::end() noexcept {
    return begin() + size_;
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::const_iterator bounded_vector<T, N, I>::end() const noexcept {
    return begin() + size_;
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::const_iterator bounded_vector<T, N, I>::cend() const noexcept {
    return end();
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::reverse_iterator bounded_vector<T, N, I>::rbegin() noexcept {
    return reverse_iterator(end());
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::const_reverse_iterator bounded_vector<T, N, I>::rbegin() const noexcept {
    return const_reverse_iterator(end());
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::const_reverse_iterator bounded_vector<T, N, I>::crbegin() const noexcept {
    return const_reverse_iterator(rbegin());
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::reverse_iterator bounded_vector<T, N, I>::rend() noexcept {
    return reverse_iterator(begin());
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::const_reverse_iterator bounded_vector<T, N, I>::rend() const noexcept {
    return const_reverse_iterator(begin());
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::const_reverse_iterator bounded_vector<T, N, I>::crend() const noexcept {
    return const_reverse_iterator(rend());
}

// ================================================================
//                           Capacity
// ================================================================

template <class T, std::size_t N, bool I>
[[nodiscard]] constexpr bool bounded_vector<T, N, I>::empty() const noexcept {
    return (0 == size_);
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::size_type bounded_vector<T, N, I>::size() const noexcept {
    return size_;
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::size_type bounded_vector<T, N, I>::max_size() const noexcept {
    return N;
}

template <class T, std::size_t N, bool I>
constexpr void bounded_vector<T, N, I>::reserve(size_type new_cap) {
    assert(new_cap < N);
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::size_type bounded_vector<T, N, I>::capacity() const noexcept {
    return N;
}

template <class T, std::size_t N, bool I>
constexpr void bounded_vector<T, N, I>::shrink_to_fit() {
    // Nothing to do
}

// ================================================================
//                           Modifiers
// ================================================================

template <class T, std::size_t N, bool I>
constexpr void bounded_vector<T, N, I>::clear() noexcept {
    resize(size_type(0));
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::iterator bounded_vector<T, N, I>::insert(const_iterator pos, const T& value) {
    assert(size() < max_size());
    assert((pos - cbegin()) >= 0);
    assert((pos - cbegin()) <= size());

    if (pos == cend()) {
        push_back(value);
        return (end() - 1);
    }
    return this->insert(pos, size_type(1), value);
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::iterator bounded_vector<T, N, I>::insert(const_iterator pos, T&& value) {
    assert(size() < max_size());
    assert((pos - cbegin()) >= 0);
    assert((pos - cbegin()) <= size());

    if (pos == cend()) {
        emplace_back(std::forward<T>(value));
        return (end() - 1);
    }

    auto it = const_cast<pointer>(pos);
    std::move_backward(it, end(), end() + 1);
    *it = std::forward<T>(value);
    ++size_;
    return it;
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::iterator bounded_vector<T, N, I>::insert(const_iterator pos, size_type count, const T& value) {
    assert((pos - cbegin()) >= 0);
    assert((pos - cbegin()) <= size());
    assert((size() + count) <= max_size());

    // Shift tail end "count" positions to right
    auto it = const_cast<pointer>(pos);
    std::move_backward(it, end(), end() + count);

    // Handle case if &value is/was within this array but after pos
    auto value_iter = static_cast<const_iterator>(&value);
    const bool value_after_pos = std::distance(pos, value_iter) >= 0;
    const bool value_before_end = std::distance(value_iter, cend()) > 0;
    if (value_after_pos && value_before_end) {
        value_iter += count;
    }
    std::fill(it, it + count, *value_iter);
    size_ += count;
    return it;
}

template <class T, std::size_t N, bool I>
template <class InputIt>
constexpr typename bounded_vector<T, N, I>::iterator bounded_vector<T, N, I>::insert(const_iterator pos, InputIt first, InputIt last) {
    assert((pos - cbegin()) >= 0);
    assert((pos - cbegin()) <= size());
    assert(size() + std::distance(first, last) <= max_size());

    auto it = const_cast<pointer>(pos);
    const auto num_added = std::distance(first, last);
    std::move_backward(it, end(), end() + num_added);
    std::copy(first, last, it);
    size_ += num_added;
    return it;
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::iterator bounded_vector<T, N, I>::insert(const_iterator pos, std::initializer_list<T> ilist) {
    return insert(pos, ilist.begin(), ilist.end());
}

template <class T, std::size_t N, bool I>
template <class... Args>
constexpr typename bounded_vector<T, N, I>::iterator bounded_vector<T, N, I>::emplace(const_iterator pos, Args&&... args) {
    assert((pos - cbegin()) >= 0);
    assert((pos - cbegin()) <= size());
    return insert(pos, T(std::forward<Args>(args)...));
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::iterator bounded_vector<T, N, I>::erase(const_iterator pos) {
    assert((pos - cbegin()) >= 0);
    assert((pos - cbegin()) < size());
    auto it = const_cast<iterator>(pos);
    std::move(it + 1, end(), it);
    --size_;
    return it;
}

template <class T, std::size_t N, bool I>
constexpr typename bounded_vector<T, N, I>::iterator bounded_vector<T, N, I>::erase(const_iterator cfirst, const_iterator clast) {
    assert((clast - cfirst) >= 0);
    assert((cfirst - cbegin()) >= 0);
    assert((clast - cbegin()) < size());

    auto first = const_cast<iterator>(cfirst);
    if (clast == cend()) {
        size_ = cfirst - cbegin();
        return first;
    }

    auto last = const_cast<iterator>(clast);
    size_ = std::move(last, end(), first) - cbegin();
    return first;
}

template <class T, std::size_t N, bool I>
constexpr void bounded_vector<T, N, I>::push_back(const T& value) {
    assert(size() < max_size());
    data_[size_] = value;
    ++size_;
}

template <class T, std::size_t N, bool I>
constexpr void bounded_vector<T, N, I>::push_back(T&& value) {
    assert(size() < max_size());
    data_[size_] = std::forward<T>(value);
    ++size_;
}

template <class T, std::size_t N, bool I>
template <class... Args>
constexpr typename bounded_vector<T, N, I>::reference bounded_vector<T, N, I>::emplace_back(Args&&... args) {
    assert(size() < max_size());
    data_[size_] = T(std::forward<Args>(args)...);
    ++size_;
    return this->back();
}

template <class T, std::size_t N, bool I>
constexpr void bounded_vector<T, N, I>::pop_back() {
    if (not empty()) {
        --size_;
    }
}

template <class T, std::size_t N, bool I>
constexpr void bounded_vector<T, N, I>::resize(size_type count) {
    assert(count <= max_size());
    if (I && (count > this->size())) {
        std::fill(data_ + size_, data_ + count, T());
    }
    size_ = count;
}

template <class T, std::size_t N, bool I>
constexpr void bounded_vector<T, N, I>::resize(size_type count, const value_type& value) {
    assert(count <= max_size());
    if (count > this->size()) {
        std::fill(data_ + size_, data_ + count, value);
    }
    size_ = count;
}

template <class T, std::size_t N, bool I>
constexpr void bounded_vector<T, N, I>::swap(bounded_vector& other) noexcept {
    if (this != &other) {
        std::swap(size_, other.size_);
        std::swap_ranges(data_, data_ + std::max(size_, other.size_), other.data_);
    }
}

template <class T, std::size_t N, bool I>
template <std::size_t N2, bool I2, class>
void bounded_vector<T, N, I>::swap(bounded_vector<T, N2, I2>& other) {
    assert(this->size() < other.max_size());
    assert(other.size() < this->max_size());
    using std::swap;

    const auto old_other_size = other.size();
    const auto old_this_size = this->size();

    if (old_other_size > old_this_size) {
        this->resize(old_other_size);
    } else if (old_this_size > old_other_size) {
        other.resize(old_this_size);
    }

    // Swap the shared elements.
    const auto min_size = std::min(old_this_size, old_other_size);
    for (size_type i = 0; i < min_size; ++i) {
        swap((*this)[i], other[i]);
    }

    // Copy over the extra values.
    if (old_this_size > old_other_size) {
        std::copy(this->begin() + min_size, this->end(), other.begin() + min_size);
        this->resize(old_other_size);
    } else if (old_other_size > old_this_size) {
        std::copy(other.begin() + min_size, other.end(), this->begin() + min_size);
        other.resize(old_this_size);
    }
}

// ================================================================
//                        Free Functions
// ================================================================

template <typename T, std::size_t N, bool Init>
inline bool operator==(const std::vector<T>& lhs, const bounded_vector<T, N, Init>& rhs) {
    return (lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T, std::size_t N, bool Init>
inline bool operator==(const bounded_vector<T, N, Init>& lhs, const std::vector<T>& rhs) {
    return (lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T, std::size_t N, bool Init>
inline bool operator==(const bounded_vector<T, N, Init>& lhs, const bounded_vector<T, N, Init>& rhs) {
    return (lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T, std::size_t N, bool Init>
inline bool operator!=(const bounded_vector<T, N, Init>& lhs, const bounded_vector<T, N, Init>& rhs) {
    return !(lhs == rhs);
}

template <typename T, std::size_t N, bool Init>
inline bool operator<(const bounded_vector<T, N, Init>& lhs, const bounded_vector<T, N, Init>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end());
}

template <typename T, std::size_t N, bool Init>
inline bool operator<=(const bounded_vector<T, N, Init>& lhs, const bounded_vector<T, N, Init>& rhs) {
    return !(lhs > rhs);
}

template <typename T, std::size_t N, bool Init>
inline bool operator>(const bounded_vector<T, N, Init>& lhs, const bounded_vector<T, N, Init>& rhs) {
    return rhs < lhs;
}

template <typename T, std::size_t N, bool Init>
inline bool operator>=(const bounded_vector<T, N, Init>& lhs, const bounded_vector<T, N, Init>& rhs) {
    return !(lhs < rhs);
}

template <typename T, std::size_t N, bool Init>
inline void swap(bounded_vector<T, N, Init>& lhs, bounded_vector<T, N, Init>& rhs) noexcept {
    lhs.swap(rhs);
}

} /* namespace xstd */