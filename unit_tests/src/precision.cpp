#define BOOST_TEST_STATIC_LINK

#include "boost/test/unit_test.hpp"
#include "boost/random.hpp"

#include "boost/log/trivial.hpp"
#include "boost/log/utility/setup/file.hpp"
#include "boost/log/utility/setup/common_attributes.hpp"
#include "boost/log/sources/severity_logger.hpp"

#include "boost/noncopyable.hpp"

#include "fixed_point.hpp"

#include <ctime>

#include <iomanip>
#include <fstream>
#include <sstream>

#include <stdexcept>

namespace libq {
namespace unit_tests {

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

class logger
    :    private boost::noncopyable,
         public src::severity_logger<logging::trivial::severity_level>
{
    bool m_is_initialized;

    logger()
        : m_is_initialized(false){}

public:
    static void init(void)
    {
        logging::add_file_log(
            keywords::file_name = "precision.log",
            keywords::format = "[%TimeStamp%]: %Message%"
        );
        logging::core::get()->set_filter(
            logging::trivial::severity >= logging::trivial::info
        );
        logging::add_common_attributes();
    }

    static logger& instance()
    {
        static logger precision_logger;
        if (!precision_logger.m_is_initialized) {
            init();

            precision_logger.m_is_initialized = true;
        }

        return precision_logger;
    }
};

/*!
 \brief gets the random uniform number that is within
 the dynamic range of specified fixed-point format
*/
template<typename Q>
double r(void)
{
    static boost::mt19937 gen(std::size_t(std::time(0)));
    static boost::uniform_01<boost::mt19937> uniform(gen);

    double const low = static_cast<double>(std::numeric_limits<Q>::min());
    double const high = static_cast<double>(std::numeric_limits<Q>::max());

    return low + uniform() * (high - low);
}

BOOST_AUTO_TEST_SUITE(Precision)

#define ITERATIONS 100
#define Q(n,f) libq::Q<n, f>
#define UQ(n, f) libq::UQ<n, f>
#define TEST_THE_PRECISION_OF2(Q1, Q2, OP, error_limit) \
    for (std::size_t it = 0; it != ITERATIONS; ++it) { \
        double const u1 = r<Q1>(); \
        double const u2 = r<Q2>(); \
        if (Q2(u2).value() == 0) { \
            it--; continue; \
        } \
\
        Q1 const a(u1); Q2 const b(u2); \
        try { \
            double const abs_diff = \
                std::fabs( \
                    static_cast<double>(OP(a, b)) - (OP(u1, u2)) \
                ); \
\
            std::stringstream stream; \
            stream \
                << std::setprecision(20) \
                << "Q" << Q1::number_of_significant_bits << "." << Q1::bits_for_fractional << "\t" \
                << "Q" << Q2::number_of_significant_bits << "." << Q2::bits_for_fractional << "\t" \
                << #OP << "(" << u1 << ", " << u2 << ")" << "\t" \
                << abs_diff; \
            std::string const message = stream.str(); \
            BOOST_LOG_SEV(logger::instance(), logging::trivial::info) << "[Info]\t" << message; \
\
            if (abs_diff > error_limit(u1, u2, static_cast<double>(a), static_cast<double>(b))) { \
                BOOST_LOG_SEV(logger::instance(), logging::trivial::error) << "[Error]\t" << message; \
                BOOST_CHECK_MESSAGE(false, message); \
            } \
        } \
        catch (std::exception) { \
            BOOST_CHECK_MESSAGE(false, ""); \
        } \
    }

#define TEST_THE_PRECISION_OF1(Q, OP, _error_limit) TEST_THE_PRECISION_OF2(Q, Q, OP, _error_limit)


BOOST_AUTO_TEST_CASE(arithmetics)
{
/*
 note, the summation max error can be determined as shown below:
 (x + e_x) + (y + e_y) = (x + y) + (e_x + e_y)

 note, error = error(x, y, a, b), where
 * x, y are the referenced real numbers
 * a = x + e_x, b = y + e_y are their approximations by fixed-point numbers (as real numbers)
*/
#define ERROR(precision) [](double, double, double, double){ return 2 * precision; }
#define PLUS(a, b) a+b
    TEST_THE_PRECISION_OF1(UQ(23, 13), PLUS, ERROR(1E-3)); // floor(log(10,2^13-1))=3
    TEST_THE_PRECISION_OF1(Q(56, 34), PLUS, ERROR(1E-10));
    TEST_THE_PRECISION_OF1(Q(61, 52), PLUS, ERROR(1E-15));
    TEST_THE_PRECISION_OF1(UQ(53, 23), PLUS, ERROR(1E-6));
#undef PLUS

/*
 note, the subtraction max error is the same as the summation has
*/
#define SUBTRACT(a, b) a-b
    TEST_THE_PRECISION_OF1(Q(28, 13), SUBTRACT, ERROR(1E-3));
    TEST_THE_PRECISION_OF1(Q(56, 34), SUBTRACT, ERROR(1E-10));
    TEST_THE_PRECISION_OF1(Q(61, 52), SUBTRACT, ERROR(1E-15));
    TEST_THE_PRECISION_OF1(Q(53, 23), SUBTRACT, ERROR(1E-6));
#undef SUBTRACT
#undef ERROR

/*
 note, the multiplication max error can be determined as shown below:
 (x + e_x) * (y + e_y) = (x * y) + (|x| * e_y + |y| * e_x) + o(e_x, e_y)
*/
#define ERROR(precision1, precision2) [](double x, double y, double a, double b) \
{ \
    return (std::fabs(x) * precision1) + (std::fabs(y) * precision2); \
}
#define MULTIPLY(a, b) a*b
    TEST_THE_PRECISION_OF2(UQ(18, 13), UQ(20, 15), MULTIPLY, ERROR(1E-3, 1E-4));
    TEST_THE_PRECISION_OF2(Q(24, 23), Q(30, 22), MULTIPLY, ERROR(1E-6, 1E-6));
    TEST_THE_PRECISION_OF2(Q(30, 29), Q(33, 33), MULTIPLY, ERROR(1E-8, 1E-9));

    TEST_THE_PRECISION_OF1(UQ(18, 13), MULTIPLY, ERROR(1E-3, 1E-3));
    TEST_THE_PRECISION_OF1(UQ(20, 15), MULTIPLY, ERROR(1E-4, 1E-4));
    TEST_THE_PRECISION_OF1(Q(24, 23), MULTIPLY, ERROR(1E-6, 1E-6));
    TEST_THE_PRECISION_OF1(Q(30, 22), MULTIPLY, ERROR(1E-6, 1E-6));
    TEST_THE_PRECISION_OF1(Q(30, 29), MULTIPLY, ERROR(1E-8, 1E-8));
#undef MULTIPLY
#undef ERROR

    /*
     note, the division max error can be determined as shown below:
     (x + e_x) / (y + e_y) = c = (x/y + e_x/y) / (1 + e_y/y)
     c = x/y + (e_x + |c|*e_y)/|y|
     */
#define ERROR(precision1, precision2) [](double x, double y, double a, double b) \
{ \
    return (precision1 + std::fabs(a / b) * precision2) / std::fabs(y); \
}
#define DIVISION(a, b) a/b
    TEST_THE_PRECISION_OF2(UQ(18, 13), UQ(20, 15), DIVISION, ERROR(1E-3, 1E-4));
    TEST_THE_PRECISION_OF2(Q(24, 23), Q(30, 22), DIVISION, ERROR(1E-6, 1E-6));
    TEST_THE_PRECISION_OF2(Q(29, 29), Q(33, 33), DIVISION, ERROR(1E-8, 1E-9));
    TEST_THE_PRECISION_OF2(Q(51, 52), Q(5, 4), DIVISION, ERROR(1E-15, 1E-1));

    TEST_THE_PRECISION_OF1(UQ(18, 13), DIVISION, ERROR(1E-3, 1E-3));
    TEST_THE_PRECISION_OF1(UQ(20, 15), DIVISION, ERROR(1E-4, 1E-4));
    TEST_THE_PRECISION_OF1(Q(24, 23), DIVISION, ERROR(1E-6, 1E-6));
    TEST_THE_PRECISION_OF1(Q(30, 22), DIVISION, ERROR(1E-6, 1E-6));
    TEST_THE_PRECISION_OF1(Q(30, 29), DIVISION, ERROR(1E-8, 1E-8));
#undef ERROR
#undef DIVISION
}

BOOST_AUTO_TEST_CASE(std_functions)
{
#define ERROR(limit) [](double, double, double, double){ return limit; }
#define LOG(a, b) std::log(a)
    TEST_THE_PRECISION_OF1(UQ(23, 17), LOG, ERROR(1E-2));
    TEST_THE_PRECISION_OF1(UQ(32, 16), LOG, ERROR(1E-3));
    TEST_THE_PRECISION_OF1(UQ(43, 20), LOG, ERROR(1E-2));
    TEST_THE_PRECISION_OF1(UQ(50, 13), LOG, ERROR(1E-2));
#undef LOG

#define SQRT(a, b) std::sqrt(a)
    TEST_THE_PRECISION_OF1(UQ(23, 17), SQRT, ERROR(1E-2));
    TEST_THE_PRECISION_OF1(UQ(32, 30), SQRT, ERROR(1E-2));
    TEST_THE_PRECISION_OF1(UQ(40, 20), SQRT, ERROR(1));
    TEST_THE_PRECISION_OF1(UQ(40, 40), SQRT, ERROR(1E-5));
#undef SQRT

#define SIN(a, b) std::sin(a)
    TEST_THE_PRECISION_OF1(Q(25, 21), SIN, ERROR(1E-4));
    TEST_THE_PRECISION_OF1(Q(44, 40), SIN, ERROR(1E-7));
    TEST_THE_PRECISION_OF1(Q(20, 16), SIN, ERROR(1E-3));
    TEST_THE_PRECISION_OF1(Q(63, 60), SIN, ERROR(1E-13));
#undef SIN

#define COS(a, b) std::cos(a)
    TEST_THE_PRECISION_OF1(Q(25, 21), COS, ERROR(1E-4));
    TEST_THE_PRECISION_OF1(Q(44, 40), COS, ERROR(1E-7));
    TEST_THE_PRECISION_OF1(Q(20, 16), COS, ERROR(1E-3));
    TEST_THE_PRECISION_OF1(Q(63, 60), COS, ERROR(1E-13));
#undef COS

#define TAN(a, b) std::tan(a)
    TEST_THE_PRECISION_OF1(Q(25, 21), TAN, ERROR(1E-4));
    TEST_THE_PRECISION_OF1(Q(44, 40), TAN, ERROR(1E-7));
    TEST_THE_PRECISION_OF1(Q(20, 16), TAN, ERROR(1E-3));
    TEST_THE_PRECISION_OF1(Q(63, 60), TAN, ERROR(1E-13));
#undef TAN

#define ASIN(a, b) std::asin(a)
    TEST_THE_PRECISION_OF1(Q(23, 21), ASIN, ERROR(1E-4));
    TEST_THE_PRECISION_OF1(Q(17, 16), ASIN, ERROR(1E-2));
    TEST_THE_PRECISION_OF1(Q(31, 30), ASIN, ERROR(1E-6));
    TEST_THE_PRECISION_OF1(Q(63, 60), ASIN, ERROR(1E-13));
#undef ASIN


#undef ERROR
}
//
//        PRECISION_TEST(t1, -0.999, 0.999, acos, 1E-4, "./acos_Q4_21.dat");
//        PRECISION_TEST(t2, -0.999, 0.999, acos, 1E-2, "./acos_Q4_16.dat");
//        PRECISION_TEST(t3, -0.999, 0.999, acos, 1E-6, "./acos_Q1_30.dat");
//
//        PRECISION_TEST(t1, -0.999, 0.999, atan, 1E-5, "./atan_Q4_21.dat");
//        PRECISION_TEST(t2, -0.999, 0.999, atan, 1E-3, "./atan_Q4_16.dat");
//        PRECISION_TEST(t3, -0.999, 0.999, atan, 1E-7, "./atan_Q1_30.dat");
//    }
//
//    BOOST_AUTO_TEST_CASE(exp)
//    {
//        S(22,21,t1); PRECISION_TEST(t1, FMIN(t1), FMAX(t1), exp, 1E-4, "./exp_Q4_21.dat");
//        S(17,16,t2); PRECISION_TEST(t2, FMIN(t2), FMAX(t2), exp, 1E-3, "./exp_Q4_16.dat");
//        S(31,30,t3); PRECISION_TEST(t3, FMIN(t3), FMAX(t3), exp, 1E-7, "./exp_Q1_30.dat");
//    }
//
//    BOOST_AUTO_TEST_CASE(sinh)
//    {
//        S(23,21,t1); PRECISION_TEST(t1, FMIN(t1), FMAX(t1), sinh, 1E-4, "./sinh_Q4_21.dat");
//        S(31,16,t2); PRECISION_TEST(t2, -3, 3u, sinh, 1E-3, "./sinh_Q15_16.dat");
//        S(31,30,t3); PRECISION_TEST(t3, FMIN(t3), FMAX(t3), sinh, 1E-7, "./sinh_Q3_60.dat");
//    }
//
//    BOOST_AUTO_TEST_CASE(cosh)
//    {
//        S(23,21,t1); PRECISION_TEST(t1, FMIN(t1), FMAX(t1), cosh, 1E-4, "./cosh_Q4_21.dat");
//        S(31,16,t2); PRECISION_TEST(t2, -3, 3u, cosh, 1E-3, "./cosh_Q15_16.dat");
//        S(31,30,t3); PRECISION_TEST(t3, FMIN(t3), FMAX(t3), cosh, 1E-7, "./cosh_Q3_60.dat");
//    }
//
//    //BOOST_AUTO_TEST_CASE(asinh)
//    //{
//    //    S(23,21,t1); PRECISION_TEST(t1, -2.0, 2.0, asinh, 1E-1, "./asinh_Q4_21.dat");
//    //    S(31,16,t2); PRECISION_TEST(t2, -5.0, 5.0, asinh, 1E-1, "./asinh_Q15_16.dat");
//    //    S(31,25,t3); PRECISION_TEST(t3, -7.0, 7.0, asinh, 1E-1, "./asinh_Q1_30.dat"); //!
//    //}
//
//    //BOOST_AUTO_TEST_CASE(acosh)
//    //{
//    //    S(23,21,t1); PRECISION_TEST(t1, 1.0, FMAX(t1), acosh, 1E-1, "./acosh_Q4_21.dat");
//    //    S(31,16,t2);/* PRECISION_TEST(t2, 1.0, FMAX(t2), acosh, 1E-1, "./acosh_Q15_16.dat");*/
//    //    std::acosh(t2(13333.7)); //!
//    //}
//
//    //BOOST_AUTO_TEST_CASE(atanh)
//    //{
//    //    S(23,21,t1); PRECISION_TEST(t1, -0.9999, 0.9999, atanh, 1E-1, "./atanh_Q4_21.dat");
//    //    S(31,16,t2); PRECISION_TEST(t2, -0.9999, 0.9999, atanh, 1E-1, "./atanh_Q15_16.dat");
//    //}
//
//    //BOOST_AUTO_TEST_CASE(tanh)
//    //{
//    //    S(23,21,t1);/* PRECISION_TEST(t1, FMIN(t1), FMAX(t1), tanh, 1e-1, "./tanh_q4_21.dat");*/
//    //    std::tanh(t1(-3.90405)); // !
//    //    //S(31,16,t2); PRECISION_TEST(t2, FMIN(t2), FMAX(t2), tanh, 1e-1, "./tanh_q15_16.dat");
//    //}
//
//#undef PRECISION_TEST
//
    BOOST_AUTO_TEST_SUITE_END()
} // unit_tests
} // libq
