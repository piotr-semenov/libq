#define BOOST_TEST_STATIC_LINK
#define BOOST_TEST_MODULE FIXED_POINT_LIB_COMMON_USE_CASES

#include <boost/test/unit_test.hpp>

#include <boost/random.hpp>

#include <limits>

#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>

#include "./../../fixed_point_lib/src/number.hpp"

namespace utils { namespace unit_tests {
    namespace {
        using utils::S_number;
        using utils::U_number;

        using utils::SOU_number;
        using utils::UOU_number;

        boost::mt19937 rng;
        typedef boost::variate_generator<boost::mt19937&, boost::uniform_real<double> >
            generator;
        typedef boost::uniform_real<double> var;
    }

    BOOST_AUTO_TEST_SUITE(Summation)

    // FIXED-POINT SUMMATION PRECISION TESTS
    //////////////////////////////////////////////////////////////////////////
    /// idea of tests 'commonCheck1' and 'commonCheck2':
    ///     1. common checks for fixed-point accuracy and result sign inference
    ///     2. number of accurate decimal numbers in fractional part is determined
    ///     as floor(log(10, 2^(f + 1) - 1)) - 1. Last number is errored by left
    ///     non-captured digits.
    BOOST_AUTO_TEST_CASE(commonCheck1)
    {
        typedef S_number<28, 13>::type type1;
        typedef U_number<37, 15>::type type2;

        var v0(static_cast<double>(std::numeric_limits<type1>::min()),
            static_cast<double>(std::numeric_limits<type1>::max()));
        double const u0 = generator(rng, v0)();

        var v1(static_cast<double>(std::numeric_limits<type2>::min()),
            static_cast<double>(std::numeric_limits<type1>::max()) - u0);
        double const u1 = generator(rng, v1)();

        type1 const a(u0);
        type1 const b(u1);

        type1::sum_type const result = a + b;

        std::stringstream message_stream;
        message_stream
            << std::setprecision(5)
            << u0
            << " + "
            << u1
            << ": summation was made with unexpected rounding error";

        double const error = static_cast<double>(result) - (u0 + u1);
        BOOST_CHECK_MESSAGE(std::fabs(static_cast<double>(result) - (u0 + u1)) < 1E-3,
            message_stream.str());
    }

    BOOST_AUTO_TEST_CASE(commonCheck2)
    {
        typedef U_number<56, 34>::type type1;
        typedef S_number<56, 57>::type type2;

        type1 const a(13.5462);
        type2 const b(0.00343);

        type1::sum_type const result = a + b;

        BOOST_CHECK_MESSAGE(std::fabs(static_cast<double>(result) - 13.54963) < 1E-5,
            "summation was made with illegal rounding error");
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

            BOOST_FAIL("Overflow event was not captured");
        }
        catch (std::overflow_error e){};
    }

    BOOST_AUTO_TEST_CASE(negativeOverflow)
    {
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
