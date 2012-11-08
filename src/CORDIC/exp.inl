/// @brief provides CORDIC for ln function
/// @ref see C. Baumann, "A simple and fast look-up table method to compute the
/// exp(x) and ln(x) functions", 2004

#include "./../../fixed_point_lib/src/CORDIC/lut/lut.hpp"

#include <boost/foreach.hpp>
#include <boost/range/irange.hpp>

namespace std {
    template<typename T, size_t n, size_t f, class op, class up>
    core::fixed_point<T, n, f, op, up> exp(core::fixed_point<T, n, f, op, up> const& val)
    {
        typedef core::fixed_point<T, n, f, op, up> fp;
        typedef core::cordic::lut<f, fixed_point> lut_type;

        // reduces argument to interval [1.0, 2.0]
        int power(0);
        fp arg(val);
        while (arg >= fp(1.0)) {
            arg = arg - 1u;
            power++;
        }
        while (arg < fp(0.0)) {
            arg = arg + 1u;
            power--;
        }

        static lut_type const log2_lut = lut_type::build_log2_lut();
        fp result(1.0);
        BOOST_FOREACH(size_t i, boost::irange<size_t>(0, f, 1))
        {
            if (fp(arg - fp::wrap(T(1u) << (f - i - 1u))) >= fp(0.0)) {
                arg = arg - fp::wrap(T(1u) << (f- i -1u));

                result = fp(result * log2_lut[i]);
            }
        }

        result = result * (fp::wrap(T(1u) << (f + power)));
        return result;
    }
}
