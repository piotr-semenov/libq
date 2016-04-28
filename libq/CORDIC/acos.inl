// acos.inl
//
// Copyright (c) 2014-2015 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file acos.inl

 Provides CORDIC for acos function
 \ref see see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"
*/

#ifndef INC_STD_ACOS_INL_
#define INC_STD_ACOS_INL_

namespace libq {
namespace details {
/*!
 \brief
*/
template<typename T>
class acos_of
{
public:
    using promoted_type = T;
};

template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
class acos_of<libq::fixed_point<T, n, f, e, op, up> >
    :   private libq::fixed_point<T, 0, f, e, op, up>,
        public type_promotion_base<
            libq::fixed_point<T, 0, f, e, op, up>
            , 3u
            , 0
            , 0
        >
{};
} // details
} // libq

namespace std {
template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
typename libq::details::acos_of<libq::fixed_point<T, n, f, e, op, up> >::promoted_type
    acos(libq::fixed_point<T, n, f, e, op, up> _val)
{
    using Q = libq::fixed_point<T, n, f, e, op, up>;
    using result_type = typename libq::details::acos_of<Q>::promoted_type;
    using lut_type = libq::cordic::lut<f, Q>;

    assert(("[std::acos] argument is not from [-1.0, 1.0]", std::fabs(_val) <= Q(1.0)));
    if (std::fabs(_val) > Q(1.0)) {
        throw std::logic_error("[std::acos] argument is not from [-1.0, 1.0]");
    }
    if (_val == Q(1.0)) {
        return result_type::make_fixed_point(0);
    }
    else if (_val == Q(-1.0)) {
        return result_type::CONST_PI;
    }
    else if (_val == Q::make_fixed_point(0)) {
        return result_type::CONST_PI_2;
    }

    bool const is_negative = std::signbit(_val);
    _val = std::fabs(_val);

    static lut_type const angles = lut_type::circular();
    static lut_type const scales = lut_type::circular_scales();

    // rotation mode: see page 6
    // shift sequence is just 0, 1, ... (circular coordinate system)
    result_type x(1.0), y(0.0), z(0.0);
    for (std::size_t i = 0; i != f; ++i) {
        int sign(0);
        if (_val <= x) {
            sign = (y < 0.0) ? -1 : +1;
        }
        else {
            sign = (y < 0.0) ? +1 : -1;
        }

        typename result_type::storage_type const storage(x.value());
        x = x - result_type::make_fixed_point(sign * (y.value() >> i));
        y = y + result_type::make_fixed_point(sign * (storage >> i));
        z = (sign > 0)? z + angles[i] : z - angles[i];
        _val = _val * scales[i]; // multiply by square of K(n)
    }

    return (is_negative) ? result_type(result_type::CONST_PI - std::fabs(z)) :
        std::fabs(result_type(z));
}
} // std

#endif // INC_STD_ACOS_INL_
