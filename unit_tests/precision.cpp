#define BOOST_TEST_STATIC_LINK

#include <boost/test/unit_test.hpp>
#include "./../../fixed_point_lib/src/fixed_point.hpp"

#include <iomanip>
#include <sstream>

#include <exception>

namespace core { namespace unit_tests {
    namespace {
        using core::S_fixed_point;
        using core::U_fixed_point;

        double r(double low, double high)
        {
            if (low > high) {
                throw std::exception("illegal range");
            }
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
#define PRECISION_TEST(t, error) \
    BOOST_FOREACH(size_t it, boost::irange<size_t>(0, ITERATIONS, 1)) { \
    /* shortening range not to take care about rounding errors at the bounds */ \
        double u1 = r(FMIN(t) + 1u, FMAX(t) - 1u); \
        double u2 = r(FMIN(t) + 1u, FMAX(t) - 1u); \
        \
        t const a(u1); t const b(u2); \
        try { \
            if (std::fabs(double(a + b) - (u1 + u2)) > 2 * error) { \
                std::stringstream message_stream; \
                message_stream \
                    << std::setprecision(5) \
                    << u1 \
                    << " + " \
                    << u2 \
                    << ": sum was made with unexpected rounding error"; \
                \
                BOOST_CHECK_MESSAGE(false, message_stream.str()); \
            } \
        } \
        catch(std::exception){ \
            std::stringstream message_stream; \
            message_stream \
                << u1 << " " << u2; \
            BOOST_CHECK_MESSAGE(false, message_stream.str()); \
        } \
    }

        U(28,13,t1); PRECISION_TEST(t1, 1E-2); // floor(log(10,2^13-1))-1=2
        S(56,34,t2); PRECISION_TEST(t2, 1E-9); // floor(log(10,2^34-1))-1=9
        S(61,52,t3); PRECISION_TEST(t3, 1E-14); // floor(log(10,2^52-1))-1=14
        U(53,23,t4); PRECISION_TEST(t4, 1E-5); // floor(log(10, 2^22-1))-1=5

#undef PRECISION_TEST
    }

    BOOST_AUTO_TEST_CASE(subtraction)
    {
#define PRECISION_TEST(t, error) \
    BOOST_FOREACH(size_t it, boost::irange<size_t>(0, ITERATIONS, 1)) { \
    /* shortening range not to take care about rounding errors at the bounds */ \
        double u1 = r(FMIN(t) + 1u, FMAX(t) - 1u); \
        double u2 = r(FMIN(t) + 1u, FMAX(t) - 1u); \
        \
        t const a(u1); t const b(u2); \
        try { \
            if (std::fabs(double(a - b) - (u1 - u2)) > 2 * error) { \
                std::stringstream message_stream; \
                message_stream \
                    << std::setprecision(5) \
                    << u1 \
                    << " - " \
                    << u2 \
                    << ": subtraction was made with unexpected rounding error"; \
                \
                BOOST_CHECK_MESSAGE(false, message_stream.str()); \
            } \
        } \
        catch(std::exception){ \
            std::stringstream message_stream; \
            message_stream \
                << u1 << " " << u2; \
            BOOST_CHECK_MESSAGE(false, message_stream.str()); \
        } \
    }
        S(28,13,t1); PRECISION_TEST(t1, 1E-2); // floor(log(10,2^13-1))-1=2
        S(56,34,t2); PRECISION_TEST(t2, 1E-9); // floor(log(10,2^34-1))-1=9
        S(61,52,t3); PRECISION_TEST(t3, 1E-14); // floor(log(10,2^52-1))-1=14
        S(53,23,t4); PRECISION_TEST(t4, 1E-5); // floor(log(10, 2^22-1))-1=5

#undef PRECISION_TEST
    }

    BOOST_AUTO_TEST_CASE(multiplication)
    {
#define PRECISION_TEST(t1, t2, error) \
    BOOST_FOREACH(size_t it, boost::irange<size_t>(0, ITERATIONS, 1)) { \
        double u1 = r(FMIN(t1), FMAX(t1)); \
        double u2 = r(FMIN(t2), FMAX(t2)); \
        \
        try { \
            t1 const a(u1); t2 const b(u2); \
            if (std::fabs(double(a * b) - (u1 * u2)) > error * (std::fabs(u1) + std::fabs(u2))) { \
                std::stringstream message_stream; \
                message_stream \
                    << std::setprecision(5) \
                    << u1 \
                    << " * " \
                    << u2 \
                    << ": multiplication was made with unexpected rounding error"; \
                \
                BOOST_CHECK_MESSAGE(false, message_stream.str()); \
            } \
        } \
        catch(std::exception){ \
            std::stringstream message_stream; \
            message_stream \
                << u1 << " " << u2; \
            BOOST_CHECK_MESSAGE(false, message_stream.str()); \
        } \
    }

        U(18,13,t1); U(20,15,t2); PRECISION_TEST(t1,t2, 1E-3); // floor(log(10,2^13-1))-1=2
        S(24,23,t3); S(30,22,t4); PRECISION_TEST(t3,t4, 1E-5); // floor(log(10,2^22-1))-1=5
        S(30,29,t5); S(33,33,t6); PRECISION_TEST(t5,t6, 1E-7); // floor(log(10,2^22-1))-1=7

#undef PRECISION_TEST
    }

    BOOST_AUTO_TEST_CASE(division)
    {
#define PRECISION_TEST(t1, t2, error) \
    BOOST_FOREACH(size_t it, boost::irange<size_t>(0, ITERATIONS, 1)) { \
        double u1 = r(FMIN(t1), FMAX(t1)); \
        double u2 = r(FMIN(t2), FMAX(t2)); \
        if (u2 == 0) { \
            continue; \
        } \
        \
        try { \
            t1 const a(u1); t2 const b(u2); \
            double const u1a(std::fabs(u1)); double const u2a(std::fabs(u2)); \
            /* supremum of the error */ \
            if (std::fabs(double(a / b) - (u1 / u2)) > error * ((u1a + u2a) / (u2a * std::fabs(u2a - error)))) { \
                std::stringstream message_stream; \
                message_stream \
                    << std::setprecision(5) \
                    << u1 \
                    << " / " \
                    << u2 \
                    << ": division was made with unexpected rounding error"; \
                \
                BOOST_CHECK_MESSAGE(false, message_stream.str()); \
            } \
        } \
        catch(std::exception){ \
            std::stringstream message_stream; \
            message_stream \
                << u1 << " " << u2; \
            BOOST_CHECK_MESSAGE(false, message_stream.str()); \
        } \
    }

        U(18,13,t1); U(20,15,t2); PRECISION_TEST(t1,t2, 1E-2); // floor(log(10,2^13-1))-1=2
        S(24,23,t3); S(30,22,t4); PRECISION_TEST(t3,t4, 1E-5); // floor(log(10,2^22-1))-1=5
        S(30,29,t5); S(33,33,t6); PRECISION_TEST(t5,t6, 1E-7); // floor(log(10,2^22-1))-1=7

#undef PRECISION_TEST
    }

    BOOST_AUTO_TEST_CASE(log)
    {
#define PRECISION_TEST(t, low, high, f, error) \
    BOOST_FOREACH(size_t i, boost::irange<size_t>(0, ITERATIONS, 1)) \
    { \
        double const u(r(low, high)); \
        t::##f##_type const x = std::##f##(fp(u)); \
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

    //U(23,17,t1); PRECISION_TEST(t, 

#undef PRECISION_TEST
    }

    BOOST_AUTO_TEST_SUITE_END()
}}
