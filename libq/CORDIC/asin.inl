// asin.inl
//
// Copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file asin.inl

 Provides CORDIC for tanh function as a ratio of sinh and tanh
 \ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"
*/

namespace libq {
namespace details {
/*!
 \brief
*/
template<typename T>
class asin_of {
 public:
    using promoted_type = T;
};


template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
class asin_of<libq::fixed_point<T, n, f, e, op, up> >
    : private libq::fixed_point<T, 0, f, e, op, up>,
      public type_promotion_base<
          libq::fixed_point<T, 0, f, e, op, up>
          , 2u
          , 0
          , 0> {
};
}  // namespace details
}  // namespace libq


namespace std {
template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
typename libq::details::asin_of<libq::fixed_point<T, n, f, e, op, up> >::promoted_type  // NOLINT
    asin(libq::fixed_point<T, n, f, e, op, up> _val) {
    using Q = libq::fixed_point<T, n, f, e, op, up>;
    using result_type = typename libq::details::asin_of<Q>::promoted_type;
    using lut_type = libq::cordic::lut<result_type::bits_for_fractional,
                                      result_type>;

    assert(("[std::asin] argument is not from [-1.0, 1.0]",
            std::fabs(_val) <= Q(1.0f)));
    if (std::fabs(_val) > Q(1.0f)) {
        throw std::logic_error("[std::asin] argument is out of range");
    }

    if (_val == Q(1.0f)) {
        return result_type::CONST_PI_2;
    } else if (_val == Q(-1.0f)) {
        return -(result_type::CONST_PI_2);
    } else if (_val == Q(0.0)) {
        return result_type::wrap(0);
    }
    static lut_type const angles = lut_type::circular();
    static lut_type const scales = lut_type::circular_scales();

    // rotation mode: see page 6
    // shift sequence is just 0, 1, ... (circular coordinate system)
    result_type x(1.0f), y(0.0), z(0.0);
    for (std::size_t i = 0; i != f; ++i) {
        int sign(0);
        if (_val >= y) {
            sign = (x < 0.0)? -1 : +1;
        } else {
            sign = (x < 0.0)? +1 : -1;
        }

        typename result_type::storage_type const store(x.value());
        x = x - result_type::wrap(sign * (y.value() >> i));
        y = y + result_type::wrap(sign * (store >> i));
        z = (sign > 0)? z + angles[i] : z - angles[i];
        _val = _val * scales[i];
    }

    if (z > result_type::CONST_PI_2) {
        z = result_type::CONST_PI - z;
    } else if (z < -result_type::CONST_PI_2) {
        z = -result_type::CONST_PI - z;
    }

    return z;
}
}  // namespace std
