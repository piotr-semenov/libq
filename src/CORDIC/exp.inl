/// @brief provides CORDIC for ln function
/// @ref see C. Baumann, "A simple and fast look-up table method to compute the
/// exp(x) and ln(x) functions", 2004

#include "./../../fixed_point_lib/src/CORDIC/lut/lut.hpp"

#include <boost/foreach.hpp>
#include <boost/range/irange.hpp>

namespace std {
    template<typename T, size_t n, size_t f, class op, class up>
    typename core::fixed_point<T, n, f, op, up>::exp_type exp(core::fixed_point<T, n, f, op, up> const& val)
    {
        typedef core::fixed_point<T, n, f, op, up> fp;
        typedef core::get_<fp>::max_signed_type work_type;
        typedef core::cordic::lut<f, fp> lut;

        // reduces argument to interval [0.0, 1.0]
        int power(0);
        work_type arg(val * fp::CONST_LOG2E);
        while (arg >= fp(1.0)) {
            arg = arg - 1u;
            power++;
        }
        while (arg < fp(0.0)) {
            arg = arg + 1u;
            power--;
        }

        static lut const pow2_lut = lut::pow2();
        work_type result(1.0);
        BOOST_FOREACH(size_t i, boost::irange<size_t>(0, f, 1))
        {
            if (work_type(arg - work_type::wrap(T(1u) << (f - i - 1u))) >= work_type(0.0)) {
                arg = arg - work_type::wrap(T(1u) << (f- i -1u));

                result = work_type(result * pow2_lut[i]);
            }
        }

        //result = result * (work_type::wrap(T(1u) << (f + power)));
        if (power >= 0) {
            as_native(result) <<= power;
        }
        else {
            as_native(result) >>= (-power);
        }
        return fp::exp_type(result);
    }
}
