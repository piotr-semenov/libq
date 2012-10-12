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

    BOOST_AUTO_TEST_SUITE(Sign)

    /// idea of test:
    ///     common checks if sign inference is accurate
    BOOST_AUTO_TEST_CASE(commonCheck)
    {
        typedef S_number<28, 13>::type type1;
        typedef U_number<37, 15>::type type2;
        typedef U_number<12, 0>::type type3;

        std::string const message("sign inference is wrong");
        #define COMMON_CHECK_IMPL(i, j, s) BOOST_CHECK_MESSAGE(s##(sign_info<type##i, type##j>::sign), message)
        #define COMMON_CHECK(i, j) COMMON_CHECK_IMPL(i, j, )
        #define COMMON_CHECK1(i, j) COMMON_CHECK_IMPL(i, j, !)

        COMMON_CHECK(1, 2);
        COMMON_CHECK1(2, 3);
        COMMON_CHECK(1, 3);
    }

    /// idea of test:
    ///     inference of product type sign must be reflexive
    BOOST_AUTO_TEST_CASE(reflexivityCheck)
    {
        typedef S_number<12, 9>::type type1;
        typedef S_number<3, 0>::type type2;
        typedef U_number<25, 26>::type type3;

        std::string const message("inference of product type sign does not satisfy reflexivity law");
        #define REFLEXIVITY_CHECK(i) BOOST_CHECK_MESSAGE((std::numeric_limits<type##i>::is_signed ==  \
                                        sign_info<type##i, type##i>::sign), message)

        REFLEXIVITY_CHECK(1);
        REFLEXIVITY_CHECK(2);
        REFLEXIVITY_CHECK(3);
    }

    /// idea of test:
    ///    inference of product type sign must be symmetric
    BOOST_AUTO_TEST_CASE(symmetryCheck)
    {
        typedef S_number<28, 13>::type type1;
        typedef U_number<37, 15>::type type2;
        typedef U_number<12, 0>::type type3;

        std::string const message("inference of product type sign does not satisfy symmetry law");
        #define SYMMETRY_CHECK(i, j) BOOST_CHECK_MESSAGE((sign_info<type##i, type##j>::sign ==  \
                                        sign_info<type##i, type##j>::sign), message)

        SYMMETRY_CHECK(1, 2);
        SYMMETRY_CHECK(1, 3);
        SYMMETRY_CHECK(2, 3);
    }

    BOOST_AUTO_TEST_SUITE_END()
}}
