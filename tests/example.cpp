#include "libq/fixed_point.hpp"

#include <vector>
#include <numeric>
#include <iostream>

namespace {

namespace fixed_accuracy {

/// Assume, we know that all the samples are < 2.0
using value_type = libq::Q<31, 29>;

}  // namespace fixed_accuracy

namespace floating_accuracy {

using value_type = double;

}  // namespace floating_accuracy

template <typename Value_type>
Value_type
    mean(std::vector<Value_type> const &_samples)
{
    Value_type const n = Value_type(_samples.size());
    Value_type const acc =
        std::accumulate(_samples.cbegin(), _samples.cend(), Value_type(0.0));

    Value_type const result = Value_type(acc / n);
    return result;
}

template <typename Value_type>
Value_type
    standard_deviation(std::vector<Value_type> const &_samples)
{
    Value_type const tolerance = Value_type(1E-8);
    Value_type const m = mean<Value_type>(_samples);
    Value_type       moment_2 = std::inner_product(_samples.cbegin(),
                                             _samples.cend(),
                                             _samples.cbegin(),
                                             Value_type(0.0));

    moment_2 /= Value_type(_samples.size());

    Value_type const diff{moment_2 - m * m};

    assert(std::fabs(diff) > tolerance);

    return std::fabs(diff) < tolerance ? Value_type(0.0)
                                       : Value_type{std::sqrt(diff)};
}

template <typename value_type>
void
    test()
{
    std::vector<value_type> const samples{value_type(0.23),
                                          value_type(0.42),
                                          value_type(1.54)};

    value_type const m = mean<value_type>(samples);
    value_type const sd = standard_deviation<value_type>(samples);

    std::cout << "mean=" << static_cast<double>(m) << ", "
              << "standard_deviation=" << static_cast<double>(sd) << std::endl;
}

}  // namespace

int
    main(int, char **)
{
    std::cout << "Floating point:" << std::endl;
    test<floating_accuracy::value_type>();

    std::cout << "Fixed point:" << std::endl;
    test<fixed_accuracy::value_type>();

    return EXIT_SUCCESS;
}
