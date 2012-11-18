#define BOOST_TEST_STATIC_LINK

#include <boost/test/unit_test.hpp>
#include "./../../fixed_point_lib/src/fixed_point.hpp"

#include <boost/random.hpp>

#include <ctime>

#include <iomanip>
#include <fstream>
#include <sstream>

#include <exception>

namespace core { namespace unit_tests {
    namespace {
        using core::S_fixed_point;
        using core::U_fixed_point;

        double r(double low, double high)
        {
            static boost::mt19937 gen(size_t(std::time(0)));
            static boost::uniform_01<boost::mt19937> uniform(gen);
            if (low > high) {
                throw std::exception("illegal range");
            }
            return low + uniform() * (high - low);
        }
    }

    BOOST_AUTO_TEST_SUITE(Precision)

#define ITERATIONS size_t(1E+6)
#define FMIN(type) double(std::numeric_limits<type>::min())
#define FMAX(type) double(std::numeric_limits<type>::max())

#define U(n, f, name) typedef UOU_fixed_point<n, f>::type name
#define S(n, f, name) typedef SOU_fixed_point<n, f>::type name

#ifdef LOGGING
#define INIT_LOGGER(path) \
    std::fstream stat; \
    stat.open(path, std::fstream::out | std::fstream::app);
#define LOG(x) \
    stat \
        << std::fixed \
        << std::setprecision(32u) \
        << x \
        << std::endl;
#define CLOSE_LOGGER() \
    stat.close();
#else
#define INIT_LOGGER(path)
#define LOG(x)
#define CLOSE_LOGGER()
#endif

