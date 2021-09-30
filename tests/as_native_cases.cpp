#define BOOST_TEST_MODULE as_native_cases
#include "boost/test/included/unit_test.hpp"

#include "libq/fixed_point.hpp"

namespace libq {
namespace unit_tests {
BOOST_AUTO_TEST_SUITE(lift)

BOOST_AUTO_TEST_CASE(as_native)
{
    libq::Q<28, 20> a(-2.302);
    libq::lift(a) += 1u << 20;
    BOOST_CHECK_MESSAGE(std::fabs(double(a) + 1.302) < 1E-4,
                        "[lift] operator '+=' has a bug");

    auto const x = libq::lift(a) + 23;
    auto const y = 23 + libq::lift(a);
    BOOST_CHECK_MESSAGE(x == -1365223 && y == -1365223,
                        "[lift] operator '+' has a bug"
                            << " x=" << x << " y=" << y);

    libq::Q<28, 2> b(1000123);
    libq::lift(b) /= 29;
    BOOST_CHECK_MESSAGE(~uint32_t(libq::lift(b)) == UINT32_C(4294829347),
                        "[lift] operator ~ has a bug");
    BOOST_CHECK_MESSAGE(std::size_t(double(b)) == 34487,
                        "[lift] operator /= has a bug");
}

BOOST_AUTO_TEST_SUITE_END
()

}  // namespace unit_tests
}  // namespace libq
