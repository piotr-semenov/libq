#define BOOST_TEST_STATIC_LINK

#include <boost/test/unit_test.hpp>
#include <boost/random.hpp>

#include "fixed_point.hpp"

#include <ctime>

#include <iomanip>
#include <fstream>
#include <sstream>

#include <exception>

namespace std {
    double asinh(double val)
    {
        double const arg(val + std::sqrt(val * val + 1));
        return std::log(arg);
    }

    double acosh(double val)
    {
        double const arg(val + std::sqrt(val * val - 1));
        return std::log(arg);
    }

    double atanh(double val)
    {
        return 0.5 * (std::log(1.0 + val) - std::log(1.0 - val));
    }
}

namespace core { namespace unit_tests {
    double r(double low, double high)
    {
        static boost::mt19937 gen(size_t(std::time(0)));
        static boost::uniform_01<boost::mt19937> uniform(gen);
        if (low > high) {
            throw std::exception("illegal range");
        }
        return low + uniform() * (high - low);
    }

    BOOST_AUTO_TEST_SUITE(Precision)

#define ITERATIONS size_t(1E+6)
#define FMIN(type) double(std::numeric_limits<type>::min())
#define FMAX(type) double(std::numeric_limits<type>::max())

#define U(n, f, name) typedef core::UOU_fixed_point<n, f>::type name
#define S(n, f, name) typedef core::SOU_fixed_point<n, f>::type name

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
        catch(std::exception) { \
            std::stringstream message_stream; \
            message_stream \
                << u1 << " " << u2; \
            BOOST_CHECK_MESSAGE(false, message_stream.str()); \
        } \
    } \
    CLOSE_LOGGER(); \
}

        U(28,13,t1); PRECISION_TEST(t1, 1E-2, "./plus_Q15_13.dat"); // floor(log(10,2^13-1))-1=2
        S(56,34,t2); PRECISION_TEST(t2, 1E-9, "./plus_Q22_34.dat"); // 9
        S(61,52,t3); PRECISION_TEST(t3, 1E-14, "./plus_Q9_52.dat"); // 14
        U(53,23,t4); PRECISION_TEST(t4, 1E-5, "./plus_Q30_23.dat"); // 5

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
        catch(std::exception) { \
            std::stringstream message_stream; \
            message_stream \
                << u1 << " " << u2; \
            BOOST_CHECK_MESSAGE(false, message_stream.str()); \
        } \
    } \
    CLOSE_LOGGER(); \
}

        S(28,13,t1); PRECISION_TEST(t1, 1E-2, "./subtraction_Q15_13.dat"); // 2
        S(56,34,t2); PRECISION_TEST(t2, 1E-9, "./subtraction_Q22_34.dat"); // 9
        S(61,52,t3); PRECISION_TEST(t3, 1E-14, "./subtraction_Q9_52.dat"); // 14
        S(53,23,t4); PRECISION_TEST(t4, 1E-5, "./subtraction_Q30_23.dat"); // 5

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
        catch(std::exception) { \
            std::stringstream message_stream; \
            message_stream \
                << u1 << " " << u2; \
            BOOST_CHECK_MESSAGE(false, message_stream.str()); \
        } \
    } \
    CLOSE_LOGGER(); \
}

        U(18,13,t1); U(20,15,t2); PRECISION_TEST(t1,t2, 1E-2, "./product_Q5_13_Q5_15.dat"); // 2
        S(24,23,t3); S(30,22,t4); PRECISION_TEST(t3,t4, 1E-5, "./product_Q1_23_Q8_22.dat"); // 5
        S(30,29,t5); S(33,33,t6); PRECISION_TEST(t5,t6, 1E-7, "./product_Q1_29_Q0_33.dat"); // 7

        PRECISION_TEST(t1,t1, 1E-2, "./product_Q5_13.dat");
        PRECISION_TEST(t2,t2, 1E-3, "./product_Q5_15.dat");
        PRECISION_TEST(t3,t3, 1E-5, "./product_Q1_23.dat");
        PRECISION_TEST(t4,t4, 1E-5, "./product_Q8_22.dat");
        PRECISION_TEST(t5,t5, 1E-7, "./product_Q1_29.dat");

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
        if (u2 == 0 || t2(u2).value() == 0) { \
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
        catch(std::exception) { \
            std::stringstream message_stream; \
            message_stream \
                << u1 << " " << u2; \
            BOOST_CHECK_MESSAGE(false, message_stream.str()); \
        } \
    } \
    CLOSE_LOGGER(); \
}

        U(18,13,t1); U(20,15,t2); PRECISION_TEST(t1,t2, 1E-2, "division_Q5_13_Q5_15.dat"); // 2
        S(24,23,t3); S(30,22,t4); PRECISION_TEST(t3,t4, 1E-5, "division_Q1_23_Q8_22.dat"); // 5
        S(30,29,t5); S(33,33,t6); PRECISION_TEST(t5,t6, 1E-7, "division_Q1_29_Q0_33.dat"); // 7

        PRECISION_TEST(t1,t1, 1E-2, "./division_Q5_13.dat");
        PRECISION_TEST(t2,t2, 1E-3, "./division_Q5_15.dat");
        PRECISION_TEST(t3,t3, 1E-5, "./division_Q1_23.dat");
        PRECISION_TEST(t4,t4, 1E-5, "./division_Q8_22.dat");
        PRECISION_TEST(t5,t5, 1E-7, "./division_Q1_29.dat");

