#define BOOST_TEST_STATIC_LINK

#include <boost/test/unit_test.hpp>
#include <boost/integer.hpp>

#include <limits>
#include <string>

#include "./../../Common/number.hpp"
#include "./../../Common/sign_info.hpp"

namespace utils { namespace unit_tests {
    namespace {
        using utils::S_number;
        using utils::U_number;

        using utils::sign_info;
    }

    BOOST_AUTO_TEST_SUITE(Summation)

    // FIXED-POINT SUMMATION PRECISION TESTS
    //////////////////////////////////////////////////////////////////////////
    /// idea of tests 'commonCheck1' and 'commonCheck2':
    ///     common checks if sign inference is accurate
    BOOST_AUTO_TEST_CASE(commonCheck1)
    {
        typedef S_number<28, 13>::type type1;
        typedef U_number<37, 15>::type type2;

        type1 const a(-200.23);
        type2 const b(10.231);

        type1::sum_type const result = a + b;

        std::string const message("summation was made with illegal rounding error");
        BOOST_CHECK_MESSAGE(std::fabs(static_cast<double>(result) + 189.99) < 1E-2, message);
    }

    BOOST_AUTO_TEST_CASE(commonCheck2)
    {
        typedef U_number<56, 34>::type type1;
        typedef S_number<56, 57>::type type2;

        type1 const a(13.5462);
        type2 const b(0.00343);

        type1::sum_type const result = a + b;

        std::string const message("summation was made with illegal rounding error");
        BOOST_CHECK_MESSAGE(std::fabs(static_cast<double>(result) - 13.54963) < 1E-6, message);
    }

    // FIXED-POINT SUMMATION IS ABELIAN GROUP IN CASE OF OPERANDS HAVING SAME TYPES
    //////////////////////////////////////////////////////////////////////////
    // idea of tests 'unitCheck' and 'commutativityCheck'
    //      checks if it satisfies laws of abelian additive group
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

    // check for negatives is considered in subtraction_cases.cpp

    BOOST_AUTO_TEST_SUITE_END()
}}
