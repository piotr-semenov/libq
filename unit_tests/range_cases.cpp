#define BOOST_TEST_STATIC_LINK

#include <boost/test/unit_test.hpp>

#include <string>
#include <limits>
#include <stdexcept>

#include "./../../fixed_point_lib/src/fixed_point.hpp"

namespace core { namespace unit_tests {
    namespace {
        using core::SOU_fixed_point;
        using core::UOU_fixed_point;
    }

    BOOST_AUTO_TEST_SUITE(Range)

    // FIXED-POINT RANGE TESTS
    //////////////////////////////////////////////////////////////////////////
    /// idea of test 'rangeFitBuiltinCheck':
    ///     checks if range of fixed-point of word length 8,16,32,64 is the same
    ///     as for built-in integers
    BOOST_AUTO_TEST_CASE(rangeFitBultinCheck)
    {
        typedef SOU_fixed_point<7, 6>::type type1;
        typedef UOU_fixed_point<16, 23>::type type2;
        typedef UOU_fixed_point<32, 12>::type type3;
        typedef UOU_fixed_point<64, 54>::type type4;
        typedef SOU_fixed_point<15, 12>::type type5;

        std::string const message("fixed-point has wrong range");
        typedef boost::int_t<8>::exact bit8_stype;
        BOOST_CHECK_MESSAGE(std::numeric_limits<bit8_stype>::max() == type1::bounds::max, message);
        BOOST_CHECK_MESSAGE(std::numeric_limits<bit8_stype>::min() == type1::bounds::min, message);

        typedef boost::uint_t<16>::exact bit16_utype;
        BOOST_CHECK_MESSAGE(std::numeric_limits<bit16_utype>::max() == type2::bounds::max, message);
        BOOST_CHECK_MESSAGE(std::numeric_limits<bit16_utype>::min() == type2::bounds::min, message);

        typedef boost::uint_t<32>::exact bit32_utype;
        BOOST_CHECK_MESSAGE(std::numeric_limits<bit32_utype>::max() == type3::bounds::max, message);
        BOOST_CHECK_MESSAGE(std::numeric_limits<bit32_utype>::min() == type3::bounds::min, message);

        typedef boost::uint_t<64>::exact bit64_utype;
        BOOST_CHECK_MESSAGE(std::numeric_limits<bit64_utype>::max() == type4::bounds::max, message);
        BOOST_CHECK_MESSAGE(std::numeric_limits<bit64_utype>::min() == type4::bounds::min, message);

        typedef boost::int_t<16>::exact bit16_stype;
        BOOST_CHECK_MESSAGE(std::numeric_limits<bit16_stype>::max() == type5::bounds::max, message);
        BOOST_CHECK_MESSAGE(std::numeric_limits<bit16_stype>::min() == type5::bounds::min, message);
    }

    /// idea of test 'outOfRangeCheck':
    ///     common checks if integer value (interpreted as fixed-point) is out
    ///     of range
    BOOST_AUTO_TEST_CASE(outOfRangeCheck)
    {
        typedef SOU_fixed_point<8, 13>::type type1;
        typedef UOU_fixed_point<13, 1>::type type2;

        std::string const message("Positive overflow was not detected");
        try {
            type1(300u);

            BOOST_FAIL(message);
        }
        catch (std::overflow_error e){}

        try {
            type2(10000u);

            BOOST_FAIL(message);
        }
        catch (std::overflow_error e){}

    }
    BOOST_AUTO_TEST_SUITE_END()
}}
