#define BOOST_TEST_STATIC_LINK

#include <boost/test/unit_test.hpp>
#include <boost/integer.hpp>

#include <limits>
#include <string>
#include <stdexcept>

#include "./../../fixed_point_lib/src/number.hpp"

namespace utils { namespace unit_tests {
    namespace c = core;

    BOOST_AUTO_TEST_SUITE(Division)

    /// idea of test:
    /// 1. we choose two fixed-number formats [t1, f1] and [t2, f2] those satisfy:
    ///    t1 = t2 and f1 >> f2
    /// 2. we assign first format to variable A and second one to variable B
    /// 3. B has such value that (B << (f1 - f2)) is out of t1 dynamic interval bounds
    /// so, fixed-point alignment has to be failed during fixed-point numbers division
    BOOST_AUTO_TEST_CASE(positiveOverflowCheck)
    {
        c::SOU_number<28, 20>::type const a(-230.2);
        c::SOU_number<28, 20>::type const b(0.0001);
        c::SOU_number<38, 4>::type const c(100123);
        c::SOU_number<38, 4>::type const d(255);

        try {
            a / b;
            BOOST_FAIL("Negative overflow was not detected");
        }
        catch (std::overflow_error e){}

        try {
            b / c;
            BOOST_FAIL("Positive overflow was not detected");
        }
        catch (std::overflow_error e){}

        try {
            b / d;
            BOOST_FAIL("Underflow was not detected");
        }
        catch (std::underflow_error e){}
    }

    /// idea of test:
    /// check a precision of fixed-point division by several cases with floating-point numbers
    BOOST_AUTO_TEST_CASE(legalDivision)
    {
        c::S_number<28, 14>::type const a(-1.232);
        c::S_number<28, 20>::type const b(-2.13245);
        c::U_number<34, 19>::type const c(1201.232);

        float c12(a / b);
        float c21(b / a);
        float c13(a / c);

        std::string const message("division was made with illegal rounding error");
        BOOST_ASSERT((message, std::fabs(c12 - 0.5777) < 1E-03));
        BOOST_ASSERT((message, std::fabs(c21 - 1.73088) < 1E-04));

        BOOST_ASSERT((message, std::fabs(c13 + 0.00103) < 1E-04));
    }

    BOOST_AUTO_TEST_SUITE_END()
}}
