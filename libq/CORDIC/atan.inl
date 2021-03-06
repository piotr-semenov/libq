// atan.inl
//
// Copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file atan.inl

 Provides CORDIC for tanh function as a ratio of sinh and tanh
 \ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"
*/

#ifndef INC_STD_ATAN_INL_
#define INC_STD_ATAN_INL_


namespace libq {
namespace details {
/*!
 \brief
*/
template<typename T>
class atan_of {
 public:
    using promoted_type = T;
};

template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
class atan_of<libq::fixed_point<T, n, f, e, op, up> >
    : public asin_of<libq::fixed_point<T, n, f, e, op, up> > {
};
}  // namespace details
}  // namespace libq


namespace std {
template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
typename libq::details::atan_of<libq::fixed_point<T, n, f, e, op, up> >::promoted_type  // NOLINT
    atan(libq::fixed_point<T, n, f, e, op, up> _val) {
    using Q = libq::fixed_point<T, n, f, e, op, up>;
    using result_type =
        typename libq::details::atan_of<libq::fixed_point<T, n, f, e, op, up> >::promoted_type;  // NOLINT
    using lut_type = libq::cordic::lut<f, Q>;

    static lut_type const angles = lut_type::circular();

    // vectoring mode: see page 10, table 24.2
    // shift sequence is just 0, 1, ... (circular coordinate system)
    result_type x(1.0), y(_val), z(0.0);

#ifdef LOOP_UNROLLING
    auto const iteration_body = [&](std::size_t i) {  // NOLINT
#else
    for (std::size_t i = 0u; i != f; ++i) {
#endif
        int const sign = ((x.value() > 0)? +1 : -1) *
            ((y.value() > 0)? +1 : -1);

        typename result_type::storage_type const store(x.value());
        x = x + result_type::wrap(sign * (y.value() >> i));
        y = y - result_type::wrap(sign * (store >> i));
        z = (sign > 0)? z + angles[i] : z - angles[i];
    };  // NOLINT
#ifdef LOOP_UNROLLING
    libq::details::unroll(iteration_body, 0u, libq::details::loop_size<f-1>());
#endif

    return z;
}
}  // namespace std

#endif  // INC_STD_ATAN_INL_
