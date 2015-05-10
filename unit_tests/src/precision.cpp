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
    :    public src::severity_logger<logging::trivial::severity_level>
{
public:
    logger(std::string const& _log_filename)
    {
        logging::add_file_log(
            keywords::file_name = _log_filename,
            keywords::format = "[%TimeStamp%]: %Message%"
        );
        logging::core::get()->set_filter(
            logging::trivial::severity >= logging::trivial::info
        );
        logging::add_common_attributes();
    }
};

/*!
 \brief gets the random uniform number that is within
 the dynamic range of specified fixed-point format
*/
template<typename Q>
double uniform_distribution_sample(void)
{
    static boost::mt19937 gen(std::size_t(std::time(0)));
    static boost::uniform_01<boost::mt19937> uniform(gen);

    double const low = static_cast<double>(std::numeric_limits<Q>::min());
    double const high = static_cast<double>(std::numeric_limits<Q>::max());

    return low + uniform() * (high - low);
}

/*!
 \brief 
*/
template<typename Q_type1, typename Q_type2, typename Operation, typename Error, std::size_t iterations = 100u>
void test_the_precision_of(Operation _op, Error _limit, logger& _log)
{
    for (std::size_t it = 0; it != iterations; ++it) {
        double const u1 = uniform_distribution_sample<Q_type1>();
        double const u2 = uniform_distribution_sample<Q_type2>();

        if (Q_type2(u2).value() == 0) {
            it--;
            continue;
        }

        Q_type1 const a(u1);
        Q_type2 const b(u2);

        try {
            double const abs_diff = std::fabs(_op(a, b) - _op(u1, u2));

            std::stringstream stream;
            stream
                << std::setprecision(20)
                << "Q" << Q_type1::number_of_significant_bits << "." << Q_type1::bits_for_fractional << "\t"
                << "Q" << Q_type2::number_of_significant_bits << "." << Q_type2::bits_for_fractional << "\t"
                << u1 << "\t" << u2 << "\t"
                << abs_diff;
            std::string const message = stream.str();
            BOOST_LOG_SEV(_log, logging::trivial::info) << "[Info]\t" << message;

            if (abs_diff > _limit(u1, u2, static_cast<double>(a), static_cast<double>(b))) {
                BOOST_LOG_SEV(_log, logging::trivial::error) << "[Error]\t" << message;
                BOOST_CHECK_MESSAGE(false, message);
            }
        }
        catch (std::exception) {
            BOOST_CHECK_MESSAGE(false, "");
        }
    }
}
BOOST_AUTO_TEST_SUITE(Precision)

class plus
{
public:
    template<typename T1, typename T2>
    double operator()(T1 _x, T2 _y) const{ return static_cast<double>(_x + _y); }
};
BOOST_AUTO_TEST_CASE(precision_of_plus)
{
/*
 note, the summation max error can be determined as shown below:
 (x + e_x) + (y + e_y) = (x + y) + (e_x + e_y)

 note, error = error(x, y, a, b), where
 * x, y are the referenced real numbers
 * a = x + e_x, b = y + e_y are their approximations by fixed-point numbers (as real numbers)
*/
#define error(precision) [](double, double, double, double){ return 2 * precision; }
    using libq::Q;
    using libq::UQ;

    plus const op;
    logger log("plus.log");

    test_the_precision_of<UQ<23, 13>, UQ<23, 13> >(op, error(1E-3), log); // floor(log(10,2^13-1))=3
    test_the_precision_of<Q<56, 34>, Q<56, 34> >(op, error(1E-10), log);
    test_the_precision_of<Q<61, 52>, Q<61, 52> >(op, error(1E-15), log);
    test_the_precision_of<UQ<53, 23>, UQ<53, 23> >(op, error(1E-6), log);
#undef error
}

