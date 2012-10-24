#define BOOST_TEST_STATIC_LINK
#define BOOST_TEST_MODULE FIXED_POINT_LIB_COMMON_USE_CASES

#include <boost/test/unit_test.hpp>
#include <boost/foreach.hpp>
#include <boost/range/irange.hpp>

#include <limits>

#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>

#include "./../../fixed_point_lib/src/number.hpp"

namespace utils { namespace unit_tests {
    #define iterations 100000

    namespace {
        using utils::SOU_number;
        using utils::UOU_number;

        double r(double low, double high)
        {
            return low + std::rand() / (double(RAND_MAX) / (high - low));
        }
    }

    #define fmin(type) double(std::numeric_limits<type>::min())
    #define fmax(type) double(std::numeric_limits<type>::max())

    BOOST_AUTO_TEST_SUITE(Summation)

    // FIXED-POINT SUMMATION PRECISION TESTS
    //////////////////////////////////////////////////////////////////////////
    /// idea of tests 'commonCheck1' and 'commonCheck2':
    ///     1. common checks for fixed-point accuracy
    ///     2. number of accurate decimal numbers in fractional part is determined
    ///     as floor(log(10, 2^(f + 1) - 1)) - 1. Last number is errored by left
    ///     non-captured digits.
    BOOST_AUTO_TEST_CASE(commonCheck1)
    {
        typedef UOU_number<28, 13>::type type1;
        typedef UOU_number<37, 15>::type type2;

        std::srand(static_cast<unsigned int>(std::time(0)));

        BOOST_FOREACH(size_t it, boost::irange<size_t>(0, iterations, 1)) {
            // shortening range not to take care about rounding errors at the
            // bounds
            double u1 = r(fmin(type1) + 1u, fmax(type1) - 1u);
            double u2 = r(std::max(fmin(type1) + 1u - u1, fmin(type2) + 1u),
                std::min(fmax(type1) - 1u - u1, fmax(type2) - 1u));

            type1 const a(u1);
            type2 const b(u2);
            type1::sum_type const result = a + b;

            std::stringstream message_stream;
            message_stream
                << std::setprecision(5)
                << u1
                << " + "
                << u2
                << ": summation was made with unexpected rounding error";

            BOOST_CHECK_MESSAGE(std::fabs(double(result) - (u1 + u2)) < 1E-3,
                message_stream.str());
        }
    }

    BOOST_AUTO_TEST_CASE(commonCheck2)
    {
        typedef SOU_number<56, 34>::type type1;
        typedef SOU_number<56, 47>::type type2;

        std::srand(static_cast<unsigned int>(std::time(0)));

        BOOST_FOREACH(size_t it, boost::irange<size_t>(0, iterations, 1)) {
            // shortening range not to take care about rounding errors at the
            // bounds
            double u1 = r(fmin(type1) + 1, fmax(type1) - 1);
            double u2 = r(std::max(fmin(type1) + 1 - u1, fmin(type2) + 1u),
                std::min(fmax(type1) - 1 - u1, fmax(type2) - 1u));

            type1 const a(u1);
            type2 const b(u2);
            type1::sum_type const result = a + b;

            std::stringstream message_stream;
            message_stream
                << std::setprecision(11)
                << u1
                << " + "
                << u2
                << ": summation was made with unexpected rounding error";

            BOOST_CHECK_MESSAGE(std::fabs(double(result) - (u1 + u2)) < 1E-9,
                message_stream.str());
        }
    }

    // POSITIVE/NEGATIVE OVERFLOW EVENTS HANDLING
    //////////////////////////////////////////////////////////////////////////
    BOOST_AUTO_TEST_CASE(positiveOverflow)
    {
        typedef SOU_number<8, 3>::type type;

        type const a(31.9);
        type const b(0.2);
        try {
            type const c(a + b);

            BOOST_FAIL("Positive overflow event was not captured");
        }
        catch (std::overflow_error e){};
    }

    BOOST_AUTO_TEST_CASE(negativeOverflow)
    {
        typedef SOU_number<8, 3>::type type;

        type const a(-31.9);
        type const b(-0.2);
        try {
            type const c(a + b);

            BOOST_FAIL("Negative overflow event was not captured");
        }
        catch (std::overflow_error e){};
    }

    BOOST_AUTO_TEST_SUITE_END()

    #undef iterations
    #undef fmin
    #undef fmax
}}