#undef PRECISION_TEST
    }

#define PRECISION_TEST(t, low, high, f, error, path) \
{ \
    INIT_LOGGER(path); \
    BOOST_FOREACH(size_t i, boost::irange<size_t>(0, ITERATIONS, 1)) \
    { \
        double const u(r(low, high)); \
        try { \
            t::##f##_type const x = std::##f##(t(u)); \
            \
            double const abs_diff = std::fabs(double(x) - std::##f##(u)); \
            LOG(abs_diff); \
            if (abs_diff > error) { \
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
        } \
        catch(std::exception) { \
            std::stringstream message_stream; \
            message_stream \
                << "Q" << (t::total - t::fractionals) << "." << t::fractionals << " = " \
                << u << " raises an exception"; \
            BOOST_CHECK_MESSAGE(false, message_stream.str()); \
        } \
    } \
    CLOSE_LOGGER(); \
}

    BOOST_AUTO_TEST_CASE(log)
    {
        U(23,17,t1); PRECISION_TEST(t1, 1.0, FMAX(t1), log, 1E-2, "./log_Q6_17.dat");
        U(32,16,t2); PRECISION_TEST(t2, 1.0, FMAX(t2), log, 1E-3, "./log_Q16_16.dat");
        S(43,20,t3); PRECISION_TEST(t3, 1E-2, FMAX(t3), log, 1E-4, "./log_Q23_20.dat");
        S(50,13,t4); PRECISION_TEST(t4, 1E-2, FMAX(t4), log, 1E-2, "./log_Q37_13.dat");
    }

    BOOST_AUTO_TEST_CASE(sqrt)
    {
        U(23,17,t1); PRECISION_TEST(t1, FMIN(t1), FMAX(t1), sqrt, 1E-2, "./sqrt_Q6_17.dat");
        U(32,30,t2); PRECISION_TEST(t2, FMIN(t2), FMAX(t2), sqrt, 1E-4, "./sqrt_Q2_30.dat");
        U(40,20,t3); PRECISION_TEST(t3, FMIN(t3), FMAX(t3), sqrt, 1E-1, "./sqrt_Q20_20.dat");
        U(40,40,t4); PRECISION_TEST(t4, FMIN(t4), FMAX(t4), sqrt, 1E-5, "./sqrt_Q0_40.dat");
    }

    BOOST_AUTO_TEST_CASE(trig)
    {
        // not to take care about precision leaks in angle reducing
        S(25,21,t1); PRECISION_TEST(t1, FMIN(t1), FMAX(t1), sin, 1E-4, "./sin_Q4_21.dat");
        S(44,40,t2); PRECISION_TEST(t2, FMIN(t2), FMAX(t2), sin, 1E-7, "./sin_Q4_40.dat");
        S(20,16,t3); PRECISION_TEST(t3, FMIN(t3), FMAX(t3), sin, 1E-3, "./sin_Q4_16.dat");
        S(63,60,t4); PRECISION_TEST(t4, FMIN(t4), FMAX(t4), sin, 1E-13, "./sin_Q3_60.dat");

        PRECISION_TEST(t1, FMIN(t1), FMAX(t1), cos, 1E-4, "./cos_Q4_21.dat");
        PRECISION_TEST(t2, FMIN(t2), FMAX(t2), cos, 1E-7, "./cos_Q4_40.dat");
        PRECISION_TEST(t3, FMIN(t3), FMAX(t3), cos, 1E-3, "./cos_Q4_16.dat");
        PRECISION_TEST(t4, FMIN(t4), FMAX(t4), cos, 1E-13, "./cos_Q3_60.dat");

#ifdef LOGGING
        PRECISION_TEST(t1, FMIN(t1), FMAX(t1), tan, 1, "./tan_Q4_21.dat");
        PRECISION_TEST(t2, FMIN(t2), FMAX(t2), tan, 1, "./tan_Q4_40.dat");
        PRECISION_TEST(t3, FMIN(t3), FMAX(t3), tan, 1, "./tan_Q4_16.dat");
        PRECISION_TEST(t4, FMIN(t4), FMAX(t4), tan, 1, "./tan_Q3_60.dat");
#endif
    }

    BOOST_AUTO_TEST_CASE(inverse_trig)
    {
        S(23,21,t1); PRECISION_TEST(t1, -0.999, 0.999, asin, 1E-4, "./asin_Q4_21.dat");
        S(17,16,t2); PRECISION_TEST(t2, -0.999, 0.999, asin, 1E-2, "./asin_Q4_16.dat");
        S(31,30,t3); PRECISION_TEST(t3, -0.999, 0.999, asin, 1E-6, "./asin_Q1_30.dat");

        PRECISION_TEST(t1, -0.999, 0.999, acos, 1E-4, "./acos_Q4_21.dat");
        PRECISION_TEST(t2, -0.999, 0.999, acos, 1E-2, "./acos_Q4_16.dat");
        PRECISION_TEST(t3, -0.999, 0.999, acos, 1E-6, "./acos_Q1_30.dat");

        PRECISION_TEST(t1, -0.999, 0.999, atan, 1E-5, "./atan_Q4_21.dat");
        PRECISION_TEST(t2, -0.999, 0.999, atan, 1E-3, "./atan_Q4_16.dat");
        PRECISION_TEST(t3, -0.999, 0.999, atan, 1E-7, "./atan_Q1_30.dat");
    }

    BOOST_AUTO_TEST_CASE(exp)
    {
        S(22,21,t1); PRECISION_TEST(t1, FMIN(t1), FMAX(t1), exp, 1E-4, "./exp_Q4_21.dat");
        S(17,16,t2); PRECISION_TEST(t2, FMIN(t2), FMAX(t2), exp, 1E-3, "./exp_Q4_16.dat");
        S(31,30,t3); PRECISION_TEST(t3, FMIN(t3), FMAX(t3), exp, 1E-7, "./exp_Q1_30.dat");
    }

    BOOST_AUTO_TEST_CASE(sinh)
    {
        S(23,21,t1); PRECISION_TEST(t1, FMIN(t1), FMAX(t1), sinh, 1E-4, "./sinh_Q4_21.dat");
        S(31,16,t2); PRECISION_TEST(t2, -3, 3u, sinh, 1E-3, "./sinh_Q15_16.dat");
        S(31,30,t3); PRECISION_TEST(t3, FMIN(t3), FMAX(t3), sinh, 1E-7, "./sinh_Q3_60.dat");
    }

    BOOST_AUTO_TEST_CASE(cosh)
    {
        S(23,21,t1); PRECISION_TEST(t1, FMIN(t1), FMAX(t1), cosh, 1E-4, "./cosh_Q4_21.dat");
        S(31,16,t2); PRECISION_TEST(t2, -3, 3u, cosh, 1E-3, "./cosh_Q15_16.dat");
        S(31,30,t3); PRECISION_TEST(t3, FMIN(t3), FMAX(t3), cosh, 1E-7, "./cosh_Q3_60.dat");
    }

    //BOOST_AUTO_TEST_CASE(asinh)
    //{
    //    S(23,21,t1); PRECISION_TEST(t1, -2.0, 2.0, asinh, 1E-1, "./asinh_Q4_21.dat");
    //    S(31,16,t2); PRECISION_TEST(t2, -5.0, 5.0, asinh, 1E-1, "./asinh_Q15_16.dat");
    //    S(31,25,t3); PRECISION_TEST(t3, -7.0, 7.0, asinh, 1E-1, "./asinh_Q1_30.dat"); //!
    //}

    //BOOST_AUTO_TEST_CASE(acosh)
    //{
    //    S(23,21,t1); PRECISION_TEST(t1, 1.0, FMAX(t1), acosh, 1E-1, "./acosh_Q4_21.dat");
    //    S(31,16,t2);/* PRECISION_TEST(t2, 1.0, FMAX(t2), acosh, 1E-1, "./acosh_Q15_16.dat");*/
    //    std::acosh(t2(13333.7)); //!
    //}

    //BOOST_AUTO_TEST_CASE(atanh)
    //{
    //    S(23,21,t1); PRECISION_TEST(t1, -0.9999, 0.9999, atanh, 1E-1, "./atanh_Q4_21.dat");
    //    S(31,16,t2); PRECISION_TEST(t2, -0.9999, 0.9999, atanh, 1E-1, "./atanh_Q15_16.dat");
    //}

    //BOOST_AUTO_TEST_CASE(tanh)
    //{
    //    S(23,21,t1);/* PRECISION_TEST(t1, FMIN(t1), FMAX(t1), tanh, 1e-1, "./tanh_q4_21.dat");*/
    //    std::tanh(t1(-3.90405)); // !
    //    //S(31,16,t2); PRECISION_TEST(t2, FMIN(t2), FMAX(t2), tanh, 1e-1, "./tanh_q15_16.dat");
    //}

#undef PRECISION_TEST

    BOOST_AUTO_TEST_SUITE_END()
}}
