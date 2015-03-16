#define BOOST_TEST_STATIC_LINK
#define BOOST_TEST_MODULE FIXED_POINT_LIB_UNIT_TESTS

#include <boost/test/unit_test.hpp>
#include <boost/integer.hpp>

#include <limits>
#include <string>

#include "./fixed_point.hpp"
#include "./as_native_proxy.hpp"

namespace libq { namespace unit_tests {
    BOOST_AUTO_TEST_SUITE(as_native)

    BOOST_AUTO_TEST_CASE(as_native)
    { 
        libq::S_fixed_point<28, 20>::type a(-2.302);
        libq::S_fixed_point<28, 2>::type b(1000123);
        libq::S_fixed_point<38, 4>::type c(10123);

        libq::as_native(a) += (1u << 20);
        libq::as_native(b) /= 29;
        BOOST_CHECK_MESSAGE(std::fabs(double(a) + 1.302) < 1E-4, "as_native operator += has a bug");
        BOOST_CHECK_MESSAGE(size_t(~libq::as_native(b)) == 268297507, "as_native operator ~ has a bug");
        BOOST_CHECK_MESSAGE(size_t(double(b)) == 34487, "as_native operator /= has a bug");

        long const x = libq::as_native(a) + 23u;
        long const y = 23u + libq::as_native(a); 
        BOOST_CHECK_MESSAGE(x == -1365223 && y == -1365223, "as_native operator + has a bug");
    }

    BOOST_AUTO_TEST_SUITE_END()
}}
