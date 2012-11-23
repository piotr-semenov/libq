/// @brief provides CORDIC for cos function
/// @ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"

#include <boost/type_traits/is_floating_point.hpp>

#include <boost/integer.hpp>

namespace core {
    template<typename T>
    class sqrt_of
    {
        BOOST_STATIC_ASSERT(boost::is_floating_point<T>::value);

    public:
        typedef T type;
    };

    template<typename T, size_t n, size_t f, class op, class up>
    class sqrt_of<fixed_point<T, n, f, op, up> >
    {
        static size_t const integer_bits = ((n - f) & 1u)? ((n - f)/2u) + 1u : (n - f)/2u;
        static size_t const fractional_bits = (f & 1u)? (f/2u) + 1u : (f/2u);
        static size_t const total_bits = integer_bits + fractional_bits;

    public:
        typedef fixed_point<
            typename boost::uint_t<total_bits>::least,
            total_bits,
            fractional_bits,
            op,
            up
        > type;
    };
}

namespace std {
    /// @brief computes square root by CORDIC-algorithm
    /// @ref page 11
    template<typename T, size_t n, size_t f, class op, class up>
    typename core::sqrt_of<core::fixed_point<T, n, f, op, up> >::type sqrt(core::fixed_point<T, n, f, op, up> val)
    {
        typedef core::fixed_point<T, n, f, op, up> fp;

        assert(("sqrt parameter is negative", val >= fp(0)));
        if (val < fp(0)) {
            throw std::exception("sqrt: arg is negative");
        }

        if (val == fp(0.0)) {
            return fp::sqrt_type(0.0);
        }

        // Chosen fixed-point format must have several bits to represent
        // lut. Also format must enable argument translating to interval [1.0, 2.0].
        // So format must reserve two bits at least for integer part.
        typedef core::fixed_point<boost::int_t<1u+f+2u>::least, f+2u, f, op, up> work_type;
        typedef core::cordic::lut<f, work_type> lut;

        using boost::mpl::if_;
        struct chooser
        {
            enum { value = (fp::total - fp::fractionals) >= 2 };
        };
        typedef if_<chooser, fp, work_type>::type reduce_type;

        int power(0);
        reduce_type arg(val);
        if (arg == reduce_type(1.0)) {
            return fp::sqrt_type(1.0);
        }

        while (arg >= work_type(2.0)) {
            as_native(arg) >>= 1u;
            power--;
        }
        while (arg < work_type(1.0)) {
            as_native(arg) <<= 1u;
            power++;
        }

        // CORDIC vectoring mode:
        lut const angles = lut::hyperbolic_wo_repeated_iterations();
        typename core::U_fixed_point<f, f>::type const norm(lut::hyperbolic_scale_with_repeated_iterations(n));
        work_type x(work_type(arg) + 0.25), y(work_type(arg) - 0.25), z(arg);
        {
            size_t repeated(4u);
            size_t num(0);

            for (size_t i(1u); i < f + 1u; ++i)
            {
                int const sign = ((x.value() < 0)? -1 : +1) * ((y.value() < 0)? -1 : +1);
                work_type::word_type const store(x.value());
                x = x - work_type::wrap(sign * (y.value() >> (num + 1u)));
                y = y - work_type::wrap(sign * (store >> (num + 1u)));
                z = (sign > 0) ? z + angles[num] : z - angles[num];

                // do repetition to receive convergence
                if (i == repeated && i != n - 1) {
                    int const sign = ((x.value() < 0)? -1 : +1) * ((y.value() < 0)? -1 : +1);
                    work_type::word_type const store(x.value());
                    x = x - work_type::wrap(sign * (y.value() >> (num + 1u)));
                    y = y - work_type::wrap(sign * (store >> (num + 1u)));
                    z = (sign > 0) ? z + angles[num] : z - angles[num];

                    i += 1u;
                    repeated = 3u * repeated  + 1u;
                }

                num += 1u;
            }
        }

        reduce_type result(x / norm);
        if (power > 0) {
            as_native(result) >>= (power >> 1u);
            if (power & 1u) {
                result = result / reduce_type::CONST_SQRT2;
            }
        }
        else {
            size_t const p(-power);
            as_native(result) <<= (p >> 1u);
            if (p & 1u) {
                result = result * work_type::CONST_SQRT2;
            }
        }

        return fp::sqrt_type(result);
    }
}
