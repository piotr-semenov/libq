#include "loop_unroller.hpp"

#include <cstdlib>
#include <cstdint>

#include <vector>
#include <numeric>
#include <chrono>
#include <iostream>

static constexpr auto const N = 100000000ul;

int
    main(int argc, char** argv)
{
    using namespace std::chrono;
    using std::uintmax_t;

    std::vector<uintmax_t> array(N);

    auto const filler = [&](uintmax_t const _i) {
        array[_i] =
            _i * (_i + 1u) * (_i + 2u) * (_i + 3u) * (_i + 4u) * (_i + 5u);
    };

    auto const start1 = system_clock::now();

    for (uintmax_t i = 0; i != N; i += CHUNK_LENGTH) {
        libq::details::unroll(filler,
                              i,
                              libq::details::loop_size<CHUNK_LENGTH - 1>());
    }

    auto const             end1 = system_clock::now();
    duration<double> const elapsed1 = end1 - start1;

    auto const start2 = system_clock::now();

    for (uintmax_t i = 0; i != N; ++i) {
        array[i] = i * (i + 1u) * (i + 2u) * (i + 3u) * (i + 4u) * (i + 5u);
    }

    auto const             end2 = system_clock::now();
    duration<double> const elapsed2 = end2 - start2;

    std::cout << elapsed1.count() << " " << elapsed2.count() << std::endl;

    return EXIT_SUCCESS;
}
