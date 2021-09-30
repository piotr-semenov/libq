#include "fixed_point.hpp"

#include <chrono>
#include <iostream>

static constexpr auto const N = 100000000ul;

int
    main(int argc, char **argv)
{
    using namespace std::chrono;
    using std::uintmax_t;

    using Q1 = libq::Q<30, 20>;
    Q1 const value(0.914);
    Q1 volatile result;

    auto const start = system_clock::now();
    for (uintmax_t i = 0; i != N; ++i) {
        result = std::acos(value);
    }

    auto const end = system_clock::now();

    duration<double> const elapsed = end - start;

    std::cout << elapsed.count() / N << std::endl;

    return EXIT_SUCCESS;
}
