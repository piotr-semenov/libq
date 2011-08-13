#define BOOST_TEST_STATIC_LINK

#include <boost/test/unit_test.hpp>

#include <string>

#include "./../../Common/number.hpp"

namespace utils { namespace unit_tests {
    namespace {
        using utils::S_number;
        using utils::U_number;

        namespace num = boost::numeric;
    }

    BOOST_AUTO_TEST_SUITE(Range)

    // idea of test:
    //     for common 8-bit, 16-bit, 32-bit formats fixed-point must have the same
    //     range as built-in types have
    BOOST_AUTO_TEST_CASE(rangeFitBultinCheck)
    {
        typedef S_number<7, 6>::type type1;
        typedef U_number<16, 23>::type type2;
        typedef U_number<32, 12>::type type3;
        typedef U_number<64, 54>::type type4;
        typedef S_number<15, 12>::type type5;

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

    // idea of test:
    //     common checks if integer value (interpreted as fixed-point) is out of range
    BOOST_AUTO_TEST_CASE(outOfRangeCheck)
    {
        using num::positive_overflow;
        using num::negative_overflow;

        typedef S_number<8, 13>::type type1;
        typedef U_number<53, 45>::type type2;
        typedef U_number<17, 1>::type type3;

        type3 x(100);

        std::string const message("Positive overflow was not detected");
        try {
            unsigned int const a = 300;

            type1::bounds::internal_cast(a);
            BOOST_FAIL(message);
        }
        catch (positive_overflow e){}

        try {
            unsigned int const a = 10000;

            type3::bounds::internal_cast(a);
            BOOST_FAIL(message);
        }
        catch (positive_overflow e){}

    }

    // idea of test:
    //     common checks if integers (interpreted as fixed-point values) fits the fixed-point format
    BOOST_AUTO_TEST_CASE(rangeFitCheck)
    {
        typedef S_number<44, 23>::type type1;
        typedef S_number<35, 12>::type type2;
        typedef U_number<19, 3>::type type3;

        std::string const message("Wrong overflow detection if integer fits the fixed-point format range");
        try {
            type1::bounds::internal_cast(17592186044415L);
            type1::bounds::internal_cast(-17592186044416L);

            type2::bounds::internal_cast(34359738367L);
            type2::bounds::internal_cast(-34359738368L);

            type3::bounds::internal_cast(524287L);
            type3::bounds::internal_cast(0);
        }
        catch (std::exception e) {
            BOOST_FAIL(message);
        }
    }

    BOOST_AUTO_TEST_SUITE_END()
}}
