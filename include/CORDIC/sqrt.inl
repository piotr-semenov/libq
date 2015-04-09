//// sqrt.inl
////
//// Copyright (c) 2014-2015 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
//// Distributed under the New BSD License. (See accompanying file LICENSE)
//
///*!
// \file sqrt.inl
//
// Provides CORDIC for sqrt function
// \ref see see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"
//*/
//
//#ifndef INC_STD_SQRT_INL_
//#define INC_STD_SQRT_INL_
//
//#include "boost/range/irange.hpp"
//#include "boost/foreach.hpp"
//
//namespace libq {
//namespace details {
//
//template<typename T, size_t n, size_t f, class op, class up>
//class sqrt_of
//{
//    enum: std::size_t {
//        integer_part_length = ((n-f) & 1u) ? ((n-f) / 2u) + 1u : (n-f) / 2u,
//        fractional_part_length = (f & 1u) ? (f / 2u) + 1u : (f / 2u),
//
//        significant_bits = integer_part_length + fractional_part_length
//    };
//
//public:
//    typedef typename boost::uint_t<significant_bits>::least promoted_storage_type;
//    typedef libq::fixed_point<
//        promoted_storage_type,
//        significant_bits,
//        fractional_part_length,
//        op,
//        up
//    > promoted_type;
//};
//} // details
//} // libq
//
//namespace std {
//
///*!
// \brief computes square root by CORDIC-algorithm
// \ref page 11
//*/
//template<typename T, std::size_t n, std::size_t f, class op, class up>
//typename libq::details::sqrt_of<T, n, f, op, up>::promoted_type
//    sqrt(libq::fixed_point<T, n, f, op, up> const& _val)
//{
//    typedef libq::fixed_point<T, n, f, op, up> Q;
//    typedef typename libq::details::sqrt_of<T, n, f, op, up>::promoted_type sqrt_type;
//
//    assert(("[std::sqrt] argument is negative", _val >= Q(0)));
//    if (_val < Q(0)) {
//        throw std::logic_error("[std::sqrt]: argument is negative");
//    }
//
//    if (_val == Q(0.0)) {
//        return sqrt_type(0.0);
//    }
//    if (_val == Q(1.0)) {
//        return sqrt_type(1.0);
//    }
//
//    // Work fixed-point format must have several bits to represent
//    // lut. Also format must enable argument translating to interval [1.0, 2.0].
//    // So format must reserve two bits at least for integer part.
//    typedef libq::Q<f+2u, f, op, up> work_type;
//    typedef libq::cordic::lut<f, work_type> lut_type;
//
//    typedef typename std::conditional<
//        Q::number_of_significant_bits - Q::bits_for_fractional >= 2,
//        Q,
//        work_type
//    >::type reduced_type;
//
//    int power(0);
//    reduced_type arg(_val);
//
//    while (arg >= work_type(2.0)) {
//        libq::lift(arg) >>= 1u;
//        power--;
//    }
//    while (arg < work_type(1.0)) {
//        libq::lift(arg) <<= 1u;
//        power++;
//    }
//
//    // CORDIC vectoring mode:
//    lut_type const angles = lut_type::hyperbolic_wo_repeated_iterations();
//    typename libq::UQ<f, f, op, up> const norm(lut_type::hyperbolic_scale_with_repeated_iterations(n));
//    work_type x(work_type(arg) + 0.25), y(work_type(arg) - 0.25), z(arg);
//    {
//        std::size_t repeated(4u);
//        std::size_t num(0);
//
//        for (std::size_t i = 1u; i < f + 1u; ++i) {
//            int const sign = ((x.value() < 0)? -1 : +1) * ((y.value() < 0)? -1 : +1);
//            typename work_type::storage_type const store(x.value());
//            x = x - work_type::make_fixed_point(sign * (y.value() >> (num + 1u)));
//            y = y - work_type::make_fixed_point(sign * (store >> (num + 1u)));
//            z = (sign > 0) ? z + angles[num] : z - angles[num];
//
//            // repeat until convergence is reached
//            if (i == repeated && i != n - 1) {
//                int const sign = ((x.value() < 0)? -1 : +1) * ((y.value() < 0)? -1 : +1);
//                typename work_type::storage_type const store(x.value());
//                x = x - work_type::make_fixed_point(sign * (y.value() >> (num + 1u)));
//                y = y - work_type::make_fixed_point(sign * (store >> (num + 1u)));
//                z = (sign > 0) ? z + angles[num] : z - angles[num];
//
//                i += 1u;
//                repeated = 3u * repeated  + 1u;
//            }
//
//            num += 1u;
//        }
//    }
//
//    reduced_type result(x / norm);
//    if (power > 0) {
//        libq::lift(result) >>= (power >> 1u);
//        if (power & 1u) {
//            result = result / reduced_type::CONST_SQRT2;
//        }
//    }
//    else {
//        std::size_t const p(-power);
//        libq::lift(result) <<= (p >> 1u);
//        if (p & 1u) {
//            result = result * work_type::CONST_SQRT2;
//        }
//    }
//
//    return sqrt_type(result);
//}
//} // std
//
//#endif // INC_STD_SQRT_INL_

