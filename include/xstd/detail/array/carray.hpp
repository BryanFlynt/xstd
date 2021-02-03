/**
 * \file       carray.hpp
 * \author     Bryan Flynt
 * \date       Feb 01, 2021
 * \copyright  Copyright (C) 2021 Bryan Flynt - All Rights Reserved
 */
#pragma once


#include <cstddef>

namespace xstd {

/// Constant array that models std::array
/**
 * A std::array like class with all member function 
 * as constexpr. The data is immutable once set.
 * 
 * Usage:
 * \code
 * template <std::size_t... Ints>
 * struct fixed_array {
 * 
 *     using cast_type = carray<std::size_t, sizeof...(Ints)>
 *     static constexpr cast_type array_ = cast_type({Ints...});
 * };
 * \endcode
 * 
 */
template <class T, std::size_t N>
class carray {
public:
    using size_type              = std::size_t;
    using value_type             = T;
    using pointer                = value_type*;
    using const_pointer          = const value_type*;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using difference_type        = std::ptrdiff_t;
    using iterator               = pointer;
    using const_iterator         = const_pointer;
    using reverse_iterator       = std::reverse_iterator<const_iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    constexpr const_reference operator[](std::size_t idx) const {
        return data_[idx];
    }

    constexpr const_iterator begin() const noexcept {
        return cbegin();
    }

    constexpr const_iterator end() const noexcept {
        return cend();
    }

    constexpr const_iterator cbegin() const noexcept {
        return data();
    }

    constexpr const_iterator cend() const noexcept {
        return data() + N;
    }

    constexpr reverse_iterator rbegin() const noexcept {
        return crbegin();
    }

    constexpr reverse_iterator rend() const noexcept {
        return crend();
    }

    constexpr const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    constexpr const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(begin());
    }

    constexpr const_pointer data() const noexcept {
        return data_;
    }

    constexpr const_reference front() const noexcept {
        return data_[0];
    }

    constexpr const_reference back() const noexcept {
        return data_[size() - 1];
    }

    constexpr bool empty() const noexcept {
        return size() == size_type(0);
    }

    constexpr size_type size() const noexcept {
        return N;
    }

    constexpr size_type max_size() const noexcept {
        return N;
    }

private:
    const T data_[N > 0 ? N : 1];
};


template <class T, std::size_t N>
inline bool operator==(const carray<T, N>& lhs, const carray<T, N>& rhs) {
    return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
}

template <class T, std::size_t N>
inline bool operator!=(const carray<T, N>& lhs, const carray<T, N>& rhs) {
    return !(lhs == rhs);
}

template <class T, std::size_t N>
inline bool operator<(const carray<T, N>& lhs, const carray<T, N>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end());
}

template <class T, std::size_t N>
inline bool operator<=(const carray<T, N>& lhs, const carray<T, N>& rhs) {
    return !(lhs > rhs);
}

template <class T, std::size_t N>
inline bool operator>(const carray<T, N>& lhs, const carray<T, N>& rhs) {
    return rhs < lhs;
}

template <class T, std::size_t N>
inline bool operator>=(const carray<T, N>& lhs, const carray<T, N>& rhs) {
    return !(lhs < rhs);
}

} /* namespace xstd */