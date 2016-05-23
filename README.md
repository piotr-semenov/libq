# LibQ: Constrain the C++ floating-point algorithms to fixed-point computations easily without any pain!
## Project Motivation
Imagine you have some floating-point algorithm implemented in C++.
For example, consider the well-known SURF algorithm for keypoint detection.
Now you want to replace all the floating-point calculations by the fixed-point arithmetics at once without the algorithm reimplementation.
For example, you want to investigate how accurately the algorithm will work for the application-specific hardware and observe the trade-off between the accuracy loss and chosen word length.
LibQ proposes you the simple, convenient and highly-configurable solution.
Do not fix your floating-point algorithm, just say that you want QN.M as the value type.
The only requirement to make this magic to work is to ensure that your algorithm satisfies some simple C++ code style requirements.

## Main Disadvantages of C++ Competitors
The most of the fixed-point libraries do not support the math functions from <cmath> (e.g. CORDIC algorithms for sin/cos/asin/etc) at all.
Moreover, libraries suggests no type promotion (e.g. for arithmetic operations +,-,/,*). 
Please, find the summary below.

Pass the following simple test emphasizing the main idea behind the LibQ:
    #include <cstdlib>
    namespace fixed_accuracy {
        using value_type = Q<24, 12>;
    }  // namespace fixed_accuracy
    namespace floating_accuracy {
        using value_type = double;
    }  // namespace floating_accuracy
TODO: add an extra column "Pass the test" to the table below.

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
https://github.com/geediiiiky/MyFixedPoint | | |
https://github.com/cajun-rat/fixed-point-compiler | | |

- GitHub repos: https://github.com/coder-mike/FixedPoint, https://github.com/juliusikkala/Fixed-point, https://github.com/FS-NulL/Fixed-Point
 * No overloads for math functions like sin, cos, etc.
 * Can not handle the negative count of bits for fractional part.
 * For the arithmetical operations, it can not infer the actual result type, just do the numeric conversions.
- GitHub repo: https://github.com/PetteriAimonen/libfixmath
 * Only for fixed count of word length
 * No ability to update your floating-point calculatins to the fixed precision easily
- https://github.com/johnmcfarlane/fixed_point/blob/master/doc/p0037.md, https://github.com/johnmcfarlane/fixed_point/blob/master/src/test/snippets.cpp:
 * For the arithmetical operations, it does not infer the actual result type, just do the numeric conversions
 (decltype(std::declval<typename Lhs::rep>() + std::declval<typename Rhs::rep>()) by
 delegating the implicit numeric conversions to the compiler.
- fixed-point c++ class from http://www.trenki.net/content/view/17/1/
 * word size is up to 32 bits only
 * sin/cos/sqrt functions only work for Q16.16 format
 * no promotion of sum/product/etc. types

- fpmath from http://fpmath.googlecode.com/svn/trunk/
 * Only 8, 16, 32 bits are available

- libfixmath from https://code.google.com/p/libfixmath/
 * lib implements the math functions only for Q16.16 format
 * no C++ operators for basic arithmetics
 * lib configuration is macros-based

- Schloesser, Oliver (2013). Implementing a C++ Fixed-Point Class for Embedded Systems. technical report (see http://www.fhnw.ch/technik/ime/publikationen/2013/2013_cppfp_paper_osc.pdf)

- 

## LibQ Features
1. Supports any Q-format (not only 2's powers).
2. Supports the compile-time type promotions for arithmetics (+, -, *, /), all elementary functions (exp, log, sin, cos, tan, asin, acos, atan, sinh, cosh, tanh, asinh, acosh, atanh).
3. Proposes certain policies for the overflow/underflow in run-time.
4. Provides the CORDIC-based implementation for math functions from <cmath>.
5. Enables user to capture the dynamic range explicitly.
For example, if set the pre-factor \f$2^{-12}\f$ for unsigned Q0.32 then user gets the range \f$[0.0, 2^{-44}]\f$.

## Code Style Requirements If You Want to Use LibQ in Your Code


## Roadmap/Future Plans
R1C - 


## TODO
- Unify all comments (start it with the capital letter)
- Guard definitions to all inl files

https://github.com/tyc85/FixedPointLDPC | | |