/*
 note, the subtraction max error is the same as the summation has
*/
class minus
{
public:
    template<typename T1, typename T2>
    double operator()(T1 _x, T2 _y) const{ return static_cast<double>(_x - _y); }
};
BOOST_AUTO_TEST_CASE(precision_of_minus)
{
#define error(precision) [](double, double, double, double){ return 2 * precision; }
    minus const op;
    logger log("minus.log");

    test_the_precision_of<Q<28, 13>, Q<28, 13> >(op, error(1E-3), log);
    test_the_precision_of<Q<56, 34>, Q<56, 34> >(op, error(1E-10), log);
    test_the_precision_of<Q<61, 52>, Q<61, 52> >(op, error(1E-15), log);
    test_the_precision_of<Q<53, 23>, Q<53, 23> >(op, error(1E-6), log);
#undef error
}
//
///*
// note, the multiplication max error can be determined as shown below:
// (x + e_x) * (y + e_y) = (x * y) + (|x| * e_y + |y| * e_x) + o(e_x, e_y)
//*/
//#define ERROR(precision1, precision2) [](double x, double y, double a, double b) \
//{ \
//    return (std::fabs(x) * precision1) + (std::fabs(y) * precision2); \
//}
//#define MULTIPLY(a, b) a*b
//    TEST_THE_PRECISION_OF2(UQ(18, 13), UQ(20, 15), MULTIPLY, ERROR(1E-3, 1E-4));
//    TEST_THE_PRECISION_OF2(Q(24, 23), Q(30, 22), MULTIPLY, ERROR(1E-6, 1E-6));
//    TEST_THE_PRECISION_OF2(Q(30, 29), Q(33, 33), MULTIPLY, ERROR(1E-8, 1E-9));
//
//    TEST_THE_PRECISION_OF1(UQ(18, 13), MULTIPLY, ERROR(1E-3, 1E-3));
//    TEST_THE_PRECISION_OF1(UQ(20, 15), MULTIPLY, ERROR(1E-4, 1E-4));
//    TEST_THE_PRECISION_OF1(Q(24, 23), MULTIPLY, ERROR(1E-6, 1E-6));
//    TEST_THE_PRECISION_OF1(Q(30, 22), MULTIPLY, ERROR(1E-6, 1E-6));
//    TEST_THE_PRECISION_OF1(Q(30, 29), MULTIPLY, ERROR(1E-8, 1E-8));
//#undef MULTIPLY
//#undef ERROR
//
//    /*
//     note, the division max error can be determined as shown below:
//     (x + e_x) / (y + e_y) = c = (x/y + e_x/y) / (1 + e_y/y)
//     c = x/y + (e_x + |c|*e_y)/|y|
//    */
//#define ERROR(precision1, precision2) [](double x, double y, double a, double b) \
//{ \
//    return (precision1 + std::fabs(a / b) * precision2) / std::fabs(y); \
//}
//#define DIVISION(a, b) a/b
//    TEST_THE_PRECISION_OF2(UQ(18, 13), UQ(20, 15), DIVISION, ERROR(1E-3, 1E-4));
//    TEST_THE_PRECISION_OF2(Q(24, 23), Q(30, 22), DIVISION, ERROR(1E-6, 1E-6));
//    TEST_THE_PRECISION_OF2(Q(29, 29), Q(33, 33), DIVISION, ERROR(1E-8, 1E-9));
//    TEST_THE_PRECISION_OF2(QExtended(52, 52, 1), Q(5, 4), DIVISION, ERROR(1E-15, 1E-1));
//
//    TEST_THE_PRECISION_OF1(UQ(18, 13), DIVISION, ERROR(1E-3, 1E-3));
//    TEST_THE_PRECISION_OF1(UQ(20, 15), DIVISION, ERROR(1E-4, 1E-4));
//    TEST_THE_PRECISION_OF1(Q(24, 23), DIVISION, ERROR(1E-6, 1E-6));
//    TEST_THE_PRECISION_OF1(Q(30, 22), DIVISION, ERROR(1E-6, 1E-6));
//    TEST_THE_PRECISION_OF1(Q(30, 29), DIVISION, ERROR(1E-8, 1E-8));
//#undef ERROR
//#undef DIVISION

