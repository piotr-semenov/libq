#define BOOST_TEST_STATIC_LINK

#include <boost/test/unit_test.hpp>
#include "./../../fixed_point_lib/src/fixed_point.hpp"

#include <iomanip>
#include <sstream>

namespace core { namespace unit_tests {
    namespace {
        using core::S_fixed_point;
        using core::U_fixed_point;

        double r(double low, double high)
        {
            return low + std::rand() / (double(RAND_MAX) / (high - low));
        }
    }

    BOOST_AUTO_TEST_SUITE(Precision)

#define ITERATIONS size_t(1E+6)
#define FMIN(type) double(std::numeric_limits<type>::min())
#define FMAX(type) double(std::numeric_limits<type>::max())

#define U(n, f, name) typedef UOU_fixed_point<n, f>::type name
#define S(n, f, name) typedef SOU_fixed_point<n, f>::type name

    BOOST_AUTO_TEST_CASE(sum)
    {
#define PRECISION_TEST(t1, t2, error) \
    BOOST_FOREACH(size_t it, boost::irange<size_t>(0, ITERATIONS, 1)) { \
    /* shortening range not to take care about rounding errors at the bounds */ \
        double u1 = r(FMIN(t1) + 1u, FMAX(t1) - 1u); \
        double u2 = r(std::max(FMIN(t1) + 1u - u1, FMIN(t2) + 1u), \
            std::min(FMAX(t1) - 1u - u1, FMAX(t2) - 1u)); \
        \
        t1 const a(u1); t2 const b(u2); \
        if (std::fabs(double(a + b) - (u1 + u2)) > error) { \
            std::stringstream message_stream; \
            message_stream \
                << std::setprecision(5) \
                << u1 \
                << " + " \
                << u2 \
                << ": summation was made with unexpected rounding error"; \
            \
            BOOST_CHECK_MESSAGE(false, message_stream.str()); \
        } \
    }
        U(28,13,t1); U(37,15,t2); PRECISION_TEST(t1,t2, 1E-3); // floor(log(10,2^13-1))-1=2
        S(56,34,t3); S(56,47,t4); PRECISION_TEST(t3,t4, 1E-9); // floor(log(10,2^34-1))-1=9
        S(61,52,t5); S(61,52,t6); PRECISION_TEST(t5,t6, 1E-14); // floor(log(10,2^52-1))-1=14
        U(53,23,t7); S(23,22,t8); PRECISION_TEST(t7,t8, 1E-5); // floor(log(10, 2^22-1))-1=5

#undef PRECISION_TEST
    }

#define PRECISION_TEST(fp, low, high, f, error) \
    BOOST_FOREACH(size_t i, boost::irange<size_t>(0, ITERATIONS, 1)) \
    { \
        double const u(r(low, high)); \
        fp::##f##_type const x = std::##f##(fp(u)); \
        \
        if (std::fabs(double(x) - std::##f##(u)) > error) { \
            std::stringstream message_stream; \
            message_stream \
                << std::setprecision(9) \
                << "unexpected accuracy: " \
                << #f \
                << "(" \
                << u \
                << ") = " \
                << std::##f(u) \
                << " was computed as " \
                << double(x); \
            \
            BOOST_CHECK_MESSAGE(false, message_stream.str()); \
        } \
    }

    BOOST_AUTO_TEST_SUITE_END()
}}
