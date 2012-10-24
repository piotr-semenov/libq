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
    }

    #define fmin(type) double(std::numeric_limits<type>::min())
    #define fmax(type) double(std::numeric_limits<type>::max())

    double r(double low, double high)
    {
        return low + std::rand() / (double(RAND_MAX) / (high - low));
    }

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

    // SAME TYPE FIXED-POINT NUMBERS BUILDS AN ADDITIVE ABELIAN GROUP
    //////////////////////////////////////////////////////////////////////////
    // idea of tests 'unitCheck' and 'commutativityCheck'
    //      checks if the additive abelian group laws are satisifed
    BOOST_AUTO_TEST_CASE(unitCheck)
    {
        typedef S_number<43, 23>::type type;
        type const unit(0.0), a(32.435), b(-23.123), c(0.034);

        std::string const message("summation does not have unit");
        BOOST_CHECK_MESSAGE((type(a + unit) == type(unit + a)) && (type(unit + a) == type(a)), message);
        BOOST_CHECK_MESSAGE((type(b + unit) == type(unit + b)) && (type(unit + b) == type(b)), message);
        BOOST_CHECK_MESSAGE((type(c + unit) == type(unit + c)) && (type(unit + c) == type(c)), message);
    }

    BOOST_AUTO_TEST_CASE(commutativityCheck)
    {
        typedef S_number<27, 27>::type type;
        type const a(0.0023), b(0.095), c(0.9998);

        std::string const message("summation does not satisfy commutativity law");
        BOOST_CHECK_MESSAGE(a + b == b + a, message);
        BOOST_CHECK_MESSAGE(b + c == c + b, message);
        BOOST_CHECK_MESSAGE(a + c == c + a, message);
    }

    BOOST_AUTO_TEST_SUITE_END()
}}
