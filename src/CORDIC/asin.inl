/// @brief provides CORDIC for arccos function
/// @ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"

#include "./../../fixed_point_lib/src/CORDIC/lut/lut.hpp"

#include <boost/type_traits/is_floating_point.hpp>

#include <boost/integer.hpp>
#include <boost/mpl/eval_if.hpp>

namespace core {
    template<typename T>
    class asin_of
    {
        BOOST_STATIC_ASSERT(boost::is_floating_point<T>::value);

    public:
        typedef T type;
    };

    template<typename T, size_t n, size_t f, class op, class up>
    class asin_of<fixed_point<T, n, f, op, up> >
    {
        static size_t const available_bits = std::numeric_limits<boost::intmax_t>::digits - f;

        struct can_expand
        {
            enum { value = (available_bits > 2u) };
        };

        struct expanded
        {
            typedef fixed_point<
                typename boost::int_t<f + 2u + 1u>::least,
                f + 2u,
                f,
                op,
                up
            > type;
        };

        struct reduced
        {
            typedef fixed_point<
                typename boost::int_t<f + 2u>::least,
                f + available_bits,
                f + available_bits - 2u,
                op,
                up
            > type;
        };

    public:
        typedef typename boost::mpl::eval_if<can_expand, expanded, reduced>::type type;
    };
}

namespace std {
    template<typename T, size_t n, size_t f, class op, class up>
    typename core::asin_of<core::fixed_point<T, n, f, op, up> >::type asin(core::fixed_point<T, n, f, op, up> val)
    {
        typedef core::fixed_point<T, n, f, op, up> fp;
        typedef typename fp::asin_type result_type;
        typedef core::cordic::lut<result_type::fractionals, result_type> lut;

        assert(("argument has to be from interval [-1.0, 1.0]", std::fabs(val) <= fp(1.0)));
        if (std::fabs(val) > fp(1.0)) {
            throw std::exception("arcsin: argument is out of range");
        }

        if (val == fp(1.0)) {
            return result_type::CONST_PI_2;
        }
        else if (val == fp(-1.0)) {
            return -(result_type::CONST_PI_2);
        }
        else if (val == fp(0)) {
            return result_type::wrap(0);
        }
        static lut const angles = lut::circular();
        static lut const scales = lut::circular_scales();

        // rotation mode: see page 6
        // shift sequence is just 0, 1, ... (circular coordinate system)
        result_type x(1.0), y(0.0), z(0.0);
        BOOST_FOREACH(size_t i, boost::irange<size_t>(0, f, 1))
        {
            int sign(0);
            if (val >= y) {
                sign = (x < 0.0)? -1 : +1;
            }
            else {
                sign = (x < 0.0)? +1 : -1;
            }

            result_type::word_type const store(x.value());
            x = x - result_type::wrap(sign * (y.value() >> i));
            y = y + result_type::wrap(sign * (store >> i));
            z = (sign > 0)? z + angles[i] : z - angles[i];
            val = val * scales[i];
        }

        if (z > result_type::CONST_PI_2) {
            z = result_type::CONST_PI - z;
        }
        else if (z < -result_type::CONST_PI_2) {
            z = -result_type::CONST_PI - z;
        }

        return z;
    }
}
