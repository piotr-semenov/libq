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

    BOOST_AUTO_TEST_SUITE(Ordering)

        // idea of test:
        //     some common checks for set of fixed-point numbers being a
        //     partially ordered set
        BOOST_AUTO_TEST_CASE(POSCheck)
    {
        typedef S_number<43, 43>::type type;

        type const a(-0.09), b(-0.23), c(0.32);
        std::string const message("set of fixed-points is not partially ordered");

        // 1. check if order '<=' is reflexive
        bool check = (a <= a) && (b <= b) && (c <= c);
        BOOST_CHECK_MESSAGE(check, message);

        // 2. check if order '<=' is antisymmetric
        check = !(a <= b && b <= a) && !(a <= c && c <= a) && !(b <= c && c <= b);
        BOOST_CHECK_MESSAGE(check, message);

        // 3. check if order '<=' is transitive
        check = (b <= a) && (a <= c) && (b <= c);
        BOOST_CHECK_MESSAGE(check, message);
    }

    BOOST_AUTO_TEST_SUITE_END()
}}
