#define BOOST_TEST_STATIC_LINK
#define BOOST_TEST_MODULE SUBTRACTION_CASES

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
        ///     common checks if fixed-point subtraction is accurate and sign inference
        ///     is performed accurately
        BOOST_AUTO_TEST_CASE(commonCheck1)
    {
        typedef S_number<59, 48>::type type1;
        typedef U_number<37, 48>::type type2;

        type1 const a(-123.12321);
        type2 const b(50.23128);

        type1::sum_type const result = a - b;

        std::string const message("subtraction was made with illegal rounding error");
        BOOST_CHECK_MESSAGE(std::fabs(static_cast<double>(result) + 173.35449) < 1E-5, message);
    }

    BOOST_AUTO_TEST_CASE(commonCheck2)
    {
        typedef U_number<32, 12>::type type1;
        typedef S_number<56, 23>::type type2;

        type1 const a(15.001);
        type2 const b(0.001);

        type1::sum_type const result = a - b;

        std::string const message("subtraction was made with illegal rounding error");
        BOOST_CHECK_MESSAGE(std::fabs(static_cast<double>(result) - 15.0) < 1E-3, message);
    }

    // SAME-TYPE FIXED-POINT NUMBERS ARE FROM THE ADDITIVE ABELIAN GROUP
    //////////////////////////////////////////////////////////////////////////
    // idea of test 'negativesCheck'
    //      checks if the abelian additive group laws are satisfied
    BOOST_AUTO_TEST_CASE(negativesCheck)
    {
        typedef S_number<54, 45>::type type;
        type const a(234.234), b(-34.5432), c(109.12358), unit(0);

        std::string const message("fixed-point arithmetics does not satisfy law of negatives");
        BOOST_CHECK_MESSAGE(a - a == type::diff_type(unit), message);
        BOOST_CHECK_MESSAGE(b - b == type::diff_type(unit), message);
        BOOST_CHECK_MESSAGE(c - c == type::diff_type(unit), message);

        typedef U_number<0, 67>::type type1;
        type const d(0.023143), e(0.000201), f(0.000001);
        BOOST_CHECK_MESSAGE(d - d == type::diff_type(unit), message);
        BOOST_CHECK_MESSAGE(e - e == type::diff_type(unit), message);
        BOOST_CHECK_MESSAGE(f - f == type::diff_type(unit), message);
    }

    BOOST_AUTO_TEST_SUITE_END()
}}
