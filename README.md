# LibQ: easy compile-time switch between floating-point/highly configurable fixed-point computations in C++
## Project Motivation
Imagine you have some floating-point algorithm implemented in C++. Now you want to discover how accurate it is
for the application-specific fixed-point hardware. How to do the simulations without algorithm
reimplementation/update? How to observe the trade-off between the accuracy loss and chosen fixed-point word length?
LibQ proposes you the convenient and highly-configurable solution. Do not fix your algorithm, just say that your
value type is QN.M, not float/double as previously. The only payment for this magic is to make sure that your
algorithm satisfies the simple C++ code style requirements.

Competitors:
* https://github.com/mizvekov/fp/tree/master/tests/static
* https://github.com/quicky2000/fixed_point
* https://github.com/viboes/fixed_point
* https://github.com/Koasing/FixedPoint
* https://github.com/lhartmann/fixed_point
* https://github.com/kirket/compile_time_fixed_point_library
* https://github.com/davidatroberts/FixedPoint
* https://github.com/trenki2/FixedPoint
* https://github.com/rroa/FixedPoint
* https://github.com/gsarkis/libfi
* https://github.com/tyc85/FixedPointLDPC
* https://github.com/geediiiiky/MyFixedPoint
* https://github.com/cajun-rat/fixed-point-compiler


## Main Disadvantages of C++ Competitors
Fixed-point Library | Overloads for sin/cos/asin/etc. | Result Type Promotion | Description
------------------- | ------------------------------- | --------------------- | -----------
https://github.com/coder-mike/FixedPoint | - | - (does the numeric conversions) | Cannot handle the negative count 
https://github.com/juliusikkala/Fixed-point | - | 
https://github.com/FS-NulL/Fixed-Point | - | 
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

## Full List of LibQ Features
1. Lib was implemented in the most general (templates, constructors and so on)
way to have the easy float-point/fixed-point and vice versa translations (w/o
time-consuming editing/refactoring of your code);
2. Lib supports any Q-format of fixed-point (not only power of two as its
analogs do);
3.	Lib does the compile-time type inference for resulting type of addition,
subtraction, multiplication, division, all elementary functions (exp, log, sin,
cos, tan, asin, acos, atan, sinh, cosh, tanh, asinh, acosh, atanh).
Existing fixed-point libs do not do this stuff at all.
They assume fixed-point types to be closed under all operations;
4.	Lib enables capturing the overflow/underflow behavior in run-time (BTW, it
can be ignored);
5.	Lib provides the implementation for all math functions for fixed-point
numbers (CORDIC-based algorithms);
6.	Lib provides the compile-time scheduling for multiplication/division in goal
to achieve max possible precision of the result;
7.	Lib is easy-scalable for any computation platform (24-bit, 28-bit, 32-bit,
64-bit, 128-bit and so on).
8. Lib allows to capture the numbers range explicitly. For example, using
wordlength = 10 and fractional bits = 32, we define the pre-factor 2^-22.
9. Works with old C++ 99 code (no C++11, C++13 features are used). But you need the BOOST MPL
10. Lib has no external dependencies. It just uses Boost for unit tests.

## Code Style Requirements If You Want to Use LibQ in Your Code


## Roadmap
R1C - 
