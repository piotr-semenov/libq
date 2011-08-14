#define BOOST_TEST_STATIC_LINK

#include <boost/test/unit_test.hpp>
#include <boost/integer.hpp>

#include <limits>
#include <string>

#include "./../../Common/number.hpp"

namespace utils { namespace unit_tests {
    namespace {
        using utils::S_number;
        using utils::U_number;
    }

    BOOST_AUTO_TEST_SUITE(Normalization)

    // idea of test:
    //     some common checks if normalization (different fixed-point alignment)
    //     is correct
    BOOST_AUTO_TEST_CASE(check)
    {
        typedef S_number<28, 13>::type type1;
        typedef S_number<28, 14>::type type2;

        type1 const a(123.9), a1(1432.23);
        type2 const b(123.9), b1(1432.23);

        // check with respect to rounding errors
        #define test(x, y) (x == y) || (x + 1 == y) || (x - 1 == y)

        std::string const message("normalization is failed");
        type2::value_type converted = type2::normalize_of<type1::value_type, type1::fractionals>(a.value());
        BOOST_CHECK_MESSAGE(test(b.value(), converted), message);

        converted = type2::normalize_of<type1::value_type, type1::fractionals>(a1.value());
        BOOST_CHECK_MESSAGE(test(b1.value(), converted), message);
    }

    BOOST_AUTO_TEST_SUITE_END()
}}
