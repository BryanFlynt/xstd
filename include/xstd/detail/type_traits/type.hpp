/*
 * type.hpp
 *
 *  Created on: Feb 23, 2020
 *      Author: bflynt
 */

#ifndef INCLUDE_XSTD_TYPE_TRAITS_TYPE_HPP_
#define INCLUDE_XSTD_TYPE_TRAITS_TYPE_HPP_


namespace xstd {

/** Simple type envelope.
 *
 * Useful to pass return types to template functions
 * without having to specify the <> before function
 * name.
 *
 * Usage:
 * \code
 *
 * template<typename ReturnType, typename IndexType>
 * ReturnType calc_new_index(xstd::type<ReturnType>, IndexType i) {
 *    return i + 1;
 * }
 *
 * \// Without xstd::type
 * int main() {
 *    auto new_val = calc_new_index<int,int>(3);
 *    return 0;
 * }
 *
 * \// With xstd::type
 * int main() {
 *    auto new_val = calc_new_index(xstd::type<int>(), 3);
 *    return 0;
 * }
 * \endcode
 */
template<typename T>
struct type {
};

}

#endif /* INCLUDE_XSTD_TYPE_TRAITS_TYPE_HPP_ */
