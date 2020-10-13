/*
 * string_to.hpp
 *
 *  Created on: Oct 12, 2020
 *      Author: bflynt
 */

#ifndef INCLUDE_XSTD_DETAIL_STRING_STRING_TO_HPP_
#define INCLUDE_XSTD_DETAIL_STRING_STRING_TO_HPP_

#include "xstd/assert.hpp"
#include "xstd/detail/type_traits/type.hpp"


#include <cstdint>
#include <sstream>
#include <string>
#include <limits>

/**
 * \file
 * string_to.hpp
 *
 * \brief
 * Convert std::string to number
 *
 * \details
 * Converts a std::string to the requested number type.
 * Kind of a an odd function signature but this eliminates the reason
 * the standards committee didn't want to implement the feature in C++.
 * Reportedly they didn't want to have the return type dependent on a
 * template argument and wanted instead the function selection to be
 * determined by an overload. We accomplish that by passing a xstd::type<>.
 */

namespace xstd {

inline
std::int64_t string_to(type<std::int64_t>, const std::string& str, std::size_t* pos = nullptr, int base = 10) {
	return static_cast<std::int64_t>(std::stoll(str,pos,base));
}

inline
std::int32_t string_to(type<std::int32_t>, const std::string& str, std::size_t* pos = nullptr, int base = 10) {
	using return_type = std::int32_t; // Return Type
	using conver_type = std::int64_t; // Conversion Type (larger than return_type)
	constexpr auto max = static_cast<conver_type>(std::numeric_limits<return_type>::max());
	constexpr auto min = static_cast<conver_type>(std::numeric_limits<return_type>::lowest());
	const auto value = string_to(type<conver_type>(), str, pos, base);
	ASSERT( value <= max );
	ASSERT( value >= min );
	return static_cast<return_type>(value);
}

inline
std::int16_t string_to(type<std::int16_t>, const std::string& str, std::size_t* pos = nullptr, int base = 10) {
	using return_type = std::int16_t; // Return Type
	using conver_type = std::int64_t; // Conversion Type (larger than return_type)
	constexpr auto max = static_cast<conver_type>(std::numeric_limits<return_type>::max());
	constexpr auto min = static_cast<conver_type>(std::numeric_limits<return_type>::lowest());
	const auto value = string_to(type<conver_type>(), str, pos, base);
	ASSERT( value <= max );
	ASSERT( value >= min );
	return static_cast<return_type>(value);
}

inline
std::int8_t string_to(type<std::int8_t>, const std::string& str, std::size_t* pos = nullptr, int base = 10) {
	using return_type = std::int8_t;  // Return Type
	using conver_type = std::int64_t; // Conversion Type (larger than return_type)
	constexpr auto max = static_cast<conver_type>(std::numeric_limits<return_type>::max());
	constexpr auto min = static_cast<conver_type>(std::numeric_limits<return_type>::lowest());
	const auto value = string_to(type<conver_type>(), str, pos, base);
	ASSERT( value <= max );
	ASSERT( value >= min );
	return static_cast<return_type>(value);
}


inline
std::uint64_t string_to(type<std::uint64_t>, const std::string& str, std::size_t* pos = nullptr, int base = 10) {
	return static_cast<std::uint64_t>(std::stoull(str,pos,base));
}

inline
std::uint32_t string_to(type<std::uint32_t>, const std::string& str, std::size_t* pos = nullptr, int base = 10) {
	using return_type = std::uint32_t; // Return Type
	using conver_type = std::uint64_t; // Conversion Type (larger than return_type)
	constexpr auto max = static_cast<conver_type>(std::numeric_limits<return_type>::max());
	constexpr auto min = static_cast<conver_type>(std::numeric_limits<return_type>::lowest());
	const auto value = string_to(type<conver_type>(), str, pos, base);
	ASSERT( value <= max );
	ASSERT( value >= min );
	return static_cast<return_type>(value);
}

inline
std::uint16_t string_to(type<std::uint16_t>, const std::string& str, std::size_t* pos = nullptr, int base = 10) {
	using return_type = std::uint16_t; // Return Type
	using conver_type = std::uint64_t; // Conversion Type (larger than return_type)
	constexpr auto max = static_cast<conver_type>(std::numeric_limits<return_type>::max());
	constexpr auto min = static_cast<conver_type>(std::numeric_limits<return_type>::lowest());
	const auto value = string_to(type<conver_type>(), str, pos, base);
	ASSERT( value <= max );
	ASSERT( value >= min );
	return static_cast<return_type>(value);
}

inline
std::uint8_t string_to(type<std::uint8_t>, const std::string& str, std::size_t* pos = nullptr, int base = 10) {
	using return_type = std::uint8_t; // Return Type
	using conver_type = std::uint64_t; // Conversion Type (larger than return_type)
	constexpr auto max = static_cast<conver_type>(std::numeric_limits<return_type>::max());
	constexpr auto min = static_cast<conver_type>(std::numeric_limits<return_type>::lowest());
	const auto value = string_to(type<conver_type>(), str, pos, base);
	ASSERT( value <= max );
	ASSERT( value >= min );
	return static_cast<return_type>(value);
}


inline
long double string_to(type<long double>, const std::string& str, std::size_t* pos = nullptr ) {
	return std::stold(str,pos);
}

inline
double string_to(type<double>, const std::string& str, std::size_t* pos = nullptr ) {
	return std::stod(str,pos);
}

inline
float string_to(type<float>, const std::string& str, std::size_t* pos = nullptr ) {
	return std::stof(str,pos);
}


/// Boolean conversion
/**
 * False = String starts with 'F' or 'f' or '0'
 * True  = Everything else
 */
inline
bool string_to(type<bool>, const std::string& str) {
	ASSERT(str.size() > 0);
	return (str[0] == 'F' || str[0] == 'f' || str[0] == '0') ? false : true;
}


} /* namespace xstd */

#endif /* INCLUDE_XSTD_DETAIL_STRING_STRING_TO_HPP_ */
