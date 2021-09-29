# LibQ: Limit the C++ floating-point algorithms to fixed-point computations easily without any pain :)
## Project Motivation
Imagine you have some floating-point algorithm implemented in C++.
For example, consider the well-known SURF algorithm for keypoint detection.
Now you want to replace all the floating-point calculations by the fixed-point arithmetics at once without the algorithm reimplementation.
For example, you want to investigate how accurately the algorithm will work for the application-specific hardware and observe the trade-off between the accuracy loss and chosen word length.
LibQ proposes you the simple, convenient and highly-configurable solution.
Do not fix your floating-point algorithm, just say that you want QN.M as the value type.
The only requirement to make this magic to work is to ensure that your algorithm satisfies some simple C++ code style requirements.

Please, see the simple code emphasizing the main idea behind the LibQ:

```` cpp
#include "libq/fixed_point.hpp"

#include <cstdlib>
#include <vector>
#include <numeric>
#include <iostream>
#include <cassert>

template <typename Value_type>
Value_type
    mean(std::vector<Value_type> const& _samples)
{
    Value_type const n = Value_type(_samples.size());
    Value_type const acc = std::accumulate(_samples.cbegin(),
                                           _samples.cend(),
                                           Value_type(0.0));

    Value_type const result = Value_type(acc / n);
    return result;
}

template <typename Value_type>
Value_type
    standard_deviation(std::vector<Value_type> const& _samples)
{
    Value_type const TOLERANCE = Value_type(1E-8);

    Value_type const m = mean<Value_type>(_samples);
    Value_type moment_2 = std::inner_product(_samples.cbegin(),
                                             _samples.cend(),
                                             _samples.cbegin(),
                                             Value_type(0.0));
    moment_2 /= Value_type(_samples.size());

    Value_type const diff = moment_2 - m * m;
    assert(std::fabs(moment_2 - m * m) > TOLERANCE);

    if (std::fabs(moment_2 - m * m) < TOLERANCE) {
        return Value_type(0.0);
    } else {
        Value_type const result = std::sqrt(moment_2 - m * m);
        return result;
    }
}


namespace fixed_accuracy {
    // Assume, we know that all the samples are < 2.0
    using value_type = libq::Q<31, 29>;
}  // namespace fixed_accuracy

namespace floating_accuracy {
    using value_type = double;
}  // namespace floating_accuracy

int
    main(int argc, char** argv)
{
    using value_type = fixed_accuracy::value_type;

    std::vector<value_type> const samples{
        value_type(0.23), value_type(0.42), value_type(1.54) };
    value_type const m = mean<value_type>(samples);
    value_type const sd = standard_deviation<value_type>(samples);

    std::cout
        << static_cast<double>(m)
        << "\t" << static_cast<double>(sd)
        << std::endl;
    return EXIT_SUCCESS;
}
````

## Main Disadvantages of C++ Competitors
The most of the fixed-point libraries do not support the math functions from `<cmath>` (e.g. CORDIC algorithms for sin/cos/asin/etc) at all.
Moreover, libraries suggests no type promotion (e.g. for arithmetic operations +,-,/,*). 

> Note, `<cmath>` supporting and type promotion are the substantial requirements to make the example above to work.

Please, find the summary table for competitors below.

Fixed-point Library | Supports sin/cos/asin/etc. | Supports type promotions | Additional description
------------------- | -------------------------- | ------------------------ | ----------------------
https://github.com/coder-mike/FixedPoint | - | - (Uses the integral promotion instead) | Cannot represent the arbitrary dynamic range.
https://github.com/juliusikkala/Fixed-point | - | - (Operands have to be normalized and the result type is promoted to the type of lhs) | Cannot represent the arbitrary dynamic range.
https://github.com/FS-NulL/Fixed-Point | - | - (The result type is promoted to the widest one from lhs, rhs) | Represents the numbers in base of \f$10\f$.
https://github.com/mizvekov/fp | - | - (Uses the integral promotion instead) | Provides the adaptors for Boost.Rational, Boost.MultiPrecision. Provides the numeric_limits tempalte specialization.
https://github.com/quicky2000/fixed_point | - | - (Supports 16-bit, 32-bit word length) | Is a C library. No C++ is supported.
https://github.com/viboes/fixed_point | - | + | Satisfies the BOOST lib requirements. Supports certain rounding policies. Unfortunately, lib provides the complex interface and no simple syntax sugar to hit the LibQ's targets.
https://github.com/Koasing/FixedPoint | - | - (Works only with word lengths before 31 bits) | Is a C library. No C++ is supported.
https://github.com/lhartmann/fixed_point | - | +- (Works only with word lengths of 32 and 64 bits) | Cannot represent the arbitrary dynamic range.
https://github.com/kirket/compile_time_fixed_point_library | - | +- (Works with word length of 64 bits) | An improvement of SystemC's sc_int type.
https://github.com/davidatroberts/FixedPoint | - | - | Very limited implementation.
https://github.com/trenki2/FixedPoint | - (Implements sin/cos/sqrt for Q32.16) | - (Works only with word length of 32 bits) | Cannot represent the arbitrary dynamic range.
https://github.com/rroa/FixedPoint | - (Has the sin implementation of limits accuracy) | - (Works only with word length of 32 bits) | Cannot represent the arbitrary dynamic range.
https://github.com/gsarkis/libfi | - | - | Supports certain rounding/overflow policies. Provides the parser for the stringified numbers.
https://github.com/geediiiiky/MyFixedPoint | - (Implements the inverse sqrt only) | - (Supports only the 64-bit word length) | Cannot represent the arbitrary dynamic range.
https://github.com/PetteriAimonen/libfixmath | +- (Implemented only for Q32.16) | - | Is a C libyrary. No C++ is supported.
https://github.com/johnmcfarlane/fixed_point | - (The `<cmath>` functions are just wrapped to provide fixed-point conversions of floating-point results) | - (Delegates the underlying type promotion to compiler. The resulting exponent for fixed-point arithmetics is always the maximum one) | This is the reference implementation of the recent proposal P0037R1 ("Fixed-point Real Numbers") for C++ STD (please, see the corresponding paper at http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0037r1.html). Also it provides natvis for Visual Studio's debugger.


## Main LibQ Features
1. Supports any word lengths (not only 2's powers) and any reasonable dynamic ranges.
2. Supports the CORDIC-implementation of all the elementary functions from `<cmath>` (exp, log, sin, cos, tan, asin, acos, atan, sinh, cosh, tanh, asinh, acosh, atanh).
3. Provides the type promotions for the arithmetics (+, -, *, /) and all elementary functions listed above.
4. Provides the certain policies for the overflow/underflow in run-time.
5. Enables the user to simulate his fixed-point algorithm with fixed-point numbers easily (see the Project Motivation section).


## Simple Code Style Requirements If You Want the LibQ to Do the Magin for You
TBD
