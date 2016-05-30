#define BOOST_TEST_STATIC_LINK

#include <string>
#include <limits>
#include <stdexcept>

#include "boost/test/unit_test.hpp"

#include "libq/fixed_point.hpp"

namespace libq {
namespace unit_tests {

BOOST_AUTO_TEST_SUITE(Range)

/// test 'division_checkRange':
///     check if the largest and the smallest rationals are enabled with
///     fixed-point promotion format derived after division operation
BOOST_AUTO_TEST_CASE(check_the_division_result_range)
{
#define UPVALUE(n, f) std::numeric_limits<libq::Q<n, f> >::max()
#define DOWNVALUE(n, f) libq::UQ<n, f>::wrap(1)
#define QMAX(n1,f1, n2,f2) UPVALUE(n1, f1)/DOWNVALUE(n2, f2)
#define QMIN(n1,f1, n2,f2) DOWNVALUE(n1,f1)/UPVALUE(n2,f2)
#define TEST(q, val, precision, msg) \
    BOOST_CHECK_MESSAGE(std::fabs(static_cast<double>(q) - val) < precision, msg);

    std::string const err0("range upper limit can not be approached");
    std::string const err1("range lower limit can not been approached");

    TEST(QMAX(7,6, 17,12), 8128, 1E-9, err0);
    TEST(QMAX(23,12, 6,4), 32767.99609375, 1E-9, err0);
    TEST(QMAX(34,23, 26,23), 17179869183.0, 1E-9, err0);
    TEST(QMAX(19,2, 43,26), 8796076244992.0, 1E-9, err0);
    TEST(QMAX(35,34, 23,22), 8388607.99975, 1E-5, err0);

    TEST(QMIN(7,6, 17,12), 0.000488285, 1E-8, err1);
    TEST(QMIN(23,12, 6,4), 0.000062003968, 1E-5, err1);
    TEST(QMIN(34,23, 26,23), 1.490116E-8, 1E-9, err1);
    TEST(QMIN(19,2, 43,26), 1.907348E-6, 1E-9, err1);
    TEST(QMIN(35,34, 23,22), 2.91038E-11, 1E-16, err1);

#undef TEST
#undef QMIN
#undef QMAX
#undef DOWNVALUE
#undef UPVALUE
}

/// test 'out_of_range_policies':
///     common checks if integer value (interpreted as fixed-point) is out
///     of range
BOOST_AUTO_TEST_CASE(out_of_range_policies)
{
    using Q = libq::UQ<8, 5>;

    std::string const msg("positive overflow was detected");
    try {
        Q const x(std::numeric_limits<Q>::max() + 1);

        BOOST_FAIL(msg);
    }
    catch (std::overflow_error e) {}

    try {
        Q const x(std::numeric_limits<Q>::min() - 1);
 
        BOOST_FAIL(msg);
    }
    catch (std::overflow_error e) {}
}
BOOST_AUTO_TEST_SUITE_END()

} // unit_tests
} // libq
