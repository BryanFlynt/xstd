/*
 * range.cpp
 *
 *  Created on: Dec 20, 2019
 *      Author: bflynt
 */

// Let Catch provide main():
#define CATCH_CONFIG_MAIN
#include "../../test/catch.hpp"


#include "xstd/detail/iterator/range.hpp"


#include <cstdint>


using IndexTypes = std::tuple<std::int8_t, std::int16_t, std::int32_t, std::int64_t>;


TEMPLATE_LIST_TEST_CASE("Range Iterators", "[default]", IndexTypes) {

	using Index = TestType;
	using Arith = double;

	const TestType first  =  3;
	const TestType last   = 10;
	const TestType pstep  =  2;
	const TestType mstep  = -2;


	SECTION("Zero Starting Index"){

		std::vector<TestType> a(last);
		std::copy( xstd::range_iterator<int>(),
				xstd::range_iterator<int>(last),
				a.begin());

		for(TestType i = 0; i < last; ++i){
			REQUIRE(i == a[i]);
		}

	}

	SECTION("Non-Zero Starting Index"){

		std::vector<TestType> a(last-first);
		std::copy( xstd::range_iterator<int>(first),
				xstd::range_iterator<int>(last),
				a.begin());

		for(TestType i = 0; i < a.size(); ++i){
			REQUIRE(i+first == a[i]);
		}

	}


	SECTION("Positive Stepping Index"){

		std::vector<TestType> a((last-first)/pstep);
		std::copy( xstd::range_iterator<int>(first,pstep),
				   xstd::range_iterator<int>(last,pstep),
				a.begin());

		for(TestType i = 0; i < a.size(); ++i){
			REQUIRE(first+(i*pstep) == a[i]);
		}
	}

	SECTION("Negative Stepping Index"){

		std::vector<TestType> a((last-first)/pstep);
		std::copy( xstd::range_iterator<int>(last,mstep),
				   xstd::range_iterator<int>(first,mstep),
				   a.begin());

		for(TestType i = 0; i < a.size(); ++i){
			REQUIRE(last+(i*mstep) == a[i]);
		}
	}

}


TEMPLATE_LIST_TEST_CASE("Range based for loop", "[default]", IndexTypes) {

    using Index = TestType;
    using Arith = double;

    const TestType first  =  3;
    const TestType last   = 10;
    const TestType pstep  =  2;
    const TestType mstep  = -2;

    SECTION("Zero Starting Index"){

        TestType truth = 0;
        for(TestType i = 0; i != last; ++i){
        	truth += i;
        }

    	TestType ans = 0;
    	for(auto i: xstd::range(last)){
    		ans += i;
    	}
    	REQUIRE(ans == truth);
    }

    SECTION("Non-Zero Starting Index"){

        TestType truth = 0;
        for(TestType i = first; i != last; ++i){
        	truth += i;
        }

    	TestType ans = 0;
    	for(auto i: xstd::range(first,last)){
    		ans += i;
    	}
    	REQUIRE(ans == truth);
    }

    SECTION("Positive Stepping Index"){

        TestType truth = 0;
        for(TestType i = first; i < last; i+=pstep){
        	truth += i;
        }

    	TestType ans = 0;
    	for(auto i: xstd::range(first,last,pstep)){
    		ans += i;
    	}
    	REQUIRE(ans == truth);
    }

    SECTION("Negative Stepping Index"){

        TestType truth = 0;
        for(TestType i = last; i > first; i+=mstep){
        	truth += i;
        }

    	TestType ans = 0;
    	for(auto i: xstd::range(last,first,mstep)){
    		ans += i;
    	}
    	REQUIRE(ans == truth);
    }

}



