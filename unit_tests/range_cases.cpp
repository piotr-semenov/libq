#define BOOST_TEST_STATIC_LINK
#define BOOST_TEST_MODULE FIXED_POINT_LIB_COMMON_USE_CASES

#include <boost/test/unit_test.hpp>

#include <string>
#include <limits>
#include <stdexcept>

#include "./../../fixed_point_lib/src/fixed_point.hpp"

namespace core { namespace unit_tests {
    namespace {
        using core::SOU_fixed_point;
        using core::UOU_fixed_point;
    }

    BOOST_AUTO_TEST_SUITE(Range)

    /// test 'division_checkRange':
    ///     check if the largest and the smallest rationals are enabled with
    ///     fixed-point promotion format derived after division operation
    BOOST_AUTO_TEST_CASE(division_check_range)
    {
#define MAX(n, f) std::numeric_limits<S_fixed_point<n, f>::type>::max()
#define MIN(n, f) SOU_fixed_point<n, f>::type::wrap(1)

#define PRECISION_CHECK(fp, val, precision, msg) \
    BOOST_CHECK_MESSAGE(std::fabs(double(fp) - val) < precision, msg);

        std::string const upper_bound_err("range upper bound has not been captured");
        std::string const lower_bound_err("range lower bound has not been captured");

        PRECISION_CHECK(MAX(7,6)/MIN(17,12), 8128, 1E-9, upper_bound_err);
        PRECISION_CHECK(MAX(23,12)/MIN(6,4), 32767.99609375, 1E-9, upper_bound_err);
        PRECISION_CHECK(MAX(34,23)/MIN(26,23), 17179869183.0, 1E-9, upper_bound_err);
        PRECISION_CHECK(MAX(19,2)/MIN(43,26), 8796076244992.0, 1E-9, upper_bound_err);
        PRECISION_CHECK(MAX(35,34)/MIN(23,22), 8388607.99975, 1E-5, upper_bound_err);

        PRECISION_CHECK(MIN(7,6)/MAX(17,12), 0.000488285, 1E-8, lower_bound_err);
        PRECISION_CHECK(MIN(23,12)/MAX(6,4), 0.000062003968, 1E-5, lower_bound_err);
        PRECISION_CHECK(MIN(34,23)/MAX(26,23), 1.490116E-8, 1E-9, lower_bound_err);
        PRECISION_CHECK(MIN(19,2)/MAX(43,26), 1.907348E-6, 1E-9, lower_bound_err);
        PRECISION_CHECK(MIN(35,34)/MAX(23,22), 2.91038E-11, 1E-16, lower_bound_err);

#undef PRECISION_CHECK
#undef MIN
#undef MAX
    }

    /// test 'out_of_range_policies':
    ///     common checks if integer value (interpreted as fixed-point) is out
    ///     of range
    BOOST_AUTO_TEST_CASE(out_of_range_policies)
    {
        typedef SOU_fixed_point<8, 5>::type type;

        std::string const message("Positive overflow was not detected");
        try {
            type(std::numeric_limits<type>::max() + 1);

            BOOST_FAIL(message);
        }
        catch (std::overflow_error e){}

        try {
            type(std::numeric_limits<type>::min() - 1);

            BOOST_FAIL(message);
        }
        catch (std::overflow_error e){}

    }
    BOOST_AUTO_TEST_SUITE_END()
}}
