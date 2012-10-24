#define BOOST_TEST_STATIC_LINK

#include <boost/test/unit_test.hpp>
#include <boost/foreach.hpp>
#include <boost/range/irange.hpp>

#include <limits>

#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>

#include "./../../fixed_point_lib/src/number.hpp"

namespace utils { namespace unit_tests {
    #define iterations 100000

    namespace {
        using utils::SOU_number;
        using utils::UOU_number;

        double r(double low, double high)
        {
            return low + std::rand() / (double(RAND_MAX) / (high - low));
        }
    }

    #define fmin(type) double(std::numeric_limits<type>::min())
    #define fmax(type) double(std::numeric_limits<type>::max())

    BOOST_AUTO_TEST_SUITE(Multiplication)

    // FIXED-POINT MULTIPLICATION PRECISION TESTS
    //////////////////////////////////////////////////////////////////////////
    /// idea of tests 'commonCheck1' and 'commonCheck2':
    ///     common checks if multiplication is accurate and if sign is
    ///     inferenced accurately
    BOOST_AUTO_TEST_CASE(commonCheck1)
    {
        typedef SOU_number<31, 13>::type type1;
        typedef UOU_number<29, 17>::type type2;

        std::srand(static_cast<unsigned int>(std::time(0)));

        BOOST_FOREACH(size_t it, boost::irange<size_t>(0, iterations, 1)) {
            // shortening range not to take care about rounding errors at the
            // bounds
            double u1 = r(fmin(type1) + 1u, fmax(type1) - 1u);
            double u2 = r(fmin(type2) + 1u, fmax(type2) - 1u);

            type1 const a(u1);
            type2 const b(u2);
            type1::product_type const result = a * b;

            std::stringstream message_stream;
            message_stream
                << std::setprecision(10)
                << u1
                << " * "
                << u2
                << ": multiplication was made with unexpected rounding error";

            BOOST_CHECK_MESSAGE(std::fabs(double(result) - (u1 * u2)) <
                1E-3 * (std::fabs(u1) + std::fabs(u2)), message_stream.str());
        }
    }

    BOOST_AUTO_TEST_CASE(commonCheck2)
    {
        typedef UOU_number<31, 16>::type type1;
        typedef SOU_number<32, 16>::type type2;

        std::srand(static_cast<unsigned int>(std::time(0)));

        BOOST_FOREACH(size_t it, boost::irange<size_t>(0, iterations, 1)) {
            // shortening range not to take care about rounding errors at the
            // bounds
            double u1 = r(fmin(type1) + 1u, fmax(type1) - 1u);
            double u2 = r(fmin(type1) + 1u, fmax(type1) - 1u);

            type1 const a(u1);
            type2 const b(u2);
            type1::product_type const result = a * b;

            std::stringstream message_stream;
            message_stream
                << std::setprecision(10)
                << u1
                << " * "
                << u2
                << ": multiplication was made with unexpected rounding error";

            BOOST_CHECK_MESSAGE(std::fabs(double(result) - (u1 * u2)) <
                1E-3 * (std::fabs(u1) + std::fabs(u2)), message_stream.str());
        }
    }

    // TEST FOR OVERFLOW EVENTS
    //////////////////////////////////////////////////////////////////////////
    BOOST_AUTO_TEST_CASE(exceptionCheck)
    {
        typedef UOU_number<7, 0>::type type;
        type const a(127), b(2);

        try {
            type const c(a * b);

            BOOST_FAIL("Positive overflow was not detected");
        }
        catch(std::overflow_error e){}

        typedef UO_number<64, 0>::type type1;
        type1 const d(12), e(3432);

        d * e;
    }

    BOOST_AUTO_TEST_SUITE_END()

    #undef iterations
    #undef fmin
    #undef fmax
}}
