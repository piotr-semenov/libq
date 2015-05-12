#define BOOST_TEST_STATIC_LINK
#define BOOST_TEST_MODULE FIXED_POINT_LIB_UNIT_TESTS

#include "boost/test/unit_test.hpp"
#include "boost/integer.hpp"

#include <limits>
#include <string>

#include "libq/fixed_point.hpp"

namespace libq {
namespace unit_tests {
BOOST_AUTO_TEST_SUITE(lift)

BOOST_AUTO_TEST_CASE(as_native)
{ 
    libq::Q<28, 20> a(-2.302);
    libq::Q<28, 2> b(1000123);
    libq::Q<38, 4> c(10123);

    libq::lift(a) += (1u << 20);
    libq::lift(b) /= 29;

    BOOST_CHECK_MESSAGE(std::fabs(double(a) + 1.302) < 1E-4, "[lift] operator += has a bug");
    BOOST_CHECK_MESSAGE(~std::size_t(libq::lift(b)) == 4294829347, "[lift] operator ~ has a bug");
    BOOST_CHECK_MESSAGE(std::size_t(double(b)) == 34487, "[lift] operator /= has a bug");

    long const x = libq::lift(a) + 23u;
    long const y = 23u + libq::lift(a); 
    BOOST_CHECK_MESSAGE(x == -1365223 && y == -1365223, "[lift] operator + has a bug");
}

BOOST_AUTO_TEST_SUITE_END()
} // unit_cases
} // libq
