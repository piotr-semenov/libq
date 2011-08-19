#define BOOST_TEST_STATIC_LINK
#define BOOST_TEST_MODULE INTEGRALS_RULE_TESTS

#include <boost/test/unit_test.hpp>
#include <string>

#include "./../rules/integrals_rule.hpp"

namespace utils { namespace verbose_output { namespace unit_tests {
    namespace {
        using utils::verbose_output::integrals_rule;
    }

    BOOST_AUTO_TEST_SUITE(IntegralsRule)

    // idea of tests 'raiseEventCheck1', 'raiseEventCheck2' and 'raiseEventCheck3':
    // 1. we choose some integral observable x
    // 2. we assign stride to some value
    // 3. we manipulate with x state and check if event is raised
    BOOST_AUTO_TEST_CASE(raiseEventCheck1)
    {
        size_t const ob = 0;

        integrals_rule<size_t> rule(ob, 50);
        BOOST_CHECK_MESSAGE(rule(ob + 49) == false, "event was misoperated");
        BOOST_CHECK_MESSAGE(rule(ob + 56) == true, "event was failed");
    }

    BOOST_AUTO_TEST_CASE(raiseEventCheck2)
    {
        double const ob = -10.23;

        integrals_rule<double> rule(ob, -0.01);
        BOOST_CHECK_MESSAGE(rule(ob - 0.001) == false, "event was misoperated");
        BOOST_CHECK_MESSAGE(rule(ob - 0.02) == true, "event was failed");
    }

    BOOST_AUTO_TEST_CASE(raiseEventCheck3)
    {
        int const ob = -203;

        integrals_rule<int> rule(ob, -20);
        BOOST_CHECK_MESSAGE(rule(ob + 19) == false, "event was misoperated");
        BOOST_CHECK_MESSAGE(rule(ob - 100) == true, "event was failed");
    }

    BOOST_AUTO_TEST_SUITE_END()
}}}
