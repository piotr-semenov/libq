// sin.inl
//
// Copyright (c) 2014-2015 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file sin.inl

 Provides CORDIC for sin function
 \ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"
*/

#ifndef INC_STD_SIN_INL_
#define INC_STD_SIN_INL_

namespace libq {
namespace details {
/*!
 \brief
*/
template<typename T>
class sin_of
{
public:
    typedef T promoted_type;
};

// trick: an extra base class is required to make the compiler to
// instantiate the class representing the fixed-point number of a new format
template<typename T, std::size_t n, std::size_t f, class op, class up>
class sin_of<libq::fixed_point<T, n, f, op, up> >
    :   private libq::fixed_point<T, f, f, op, up>,
        public type_promotion_base<
            libq::fixed_point<T, f, f, op, up>
            , 1u
            , 0
        >
{};
} // details
} // libq

namespace std {
template<typename T, std::size_t n, std::size_t f, class op, class up>
typename libq::details::sin_of<libq::fixed_point<T, n, f, op, up> >::promoted_type
    sin(libq::fixed_point<T, n, f, op, up> _val)
{
    typedef typename libq::details::sin_of<libq::fixed_point<T, n, f, op, up> >::promoted_type sin_type;

    // gap in 3 bits is needed for CONST_PI existence
    typedef libq::Q<f + 3u, f, op, up> work_type;

    // convergence interval for CORDIC rotations is [-pi/2, pi/2].
    // So anyone must map the input angle to that interval
    work_type arg(0);
    int sign(1);
    {
        // reduce the argument to interval [-pi, +pi] and preserve its sign
        work_type const x = work_type::CONST_PI - std::fmod(_val, work_type::CONST_2PI);
        if (x < -work_type::CONST_PI_2) {
            arg = x + work_type::CONST_PI;

            sign = -1;
        }
        else if (x > work_type::CONST_PI_2) {
            arg = x - work_type::CONST_PI;

            sign = -1;
        }
        else {
            arg = x;
        }
    }

    typedef libq::cordic::lut<f, work_type> lut_type;
    static auto const angles = lut_type::circular();

    // normalization factor: see page 10, table 24.1 and pages 4-5, equations (5)-(6)
    // factor converges to the limit 1.64676 very fast: it takes just 8 iterations.
    // 8 iterations corresponds to precision of size 0.007812 for the angle approximation
    static work_type norm_factor(1.0 / lut_type::circular_scale(f));

    // rotation mode: see page 6
    // shift sequence is just 0, 1, ... (circular coordinate system)
    work_type x(norm_factor), y(0.0), z(arg);
    work_type x1, y1, z1;
    for (std::size_t i = 0; i != f; ++i) {
        int const sign = (z > work_type(0)) ? 1 : -1;
        work_type const x_scaled = work_type::make_fixed_point(sign * (x.value() >> i));
        work_type const y_scaled = work_type::make_fixed_point(sign * (y.value() >> i));

        x1 = work_type(x - y_scaled);
        y1 = work_type(y + x_scaled);
        z1 = work_type(z - work_type((sign > 0) ? angles[i] : -angles[i]));

        x = x1; y = y1; z = z1;
    }

    return sin_type((sign > 0) ? y : -y);
}
} // std

#endif // INC_STD_SIN_INL_
