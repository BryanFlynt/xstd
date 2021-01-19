/*
 * intersection.hpp
 *
 *  Created on: Jan 5, 2020
 *      Author: bflynt
 */

#ifndef INCLUDE_XSTD_SET_INTERSECTION_HPP_
#define INCLUDE_XSTD_SET_INTERSECTION_HPP_

#include "xstd/tuple.hpp"

namespace xstd {

/// Set Intersection taking multiple sorted containers
/**
 * The Intersection function takes multiple sorted containers
 * and performs an intersection operation on them all at the
 * same time. This is much faster than performing a chain of
 * std::set_intersection calls on the same containers.
 *
 * \param out[inout] Output container to build intersected set within
 * \param args[in] Variable argument array of sorted containers
 *
 * \returns The new output container with intersection
 */
template<typename Output, typename... SortedType>
Output Intersection(Output& out, const SortedType& ...args){
    using std::max;

	auto its = std::make_tuple( std::cbegin(args)... ); // tuple of begin() iterators
	const auto ends  = std::make_tuple( std::cend(args)... );   // tuple of end() iterators

	auto not_equal = [](const auto& a, const auto& b){return *a != *b;};
	while( xstd::all_of(its, ends, not_equal) ) {

	    // Get tuple of dereferenced values
	    const auto value_tuple = xstd::transform(its, [](const auto& it){return *it;});

	    // Get max value within current tuple
	    const auto max_value = xstd::max(value_tuple);

	    // Count how many values match the current max_value
        const auto count = xstd::count_if(value_tuple, [max_value](const auto& val){return (val == max_value);});

        // If all dereferenced iterators equal the same then we have an intersection
        if( count == std::tuple_size_v<decltype(its)> ){
            out.insert(std::cend(out), max_value);
            xstd::for_each(its, [](auto& it){++it;});
        }
        else { // No intersection so increment all iterators less than max
            xstd::for_each(its, [max_value](auto& it){
                if( *it < max_value ){
                    ++it;
                }
            });
        }
	}
	return out;
}


} /* namespace xstd */


#endif /* INCLUDE_XSTD_SET_INTERSECTION_HPP_ */
