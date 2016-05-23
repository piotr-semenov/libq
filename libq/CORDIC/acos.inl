// acos.inl
//
// Copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file acos.inl

 Provides CORDIC for acos function.
 \ref see see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures".
*/

#ifndef INC_STD_ACOS_INL_
#define INC_STD_ACOS_INL_

namespace libq {
namespace details {
/*!
 \brief
*/
template<typename T>
class acos_of {
 public:
    using promoted_type = T;
};


template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
class acos_of<libq::fixed_point<T, n, f, e, op, up> >
    : private libq::fixed_point<T, 0, f, e, op, up>,
      public
         type_promotion_base<libq::fixed_point<T, 0, f, e, op, up>, 3u, 0, 0> {
};
}  // namespace details
}  // namespace libq


namespace std {
/*!
 <I>Example 1</I>: "how-to" to improve the performance of the array
 initialization
 \code{.cpp}
    #include "fixed_point.hpp"

    #include <cstdlib>
    #include <cstdint>

    #include <chrono>
    #include <iostream>

    #define N 100000000ul

    int main(int argc, char** argv) {
        using namespace std::chrono;
        using std::uintmax_t;

        using Q1 = libq::Q<30, 20>;
        Q1 const value(0.914);
        Q1 volatile result;

        auto const start = system_clock::now();
        for (uintmax_t i = 0; i != N; i++) {
            result = std::acos(value);
        }
        auto const end = system_clock::now();
        duration<double> const elapsed = end - start;

        std::cout
            << elapsed.count() / N
            << std::endl;

        return EXIT_SUCCESS;
    }
 \endcode
 \note The target machine is x64 Intel(R) i7-4702MQ CPU @ 2.20GHz 2.20GHz.
 Please see the performance points for the native code generated by the engines
 below:
 - g++ ver. 4.8.3 (CYGWIN_NT-6.3 x86_64) :
  * \verbatim
g++ -std=gnu++11 -O3 -ftemplate-depth=101 -I"C:\Users\peter.semenov\Libs\boost_1_57_0" -DLOOP_UNROLLING -mtune=native -o example2 ./example2.cpp 2> log
while true; do ./example2; done
\endverbatim
- MS VC 18.00.21005.1 (x64):
* \verbatim
SET PATH=C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC;%PATH%
vcvarsall.bat amd64
cl /nologo /O2 /favor:AMD64 /I "C:\Users\peter.semenov\Libs\boost_1_57_0" /DLOOP_UNROLLING /Feexample2 ./example2.cpp
FOR /L %i in (1,1,50) do (example2.exe) >> log.txt
\endverbatim
*/
template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
typename libq::details::acos_of<libq::fixed_point<T, n, f, e, op, up> >::promoted_type  // NOLINT
    acos(libq::fixed_point<T, n, f, e, op, up> _val) {
    using Q = libq::fixed_point<T, n, f, e, op, up>;
    using result_type = typename libq::details::acos_of<Q>::promoted_type;
    using lut_type = libq::cordic::lut<f, Q>;

    assert(("[std::acos] argument is not from [-1.0, 1.0]",
            std::fabs(_val) <= Q(1.0)));
    if (std::fabs(_val) > Q(1.0)) {
        throw std::logic_error("[std::acos] argument is not from [-1.0, 1.0]");
    }
    if (_val == Q(1.0)) {
        return result_type::wrap(0);
    } else if (_val == Q(-1.0)) {
        return result_type::CONST_PI;
    } else if (_val == Q::wrap(0)) {
        return result_type::CONST_PI_2;
    }

    bool const is_negative = std::signbit(_val);
    _val = std::fabs(_val);


    static lut_type const angles = lut_type::circular();
    static lut_type const scales = lut_type::circular_scales();

    // rotation mode: see page 6
    // shift sequence is just 0, 1, ... (circular coordinate system)
    result_type x(1.0), y(0.0), z(0.0);

#ifdef LOOP_UNROLLING
    auto const iteration_body = [&](std::size_t i) {  // NOLINT
#else
#pragma unroll
    for (std::size_t i = 0u; i != f; ++i) {
#endif
        int sign(0);
        if (_val <= x) {
            sign = (y < 0.0) ? -1 : +1;
        } else {
            sign = (y < 0.0) ? +1 : -1;
        }

        typename result_type::storage_type const storage(x.value());
        x = x - result_type::wrap(sign * (y.value() >> i));
        y = y + result_type::wrap(sign * (storage >> i));
        z = (sign > 0)? z + angles[i] : z - angles[i];
        _val = _val * scales[i];  // multiply by square of K(n)
    };  // NOLINT
#ifdef LOOP_UNROLLING
    libq::details::unroll(iteration_body, 0u, libq::details::loop_size<f-1>());
#endif

    return (is_negative) ?
               result_type(result_type::CONST_PI - std::fabs(z)) :
               std::fabs(result_type(z));
}
}  // namespace std

#endif  // INC_STD_ACOS_INL_
