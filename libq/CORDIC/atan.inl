// atan.inl
//
// Copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file atan.inl

 Provides CORDIC for tanh function as a ratio of sinh and tanh
 \ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"
*/

namespace libq {
namespace details {
/*!
 \brief
*/
template<typename T>
class atan_of
{
public:
    using promoted_type = T;
};

template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
class atan_of<libq::fixed_point<T, n, f, e, op, up> >
    :    public asin_of<libq::fixed_point<T, n, f, e, op, up> >
{};
} // details
} // libq

namespace std {
template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
typename libq::details::atan_of<libq::fixed_point<T, n, f, e, op, up> >::promoted_type
    atan(libq::fixed_point<T, n, f, e, op, up> _val)
{
    using Q = libq::fixed_point<T, n, f, e, op, up>;
    using result_type = typename libq::details::atan_of<libq::fixed_point<T, n, f, e, op, up> >::promoted_type;
    using lut_type = libq::cordic::lut<f, Q>;

    static lut_type const angles = lut_type::circular();

    // vectoring mode: see page 10, table 24.2
    // shift sequence is just 0, 1, ... (circular coordinate system)
    result_type x(1.0), y(_val), z(0.0);
    for (std::size_t i = 0; i != f; ++i) {
        int const sign = ((x.value() > 0)? +1 : -1) * ((y.value() > 0)? +1 : -1);

        typename result_type::storage_type const store(x.value());
        x = x + result_type::wrap(sign * (y.value() >> i));
        y = y - result_type::wrap(sign * (store >> i));
        z = (sign > 0)? z + angles[i] : z - angles[i];
    }

    return z;
}
} // std