namespace libq {
namespace details {

template<typename T>
class sqrt_of
{
public:
    typedef T type;
};

template<typename T, size_t n, size_t f, class op, class up>
class sqrt_of < fixed_point<T, n, f, op, up> >
{
    static size_t const integer_bits = ((n - f) & 1u) ? ((n - f) / 2u) + 1u : (n - f) / 2u;
    static size_t const fractional_bits = (f & 1u) ? (f / 2u) + 1u : (f / 2u);
    static size_t const total_bits = integer_bits + fractional_bits;

public:
    typedef fixed_point <
        typename boost::uint_t<total_bits>::least,
        total_bits,
        fractional_bits,
        op,
        up
    > type;
};
}
}

namespace std {
    /// @brief computes square root by CORDIC-algorithm
    /// @ref page 11
    template<typename T, size_t n, size_t f, class op, class up>
    typename libq::details::sqrt_of<libq::fixed_point<T, n, f, op, up> >::type sqrt(libq::fixed_point<T, n, f, op, up> val)
    {
        typedef libq::fixed_point<T, n, f, op, up> fp;

        assert(("sqrt parameter is negative", val >= fp(0)));
        if (val < fp(0)) {
            throw std::exception("sqrt: arg is negative");
        }

        typedef typename libq::details::sqrt_of<libq::fixed_point<T, n, f, op, up> >::type sqrt_type;
        if (val == fp(0.0)) {
            return sqrt_type(0.0);
        }

        // Chosen fixed-point format must have several bits to represent
        // lut. Also format must enable argument translating to interval [1.0, 2.0].
        // So format must reserve two bits at least for integer part.
        typedef libq::fixed_point<boost::int_t<1u + f + 2u>::least, f + 2u, f, op, up> work_type;
        typedef libq::cordic::lut<f, work_type> lut;

        using boost::mpl::if_;
        struct chooser
        {
            enum { value = (fp::number_of_significant_bits - fp::bits_for_fractional) >= 2 };
        };
        typedef if_<chooser, fp, work_type>::type reduce_type;

        int power(0);
        reduce_type arg(val);
        if (arg == reduce_type(1.0)) {
            return sqrt_type(1.0);
        }

        while (arg >= work_type(2.0)) {
            libq::lift(arg) >>= 1u;
            power--;
        }
        while (arg < work_type(1.0)) {
            libq::lift(arg) <<= 1u;
            power++;
        }

        // CORDIC vectoring mode:
        lut const angles = lut::hyperbolic_wo_repeated_iterations();
        libq::UQ<f, f, op, up> const norm(lut::hyperbolic_scale_with_repeated_iterations(n));
        work_type x(work_type(arg) + 0.25), y(work_type(arg) - 0.25), z(arg);
        {
            size_t repeated(4u);
            size_t num(0);

            for (size_t i(1u); i < f + 1u; ++i)
            {
                int const sign = ((x.value() < 0) ? -1 : +1) * ((y.value() < 0) ? -1 : +1);
                work_type::storage_type const store(x.value());
                x = x - work_type::make_fixed_point(sign * (y.value() >> (num + 1u)));
                y = y - work_type::make_fixed_point(sign * (store >> (num + 1u)));
                z = (sign > 0) ? z + angles[num] : z - angles[num];

                // do repetition to receive convergence
                if (i == repeated && i != n - 1) {
                    int const sign = ((x.value() < 0) ? -1 : +1) * ((y.value() < 0) ? -1 : +1);
                    work_type::storage_type const store(x.value());
                    x = x - work_type::make_fixed_point(sign * (y.value() >> (num + 1u)));
                    y = y - work_type::make_fixed_point(sign * (store >> (num + 1u)));
                    z = (sign > 0) ? z + angles[num] : z - angles[num];

                    i += 1u;
                    repeated = 3u * repeated + 1u;
                }

                num += 1u;
            }
        }

        reduce_type result = static_cast<reduce_type>(x / norm);
        if (power > 0) {
            libq::lift(result) >>= (power >> 1u);
            if (power & 1u) {
                result = result / reduce_type::CONST_SQRT2;
            }
        }
        else {
            size_t const p(-power);
            libq::lift(result) <<= (p >> 1u);
            if (p & 1u) {
                result = result * work_type::CONST_SQRT2;
            }
        }

        return sqrt_type(result);
    }
}
