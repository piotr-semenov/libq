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
