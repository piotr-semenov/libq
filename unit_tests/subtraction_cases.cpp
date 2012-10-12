#define BOOST_TEST_STATIC_LINK

#include <boost/test/unit_test.hpp>
#include <boost/integer.hpp>

#include <limits>
#include <string>

#include "./../../fixed_point_lib/src/number.hpp"
#include "./../../fixed_point_lib/src/sign_info.hpp"

namespace utils { namespace unit_tests {
    namespace {
        using utils::S_number;
        using utils::U_number;

        using utils::sign_info;
    }

    BOOST_AUTO_TEST_SUITE(Subtraction)

        // FIXED-POINT SUBTRACTION PRECISION TESTS
        //////////////////////////////////////////////////////////////////////////
        /// idea of tests 'commonCheck1' and 'commonCheck2':
        ///     common checks if sign inference is accurate
        BOOST_AUTO_TEST_CASE(commonCheck1)
    {
        typedef S_number<59, 48>::type type1;
        typedef U_number<37, 48>::type type2;

        type1 const a(-123.12321);
        type2 const b(50.23129);

        type1::sum_type const result = a - b;

        std::string const message("subtraction was made with illegal rounding error");
        BOOST_CHECK_MESSAGE(std::fabs(static_cast<double>(result) + 173.3545) < 1E-5, message);
    }

    BOOST_AUTO_TEST_CASE(commonCheck2)
    {
        typedef U_number<32, 12>::type type1;
        typedef S_number<56, 23>::type type2;

        type1 const a(15.001);
        type2 const b(0.001);

        type1::sum_type const result = a - b;

        std::string const message("subtraction was made with illegal rounding error");
        BOOST_CHECK_MESSAGE(std::fabs(static_cast<double>(result) - 15.0) < 1E-4, message);
    }

    // FIXED-POINT SUMMATION IS ABELIAN GROUP IN CASE OF OPERANDS HAVING SAME TYPES
    //////////////////////////////////////////////////////////////////////////
    // idea of test 'negativesCheck'
    //      checks if it satisfies laws of abelian additive group
    BOOST_AUTO_TEST_CASE(negativesCheck)
    {
        typedef S_number<54, 45>::type type;
        type const a(234.234), b(-34.5432), c(109.12358), unit(0);

        std::string const message("fixed-point arithmetics does not satisfy law of negatives");
        BOOST_CHECK_MESSAGE(a - a == type::diff_type(unit), message);
        BOOST_CHECK_MESSAGE(b - b == type::diff_type(unit), message);
        BOOST_CHECK_MESSAGE(c - c == type::diff_type(unit), message);
    }

    // check for unit and commutativity is considered in sum_cases.cpp

    BOOST_AUTO_TEST_SUITE_END()
}}
