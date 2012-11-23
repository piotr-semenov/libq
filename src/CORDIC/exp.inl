/// @brief provides CORDIC for ln function
/// @ref see C. Baumann, "A simple and fast look-up table method to compute the
/// exp(x) and ln(x) functions", 2004

#include "./lut/lut.hpp"

#include <boost/foreach.hpp>
#include <boost/range/irange.hpp>

#include <boost/type_traits/is_floating_point.hpp>

namespace core {
    template<typename T>
    class exp_of
    {
        BOOST_STATIC_ASSERT(boost::is_floating_point<T>::value);

    public:
        typedef T type;
    };

    template<typename T, size_t n, size_t f, class op, class up>
    class exp_of<fixed_point<T, n, f, op, up> >
    {
    public:
        typedef fixed_point<
            boost::uintmax_t,
            std::numeric_limits<boost::uintmax_t>::digits,
            f,
            op,
            up
        > type;
    };
}

namespace std {
    template<typename T, size_t n, size_t f, class op, class up>
    typename core::exp_of<core::fixed_point<T, n, f, op, up> >::type exp(core::fixed_point<T, n, f, op, up> val)
    {
        typedef core::fixed_point<T, n, f, op, up> fp_type;
        typedef core::exp_of<fp_type>::type result_type;

        typedef core::cordic::lut<f, fp_type> lut_type;
        typedef core::S_fixed_point<f+1u, f>::type work_type;

        // reduces argument to interval [0.0, 1.0]
        int power(0);
        core::product<fp_type, work_type>::type arg(val * work_type::CONST_LOG2E);
        while (arg >= result_type(1.0)) {
            arg = arg - 1u;
            power++;
        }
        while (arg < result_type(0.0)) {
            arg = arg + 1u;
            power--;
        }

        static lut_type const pow2_lut = lut_type::pow2();
        result_type result(1.0);
        work_type x(arg);
        BOOST_FOREACH(size_t i, boost::irange<size_t>(0, f, 1))
        {
            work_type const pow2 = work_type::wrap(T(1u) << (f - i - 1u));

            if (x - pow2 >= work_type(0.0)) {
                x = x - pow2;

                result *= pow2_lut[i];
            }
        }

        if (power >= 0) {
            as_native(result) <<= power;
        }
        else {
            as_native(result) >>= (-power);
        }
        return result;
    }
}
