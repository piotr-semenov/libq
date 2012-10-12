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

    BOOST_AUTO_TEST_SUITE(Multiplication)

    // FIXED-POINT MULTIPLICATION PRECISION TESTS
    //////////////////////////////////////////////////////////////////////////
    /// idea of tests 'commonCheck1' and 'commonCheck2':
    ///     common checks if multiplication is accurate
    BOOST_AUTO_TEST_CASE(commonCheck1)
    {
        typedef S_number<56, 13>::type type1;
        typedef U_number<47, 17>::type type2;

        type1 const a(-34242.431);
        type2 const b(1231.321);

        type1::product_type const result = a * b;

        std::string const message("multiplication was made with illegal rounding error");
        BOOST_CHECK_MESSAGE(std::fabs(static_cast<double>(result) + 4.21634E+7) < 1E+2, message);
    }

    BOOST_AUTO_TEST_CASE(commonCheck2)
    {
        typedef U_number<24, 14>::type type1;
        typedef S_number<32, 16>::type type2;

        type1 const a(23.312);
        type2 const b(10.0343);

        type1::sum_type const result = a * b;

        std::string const message("multiplication was made with illegal rounding error");
        BOOST_CHECK_MESSAGE(std::fabs(static_cast<double>(result) - 233.92) < 1E-2, message);
    }

    // FIXED-POINT MULTIPLICATION IS ABELIAN GROUP IN CASE OF OPERANDS HAVING SAME TYPES
    //////////////////////////////////////////////////////////////////////////
    // idea of tests 'unitCheck' and 'commutativityCheck'
    //      checks if it satisfies laws of abelian multiplicative group
    BOOST_AUTO_TEST_CASE(unitCheck)
    {
        typedef S_number<15, 9>::type type;
        type const unit(1.0), a(2.4), b(-3.1), c(0.2);

        std::string const message("multiplication does not have unit");
        BOOST_CHECK_MESSAGE((type(a * unit) == type(unit * a)) && (type(unit * a) == type(a)), message);
        BOOST_CHECK_MESSAGE((type(b * unit) == type(unit * b)) && (type(unit * b) == type(b)), message);
        BOOST_CHECK_MESSAGE((type(c * unit) == type(unit * c)) && (type(unit * c) == type(c)), message);
    }

    BOOST_AUTO_TEST_CASE(commutativityCheck)
    {
        typedef S_number<30, 31>::type type;
        type const a(0.0097), b(0.0465), c(0.0992);

        std::string const message("multiplication does not satisfy commutativity law");
        BOOST_CHECK_MESSAGE(a * b == b * a, message);
        BOOST_CHECK_MESSAGE(b * c == c * b, message);
        BOOST_CHECK_MESSAGE(a * c == c * a, message);
    }

    // check for inverses is considered in division_cases.cpp

    BOOST_AUTO_TEST_SUITE_END()
}}