//BOOST_AUTO_TEST_CASE(std_functions)
//{
//#define ERROR(limit) [](double, double, double, double){ return limit; }
//#define LOG(a, b) std::log(a)
//    TEST_THE_PRECISION_OF1(UQ(23, 17), LOG, ERROR(1E-2));
//    TEST_THE_PRECISION_OF1(UQ(32, 16), LOG, ERROR(1E-3));
//    TEST_THE_PRECISION_OF1(UQ(43, 20), LOG, ERROR(1E-2));
//    TEST_THE_PRECISION_OF1(UQ(50, 13), LOG, ERROR(1E-2));
//#undef LOG
//
//#define SQRT(a, b) std::sqrt(a)
//    TEST_THE_PRECISION_OF1(UQ(23, 17), SQRT, ERROR(1E-2));
//    TEST_THE_PRECISION_OF1(UQ(32, 30), SQRT, ERROR(1E-2));
//    TEST_THE_PRECISION_OF1(UQ(40, 20), SQRT, ERROR(1));
//    TEST_THE_PRECISION_OF1(UQ(40, 40), SQRT, ERROR(1E-5));
//#undef SQRT
//
//#define SIN(a, b) std::sin(a)
//    TEST_THE_PRECISION_OF1(Q(25, 21), SIN, ERROR(1E-4));
//    TEST_THE_PRECISION_OF1(Q(44, 40), SIN, ERROR(1E-7));
//    TEST_THE_PRECISION_OF1(Q(20, 16), SIN, ERROR(1E-3));
//    TEST_THE_PRECISION_OF1(Q(63, 60), SIN, ERROR(1E-13));
//#undef SIN
//
//#define COS(a, b) std::cos(a)
//    TEST_THE_PRECISION_OF1(Q(25, 21), COS, ERROR(1E-4));
//    TEST_THE_PRECISION_OF1(Q(44, 40), COS, ERROR(1E-7));
//    TEST_THE_PRECISION_OF1(Q(20, 16), COS, ERROR(1E-3));
//    TEST_THE_PRECISION_OF1(Q(63, 60), COS, ERROR(1E-13));
//#undef COS
//
//#define TAN(a, b) std::tan(a)
//    TEST_THE_PRECISION_OF1(Q(25, 21), TAN, ERROR(1E-4));
//    TEST_THE_PRECISION_OF1(Q(44, 40), TAN, ERROR(1E-7));
//    TEST_THE_PRECISION_OF1(Q(20, 16), TAN, ERROR(1E-3));
//    TEST_THE_PRECISION_OF1(Q(63, 60), TAN, ERROR(1E-13));
//#undef TAN
//
//#define ASIN(a, b) std::asin(a)
//    TEST_THE_PRECISION_OF1(Q(23, 21), ASIN, ERROR(1E-4));
//    TEST_THE_PRECISION_OF1(Q(17, 16), ASIN, ERROR(1E-2));
//    TEST_THE_PRECISION_OF1(Q(31, 30), ASIN, ERROR(1E-6));
//    TEST_THE_PRECISION_OF1(Q(63, 60), ASIN, ERROR(1E-13));
//#undef ASIN
//
//#undef ERROR
//}
////
////        PRECISION_TEST(t1, -0.999, 0.999, acos, 1E-4, "./acos_Q4_21.dat");
////        PRECISION_TEST(t2, -0.999, 0.999, acos, 1E-2, "./acos_Q4_16.dat");
////        PRECISION_TEST(t3, -0.999, 0.999, acos, 1E-6, "./acos_Q1_30.dat");
////
////        PRECISION_TEST(t1, -0.999, 0.999, atan, 1E-5, "./atan_Q4_21.dat");
////        PRECISION_TEST(t2, -0.999, 0.999, atan, 1E-3, "./atan_Q4_16.dat");
////        PRECISION_TEST(t3, -0.999, 0.999, atan, 1E-7, "./atan_Q1_30.dat");
////    }
////
////    BOOST_AUTO_TEST_CASE(exp)
////    {
////        S(22,21,t1); PRECISION_TEST(t1, FMIN(t1), FMAX(t1), exp, 1E-4, "./exp_Q4_21.dat");
////        S(17,16,t2); PRECISION_TEST(t2, FMIN(t2), FMAX(t2), exp, 1E-3, "./exp_Q4_16.dat");
////        S(31,30,t3); PRECISION_TEST(t3, FMIN(t3), FMAX(t3), exp, 1E-7, "./exp_Q1_30.dat");
////    }
////
////    BOOST_AUTO_TEST_CASE(sinh)
////    {
////        S(23,21,t1); PRECISION_TEST(t1, FMIN(t1), FMAX(t1), sinh, 1E-4, "./sinh_Q4_21.dat");
////        S(31,16,t2); PRECISION_TEST(t2, -3, 3u, sinh, 1E-3, "./sinh_Q15_16.dat");
////        S(31,30,t3); PRECISION_TEST(t3, FMIN(t3), FMAX(t3), sinh, 1E-7, "./sinh_Q3_60.dat");
////    }
////
////    BOOST_AUTO_TEST_CASE(cosh)
////    {
////        S(23,21,t1); PRECISION_TEST(t1, FMIN(t1), FMAX(t1), cosh, 1E-4, "./cosh_Q4_21.dat");
////        S(31,16,t2); PRECISION_TEST(t2, -3, 3u, cosh, 1E-3, "./cosh_Q15_16.dat");
////        S(31,30,t3); PRECISION_TEST(t3, FMIN(t3), FMAX(t3), cosh, 1E-7, "./cosh_Q3_60.dat");
////    }
////
////    //BOOST_AUTO_TEST_CASE(asinh)
////    //{
////    //    S(23,21,t1); PRECISION_TEST(t1, -2.0, 2.0, asinh, 1E-1, "./asinh_Q4_21.dat");
////    //    S(31,16,t2); PRECISION_TEST(t2, -5.0, 5.0, asinh, 1E-1, "./asinh_Q15_16.dat");
////    //    S(31,25,t3); PRECISION_TEST(t3, -7.0, 7.0, asinh, 1E-1, "./asinh_Q1_30.dat"); //!
////    //}
////
////    //BOOST_AUTO_TEST_CASE(acosh)
////    //{
////    //    S(23,21,t1); PRECISION_TEST(t1, 1.0, FMAX(t1), acosh, 1E-1, "./acosh_Q4_21.dat");
////    //    S(31,16,t2);/* PRECISION_TEST(t2, 1.0, FMAX(t2), acosh, 1E-1, "./acosh_Q15_16.dat");*/
////    //    std::acosh(t2(13333.7)); //!
////    //}
////
////    //BOOST_AUTO_TEST_CASE(atanh)
////    //{
////    //    S(23,21,t1); PRECISION_TEST(t1, -0.9999, 0.9999, atanh, 1E-1, "./atanh_Q4_21.dat");
////    //    S(31,16,t2); PRECISION_TEST(t2, -0.9999, 0.9999, atanh, 1E-1, "./atanh_Q15_16.dat");
////    //}
////
////    //BOOST_AUTO_TEST_CASE(tanh)
////    //{
////    //    S(23,21,t1);/* PRECISION_TEST(t1, FMIN(t1), FMAX(t1), tanh, 1e-1, "./tanh_q4_21.dat");*/
////    //    std::tanh(t1(-3.90405)); // !
////    //    //S(31,16,t2); PRECISION_TEST(t2, FMIN(t2), FMAX(t2), tanh, 1e-1, "./tanh_q15_16.dat");
////    //}
////
////#undef PRECISION_TEST
////
    BOOST_AUTO_TEST_SUITE_END()
} // unit_tests
} // libq
