
#include "xstd/detail/vector/bounded_vector.hpp"

#include <iostream>
#include <typeinfo>
#include <vector>

#include "catch.hpp"

TEST_CASE("Static Vector", "[default]") {
    using namespace std;
    using namespace xstd;

    using value_type = double;
    constexpr std::size_t N = 4;

    SECTION("Empty [Constructor]") {
        vector<value_type> vec;
        bounded_vector<value_type, N> svec;
        REQUIRE(vec == svec);
    }

    SECTION("Size and Value [Constructor]") {
        vector<value_type> vec(4, 2.0);
        bounded_vector<value_type, N> svec(4, 2.0);
        REQUIRE(vec == svec);
    }

    SECTION("Iterator [Constructor]") {
        vector<value_type> vec(4, 2.0);
        bounded_vector<value_type, N> svec(vec.begin(), vec.end());
        REQUIRE(vec == svec);
    }

    SECTION("std::vector [Constructor]") {
        vector<value_type> vec(4, 2.0);
        bounded_vector<value_type, N> svec(vec);
        bounded_vector<value_type, N> xvec(svec);
        REQUIRE(vec == xvec);
    }

    SECTION("Move [5]") {
        vector<value_type> vec(4, 2.0);
        bounded_vector<value_type, N> svec(vec);
        bounded_vector<value_type, N> xvec(std::move(svec));
        REQUIRE(vec == xvec);
    }

    SECTION("initializer_list [Constructor]") {
        vector<value_type> vec({3, 4, 5, 6});
        bounded_vector<value_type, N> svec({3, 4, 5, 6});
        REQUIRE(vec == svec);
    }

    SECTION("Different sized bounded_vector [Constructor]") {
        vector<value_type> vec({3, 4, 5, 6});
        bounded_vector<value_type, N + 1> xvec({3, 4, 5, 6});
        bounded_vector<value_type, N> svec(xvec);
        REQUIRE(vec == svec);
    }

    SECTION("Copy [Assignment]") {
        vector<value_type> vec({3, 4, 5, 6});
        bounded_vector<value_type, N> xvec(vec);
        bounded_vector<value_type, N> svec = xvec;
        REQUIRE(vec == svec);
    }

    SECTION("Move [Assignment]") {
        vector<value_type> vec({3, 4, 5, 6});
        bounded_vector<value_type, N> svec = bounded_vector<value_type, N>(vec);
        REQUIRE(vec == svec);
    }

    SECTION("initializer_list [Assignment]") {
        vector<value_type> vec = {3, 4, 5, 6};
        bounded_vector<value_type, N> svec = {3, 4, 5, 6};
        REQUIRE(vec == svec);
    }

    SECTION("Copy std::vector [Assignment]") {
        vector<value_type> vec({3, 4, 5, 6});
        bounded_vector<value_type, N> svec = vec;
        REQUIRE(vec == svec);
    }

    SECTION("Different sized bounded_vector [Assignment]") {
        vector<value_type> vec({3, 4, 5, 6});
        bounded_vector<value_type, N + 1> xvec(vec);
        bounded_vector<value_type, N> svec = xvec;
        REQUIRE(vec == svec);
    }

    SECTION("At [Access]") {
        vector<value_type> vec({0, 1, 2, 3});
        bounded_vector<value_type, N> svec = vec;
        for (auto i = 0; i < svec.size(); ++i) {
            REQUIRE(vec.at(i) == svec.at(i));
        }
    }

    SECTION("Square Bracket [Access]") {
        vector<value_type> vec({0, 1, 2, 3});
        bounded_vector<value_type, N> svec = vec;
        for (auto i = 0; i < svec.size(); ++i) {
            REQUIRE(vec[i] == svec[i]);
        }
    }

    SECTION("Front [Access]") {
        vector<value_type> vec({3, 1, 2, 5});
        bounded_vector<value_type, N> svec = vec;
        REQUIRE(vec.front() == svec.front());
    }

    SECTION("Back [Access]") {
        vector<value_type> vec({3, 1, 2, 5});
        bounded_vector<value_type, N> svec = vec;
        REQUIRE(vec.back() == svec.back());
    }

    SECTION("empty() [Capacity]") {
        vector<value_type> vec;
        bounded_vector<value_type, N> svec;
        REQUIRE(vec.empty() == svec.empty());

        vec.push_back(1.0);
        svec.push_back(1.0);
        REQUIRE(vec.empty() == svec.empty());
    }

    SECTION("size() [Capacity]") {
        bounded_vector<value_type, N> svec;
        REQUIRE(svec.size() == 0);

        svec.push_back(1.0);
        REQUIRE(svec.size() == 1);
    }

    SECTION("max_size() [Capacity]") {
        bounded_vector<value_type, N> svec;
        REQUIRE(svec.max_size() == N);
    }

    SECTION("reserve() [Capacity]") {
        bounded_vector<value_type, N> svec;
        svec.reserve(N - 1);
        REQUIRE(true);
    }

    SECTION("capacity() [Capacity]") {
        bounded_vector<value_type, N> svec;
        REQUIRE(svec.capacity() == N);
    }

    SECTION("shrink_to_fit() [Capacity]") {
        bounded_vector<value_type, N> svec;
        svec.shrink_to_fit();
        REQUIRE(true);
    }

    SECTION("clear() [Modifiers]") {
        bounded_vector<value_type, N> svec = {2, 3, 4, 5};
        REQUIRE(not svec.empty());

        svec.clear();
        REQUIRE(svec.empty());
    }

    SECTION("Insert at front with value copy [Modifiers]") {
        vector<value_type> vec = {2, 3, 4};
        bounded_vector<value_type, N> svec(vec);

        value_type value = 7.0;
        auto vit = vec.insert(vec.begin(), value);
        auto sit = svec.insert(svec.begin(), value);
        REQUIRE(vec == svec);
        REQUIRE((vit - vec.begin()) == (sit - svec.begin()));
    }

    SECTION("Insert at front with &value inside [Modifiers]") {
        vector<value_type> vec = {2, 3, 4};
        bounded_vector<value_type, N> svec(vec);

        auto vit = vec.insert(vec.begin(), vec[1]);
        auto sit = svec.insert(svec.begin(), svec[1]);
        REQUIRE(vec == svec);
        REQUIRE((vit - vec.begin()) == (sit - svec.begin()));
    }

    SECTION("Insert at front with &value at pos [Modifiers]") {
        vector<value_type> vec = {2, 3, 4};
        bounded_vector<value_type, N> svec(vec);

        auto vit = vec.insert(vec.begin() + 1, vec[1]);
        auto sit = svec.insert(svec.begin() + 1, svec[1]);
        REQUIRE(vec == svec);
        REQUIRE((vit - vec.begin()) == (sit - svec.begin()));
    }

    SECTION("Insert at front with &value at end()-1 [Modifiers]") {
        vector<value_type> vec = {2, 3, 4};
        bounded_vector<value_type, N> svec(vec);

        auto vit = vec.insert(vec.begin(), vec[vec.size() - 1]);
        auto sit = svec.insert(svec.begin(), svec[svec.size() - 1]);
        REQUIRE(vec == svec);
        REQUIRE((vit - vec.begin()) == (sit - svec.begin()));
    }

    SECTION("Insert at front with value move [Modifiers]") {
        vector<value_type> vec = {2, 3, 4};
        bounded_vector<value_type, N> svec(vec);

        auto vit = vec.insert(vec.begin(), 7.0);
        auto sit = svec.insert(svec.begin(), 7.0);
        REQUIRE(vec == svec);
        REQUIRE((vit - vec.begin()) == (sit - svec.begin()));
    }

    SECTION("Insert at back with value copy [Modifiers]") {
        vector<value_type> vec = {2, 3, 4};
        bounded_vector<value_type, N> svec(vec);

        value_type value = 7.0;
        auto vit = vec.insert(vec.end(), value);
        auto sit = svec.insert(svec.end(), value);
        REQUIRE(vec == svec);
        REQUIRE((vit - vec.begin()) == (sit - svec.begin()));
    }

    SECTION("Insert at back with value move [Modifiers]") {
        vector<value_type> vec = {2, 3, 4};
        bounded_vector<value_type, N> svec(vec);

        auto vit = vec.insert(vec.end(), 7.0);
        auto sit = svec.insert(svec.end(), 7.0);
        REQUIRE(vec == svec);
        REQUIRE((vit - vec.begin()) == (sit - svec.begin()));
    }

    SECTION("insert(vec.begin(), 2, value) [Modifiers]") {
        vector<value_type> vec = {2, 3};
        bounded_vector<value_type, N> svec(vec);

        value_type value = 7.0;
        auto vit = vec.insert(vec.begin(), 2, value);
        auto sit = svec.insert(svec.begin(), 2, value);
        REQUIRE(vec == svec);
        REQUIRE((vit - vec.begin()) == (sit - svec.begin()));
    }

    SECTION("insert(vec.begin(), 2, vec[1]) [Modifiers]") {
        vector<value_type> vec = {2, 3};
        bounded_vector<value_type, N> svec(vec);

        auto vit = vec.insert(vec.begin(), 2, vec[1]);
        auto sit = svec.insert(svec.begin(), 2, svec[1]);
        REQUIRE(vec == svec);
        REQUIRE((vit - vec.begin()) == (sit - svec.begin()));
    }

    SECTION("insert(begin() + 1, 2, vec[vec.size() - 1]) [Modifiers]") {
        vector<value_type> vec = {2, 3};
        bounded_vector<value_type, N> svec(vec);

        auto vit = vec.insert(vec.begin() + 1, 2, vec[1]);
        auto sit = svec.insert(svec.begin() + 1, 2, svec[1]);
        REQUIRE(vec == svec);
        REQUIRE((vit - vec.begin()) == (sit - svec.begin()));
    }

    SECTION("insert(begin(), 2, vec[vec.size() - 1]) [Modifiers]") {
        vector<value_type> vec = {2, 3};
        bounded_vector<value_type, N> svec(vec);

        auto vit = vec.insert(vec.begin(), 2, vec[vec.size() - 1]);
        auto sit = svec.insert(svec.begin(), 2, svec[svec.size() - 1]);
        REQUIRE(vec == svec);
        REQUIRE((vit - vec.begin()) == (sit - svec.begin()));
    }

    SECTION("insert(begin(), 2, std::move(value)) [Modifiers]") {
        vector<value_type> vec = {2, 3};
        bounded_vector<value_type, N> svec(vec);

        auto vit = vec.insert(vec.begin(), 2, 7.0);
        auto sit = svec.insert(svec.begin(), 2, 7.0);
        REQUIRE(vec == svec);
        REQUIRE((vit - vec.begin()) == (sit - svec.begin()));
    }

    SECTION("insert(end(), 2, value) [Modifiers]") {
        vector<value_type> vec = {2, 3};
        bounded_vector<value_type, N> svec(vec);

        value_type value = 7.0;
        auto vit = vec.insert(vec.end(), 2, value);
        auto sit = svec.insert(svec.end(), 2, value);
        REQUIRE(vec == svec);
        REQUIRE((vit - vec.begin()) == (sit - svec.begin()));
    }

    SECTION("insert(end(), 2, std::move(value)) [Modifiers]") {
        vector<value_type> vec = {2, 3};
        bounded_vector<value_type, N> svec(vec);

        auto vit = vec.insert(vec.end(), 2, 7.0);
        auto sit = svec.insert(svec.end(), 2, 7.0);
        REQUIRE(vec == svec);
        REQUIRE((vit - vec.begin()) == (sit - svec.begin()));
    }

    SECTION("insert(begin(), ...) [Modifiers]") {
        vector<value_type> vec = {2, 3};
        bounded_vector<value_type, N> svec(vec);

        auto vit = vec.insert(vec.begin(), {1, 2});
        auto sit = svec.insert(svec.begin(), {1, 2});
        REQUIRE(vec == svec);
        REQUIRE((vit - vec.begin()) == (sit - svec.begin()));
    }

    SECTION("insert(end(), ...) [Modifiers]") {
        vector<value_type> vec = {2, 3};
        bounded_vector<value_type, N> svec(vec);

        auto vit = vec.insert(vec.end(), {1, 2});
        auto sit = svec.insert(svec.end(), {1, 2});
        REQUIRE(vec == svec);
        REQUIRE((vit - vec.begin()) == (sit - svec.begin()));
    }

    // SECTION("emplace(begin(), value) [Modifiers]") {
    //     vector<value_type> vec = {2, 3};
    //     bounded_vector<value_type, N> svec(vec);

    //     auto vit = vec.emplace(vec.begin(), 3.1415);
    //     auto sit = svec.emplace(svec.begin(), 3.1415);
    //     REQUIRE(vec == svec);
    //     REQUIRE((vit - vec.begin()) == (sit - svec.begin()));
    // }

    // SECTION("emplace(end(), value) [Modifiers]") {
    //     vector<value_type> vec = {2, 3};
    //     bounded_vector<value_type, N> svec(vec);

    //     auto vit = vec.emplace(vec.end(), 3.1415);
    //     auto sit = svec.emplace(svec.end(), 3.1415);
    //     REQUIRE(vec == svec);
    //     REQUIRE((vit - vec.begin()) == (sit - svec.begin()));
    // }

    // SECTION("erase(vec.begin()) [Modifiers]") {
    //     vector<value_type> vec = {2, 3, 4, 5};
    //     bounded_vector<value_type, N> svec(vec);

    //     auto vit = vec.erase(vec.begin());
    //     auto sit = svec.erase(svec.begin());
    //     REQUIRE(vec == svec);
    //     REQUIRE((vit - vec.begin()) == (sit - svec.begin()));
    // }

    // SECTION("erase(vec.end()-1) [Modifiers]") {
    //     vector<value_type> vec = {2, 3, 4, 5};
    //     bounded_vector<value_type, N> svec(vec);

    //     auto vit = vec.erase(vec.end() - 1);
    //     auto sit = svec.erase(svec.end() - 1);
    //     REQUIRE(vec == svec);
    //     REQUIRE((vit - vec.begin()) == (sit - svec.begin()));
    // }

    // SECTION("erase(vec.begin(),vec.begin()+2) [Modifiers]") {
    //     vector<value_type> vec = {2, 3, 4, 5};
    //     bounded_vector<value_type, N> svec(vec);

    //     auto vit = vec.erase(vec.begin(), vec.begin() + 2);
    //     auto sit = svec.erase(svec.begin(), svec.begin() + 2);
    //     REQUIRE(vec == svec);
    //     REQUIRE((vit - vec.begin()) == (sit - svec.begin()));
    // }

    // SECTION("erase(vec.end()-2,vec.end()-1) [Modifiers]") {
    //     vector<value_type> vec = {2, 3, 4, 5};
    //     bounded_vector<value_type, N> svec(vec);

    //     auto vit = vec.erase(vec.end() - 2, vec.end() - 1);
    //     auto sit = svec.erase(svec.end() - 2, svec.end() - 1);
    //     REQUIRE(vec == svec);
    //     REQUIRE((vit - vec.begin()) == (sit - svec.begin()));
    // }

    // SECTION("push_back(value) [Modifiers]") {
    //     vector<value_type> vec = {2, 3};
    //     bounded_vector<value_type, N> svec(vec);

    //     value_type value = 3.1415;
    //     vec.push_back(value);
    //     svec.push_back(value);
    //     REQUIRE(vec == svec);
    // }

    // SECTION("push_back(3.1415) [Modifiers]") {
    //     vector<value_type> vec = {2, 3};
    //     bounded_vector<value_type, N> svec(vec);

    //     vec.push_back(3.1415);
    //     svec.push_back(3.1415);
    //     REQUIRE(vec == svec);
    // }

    // SECTION("emplace_back(3.1415) [Modifiers]") {
    //     vector<value_type> vec = {2, 3};
    //     bounded_vector<value_type, N> svec(vec);

    //     vec.emplace_back(3.1415);
    //     svec.emplace_back(3.1415);
    //     REQUIRE(vec == svec);
    // }

    // SECTION("pop_back() [Modifiers]") {
    //     vector<value_type> vec = {2, 3, 4, 5};
    //     bounded_vector<value_type, N> svec(vec);

    //     vec.pop_back();
    //     svec.pop_back();
    //     REQUIRE(vec == svec);
    //     REQUIRE(vec.size() == svec.size());
    // }

    // SECTION("resize() [Modifiers]") {
    //     vector<value_type> vec = {2, 3, 4, 5};
    //     bounded_vector<value_type, N> svec(vec);

    //     vec.resize(2);
    //     svec.resize(2);
    //     REQUIRE(vec == svec);
    //     REQUIRE(vec.size() == svec.size());
    // }

    // SECTION("resize(4, value) [Modifiers]") {
    //     vector<value_type> vec = {2, 3};
    //     bounded_vector<value_type, N> svec(vec);

    //     vec.resize(4, 1.123);
    //     svec.resize(4, 1.123);
    //     REQUIRE(vec == svec);
    //     REQUIRE(vec.size() == svec.size());
    // }

    // SECTION("swap(other) same size [Modifiers]") {
    //     vector<value_type> va = {2, 3};
    //     vector<value_type> vb = {4, 5};

    //     bounded_vector<value_type, N> a(va);
    //     bounded_vector<value_type, N> b(vb);

    //     va.swap(vb);
    //     a.swap(b);

    //     REQUIRE(va == a);
    //     REQUIRE(vb == b);
    // }

    // SECTION("swap(other) different size [Modifiers]") {
    //     vector<value_type> va = {2, 3, 4};
    //     vector<value_type> vb = {4, 5};

    //     bounded_vector<value_type, N> a(va);
    //     bounded_vector<value_type, N> b(vb);

    //     va.swap(vb);
    //     a.swap(b);

    //     REQUIRE(va == a);
    //     REQUIRE(vb == b);
    // }

    // SECTION("swap(other<N+1>) same size [Modifiers]") {
    //     vector<value_type> va = {2, 3, 4, 0};
    //     vector<value_type> vb = {4, 5};

    //     bounded_vector<value_type, N> a(va);
    //     bounded_vector<value_type, N+1> b(vb);

    //     va.swap(vb);
    //     a.swap(b);

    //     REQUIRE(va == a);
    //     REQUIRE(vb == b);
    // }
}