    BOOST_AUTO_TEST_CASE(sum)
    {
#define PRECISION_TEST(t, error, path) \
{ \
    INIT_LOGGER(path); \
    BOOST_FOREACH(size_t it, boost::irange<size_t>(0, ITERATIONS, 1)) { \
    /* shortening range not to take care about rounding errors at the bounds */ \
        double u1 = r(FMIN(t) + 1u, FMAX(t) - 1u); \
        double u2 = r(FMIN(t) + 1u, FMAX(t) - 1u); \
        \
        t const a(u1); t const b(u2); \
        try { \
            double const abs_diff(std::fabs(double(a + b) - (u1 + u2))); \
            LOG(abs_diff); \
            if (abs_diff > 2 * error) { \
                std::stringstream message_stream; \
                message_stream \
                    << std::setprecision(5) \
                    << u1 << " + " << u2 \
                    << ": summation of Q" \
                    << t::total - t::fractionals << "." << t::fractionals \
                    << " was made with unexpected rounding error"; \
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
    } \
    CLOSE_LOGGER(); \
}

        U(28,13,t1); PRECISION_TEST(t1, 1E-2, "./plus_Q15_13"); // floor(log(10,2^13-1))-1=2
        S(56,34,t2); PRECISION_TEST(t2, 1E-9, "./plus_Q22_34"); // floor(log(10,2^34-1))-1=9
        S(61,52,t3); PRECISION_TEST(t3, 1E-14, "./plus_Q9_52"); // floor(log(10,2^52-1))-1=14
        U(53,23,t4); PRECISION_TEST(t4, 1E-5, "./plus_Q30_23"); // floor(log(10, 2^22-1))-1=5

#undef PRECISION_TEST
    }

    BOOST_AUTO_TEST_CASE(subtraction)
    {
#define PRECISION_TEST(t, error, path) \
{ \
    INIT_LOGGER(path); \
    BOOST_FOREACH(size_t it, boost::irange<size_t>(0, ITERATIONS, 1)) { \
    /* shortening range not to take care about rounding errors at the bounds */ \
        double u1 = r(FMIN(t) + 1u, FMAX(t) - 1u); \
        double u2 = r(FMIN(t) + 1u, FMAX(t) - 1u); \
        \
        t const a(u1); t const b(u2); \
        try { \
            double const abs_diff = std::fabs(double(a - b) - (u1 - u2)); \
            LOG(abs_diff); \
            if (abs_diff > 2 * error) { \
                std::stringstream message_stream; \
                message_stream \
                    << std::setprecision(5) \
                    << u1 << " - " << u2 \
                    << ": subtraction of Q" << t::total - t::fractionals << "." << t::fractionals \
                    << " was made with unexpected rounding error"; \
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
    } \
    CLOSE_LOGGER(); \
}

        S(28,13,t1); PRECISION_TEST(t1, 1E-2, "./subtraction_Q15_13"); // floor(log(10,2^13-1))-1=2
        S(56,34,t2); PRECISION_TEST(t2, 1E-9, "./subtraction_Q22_34"); // floor(log(10,2^34-1))-1=9
        S(61,52,t3); PRECISION_TEST(t3, 1E-14, "./subtraction_Q9_52"); // floor(log(10,2^52-1))-1=14
        S(53,23,t4); PRECISION_TEST(t4, 1E-5, "./subtraction_Q30_23"); // floor(log(10, 2^22-1))-1=5

#undef PRECISION_TEST
    }

    BOOST_AUTO_TEST_CASE(multiplication)
    {
#define PRECISION_TEST(t1, t2, error, path) \
{ \
    INIT_LOGGER(path); \
    BOOST_FOREACH(size_t it, boost::irange<size_t>(0, ITERATIONS, 1)) { \
        double u1 = r(FMIN(t1), FMAX(t1)); \
        double u2 = r(FMIN(t2), FMAX(t2)); \
        \
        try { \
            t1 const a(u1); t2 const b(u2); \
            double const abs_diff = std::fabs(double(a * b) - (u1 * u2)); \
            LOG(abs_diff); \
            if (abs_diff > error * (std::fabs(u1) + std::fabs(u2))) { \
                std::stringstream message_stream; \
                message_stream \
                    << std::setprecision(5) \
                    << u1 << " * " << u2 \
                    << ": product of Q" << t1::total - t1::fractionals << "." << t1::fractionals \
                    << " and Q" << t2::total - t2::fractionals << "." << t2::fractionals \
                    << " was made with unexpected rounding error"; \
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
    } \
    CLOSE_LOGGER(); \
}

        U(18,13,t1); U(20,15,t2); PRECISION_TEST(t1,t2, 1E-2, "./product_Q5_13_Q5_15"); // floor(log(10,2^13-1))-1=2
        S(24,23,t3); S(30,22,t4); PRECISION_TEST(t3,t4, 1E-5, "./product_Q1_23_Q8_22"); // floor(log(10,2^22-1))-1=5
        S(30,29,t5); S(33,33,t6); PRECISION_TEST(t5,t6, 1E-7, "./product_Q1_29_Q0_33"); // floor(log(10,2^29-1))-1=7

        PRECISION_TEST(t1,t1, 1E-2, "./product_Q5_13");
        PRECISION_TEST(t2,t2, 1E-3, "./product_Q5_15");
        PRECISION_TEST(t3,t3, 1E-5, "./product_Q1_23");
        PRECISION_TEST(t4,t4, 1E-5, "./product_Q8_22");
        PRECISION_TEST(t5,t5, 1E-7, "./product_Q1_29");

#undef PRECISION_TEST
    }

    BOOST_AUTO_TEST_CASE(division)
    {
#define PRECISION_TEST(t1, t2, error, path) \
{ \
    INIT_LOGGER(path); \
    BOOST_FOREACH(size_t it, boost::irange<size_t>(0, ITERATIONS, 1)) { \
        double u1 = r(FMIN(t1), FMAX(t1)); \
        double u2 = r(FMIN(t2), FMAX(t2)); \
        if (u2 == 0) { \
            continue; \
        } \
        \
        try { \
            t1 const a(u1); t2 const b(u2); \
            double const abs_diff = std::fabs(double(a / b) - (u1 / u2)); \
            LOG(abs_diff); \
            \
            double const u1a(std::fabs(u1)); double const u2a(std::fabs(u2)); \
            /* supremum of the error */ \
            if (abs_diff > error * ((u1a + u2a) / (u2a * std::fabs(u2a - error)))) { \
                std::stringstream message_stream; \
                message_stream \
                    << std::setprecision(5) \
                    << u1 << " / " << u2 \
                    << ": division of Q" << t1::total - t1::fractionals << "." << t1::fractionals \
                    << " by Q" << t2::total - t2::fractionals << "." << t2::fractionals \
                    << " was made with unexpected rounding error"; \
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
    } \
    CLOSE_LOGGER(); \
}

        U(18,13,t1); U(20,15,t2); PRECISION_TEST(t1,t2, 1E-2, "division_Q5_13_Q5_15"); // floor(log(10,2^13-1))-1=2
        S(24,23,t3); S(30,22,t4); PRECISION_TEST(t3,t4, 1E-5, "division_Q1_23_Q8_22"); // floor(log(10,2^22-1))-1=5
        S(30,29,t5); S(33,33,t6); PRECISION_TEST(t5,t6, 1E-7, "division_Q1_29_Q0_33"); // floor(log(10,2^22-1))-1=7

        PRECISION_TEST(t1,t1, 1E-2, "./division_Q5_13");
        PRECISION_TEST(t2,t2, 1E-3, "./division_Q5_15");
        PRECISION_TEST(t3,t3, 1E-5, "./division_Q1_23");
        PRECISION_TEST(t4,t4, 1E-5, "./division_Q8_22");
        PRECISION_TEST(t5,t5, 1E-7, "./division_Q1_29");

#undef PRECISION_TEST
    }

    BOOST_AUTO_TEST_CASE(log_sqrt)
    {
#define PRECISION_TEST(t, low, high, f, error) \
    BOOST_FOREACH(size_t i, boost::irange<size_t>(0, ITERATIONS, 1)) \
    { \
        double const u(r(low, high)); \
        t::##f##_type const x = std::##f##(t(u)); \
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

    // log
    U(23,17,t1); /*PRECISION_TEST(t1, 1.0, FMAX(t1), log, 1E-3);*/
    //U(32,16,t2); PRECISION_TEST(t2, 1.0, FMAX(t2), log, 1E-3);
    //S(43,20,t3); PRECISION_TEST(t3, 1E-2, FMAX(t3), log, 1E-4);
    //S(50,13,t4); PRECISION_TEST(t4, 1E-2, FMAX(t4), log, 1E-2);

    // sqrt
    //PRECISION_TEST(t1, FMIN(t1), FMAX(t1), sqrt, 1E-2);
    U(32,30,t2); PRECISION_TEST(t2, FMIN(t2), FMAX(t2), sqrt, 1E-4);
    U(40,20,t3); PRECISION_TEST(t3, FMIN(t3), FMAX(t3), sqrt, 1E-1);

#undef PRECISION_TEST
    }

    BOOST_AUTO_TEST_SUITE_END()
}}
