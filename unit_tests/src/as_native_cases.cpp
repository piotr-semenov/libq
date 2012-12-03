#define BOOST_TEST_STATIC_LINK

#include <boost/test/unit_test.hpp>
#include <boost/integer.hpp>

#include <limits>
#include <string>

#include "./../../fixed_point_lib/src/number.hpp"
#include "./../../fixed_point_lib/src/as_native_proxy.hpp"

namespace core { namespace unit_tests {
    namespace c = core;

    BOOST_AUTO_TEST_SUITE(as_native)

    /// idea of test:
    /// 1. we choose two fixed-number formats [t1, f1] and [t2, f2] those satisfy:
    ///    t1 = t2 and f1 >> f2
    /// 2. we assign first format to variable A and second one to variable B
    /// 3. B has such value that (B << (f1 - f2)) is out of t1 dynamic interval bounds
    /// so, fixed-point alignment has to be failed during fixed-point numbers division
    BOOST_AUTO_TEST_CASE(bitCheck)
    {
        c::S_fixed_point<28, 20>::type a(-2.302);
        c::S_fixed_point<28, 2>::type b(1000123);
        c::S_fixed_point<38, 4>::type c(10123);

        c::as_native(a) += (1u << 20);
        double const value = static_cast<double>(a);
        as_native(b) /= 23;
    }

    BOOST_AUTO_TEST_SUITE_END()
}}
