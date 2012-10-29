/// @brief provides CORDIC for ln function
/// @ref see C. Baumann, "A simple and fast look-up table method to compute the
/// exp(x) and ln(x) functions", 2004

#include "./../../fixed_point_lib/src/CORDIC/lut/lut.hpp"

#include <boost/foreach.hpp>
#include <boost/range/irange.hpp>

namespace std {
    template<typename T, size_t n, size_t f, class op, class up>
    utils::number<T, n, f, op, up> exp(utils::number<T, n, f, op, up> const& val)
    {
        typedef utils::number<T, n, f, op, up> fixed_point;
        typedef utils::cordic::lut<f, fixed_point> lut_type;

        assert(("log2: argument has to be positive", val > fixed_point(0)));

        // reduces argument to interval [1.0, 2.0]
        int power(0);
        fixed_point arg(val);
        while (arg >= fixed_point(1.0)) {
            arg = arg - 1u;
            power++;
        }
        while (arg < fixed_point(0.0)) {
            arg = arg + 1u;
            power--;
        }

        static lut_type const log2_lut = lut_type::build_log2_lut();
        fixed_point result(1.0);
        BOOST_FOREACH(size_t i, boost::irange<size_t>(0, f, 1))
        {
            if (fixed_point(arg - fixed_point::wrap(T(1u) << (f - i - 1u))) >= fixed_point(0.0)) {
                arg = arg - fixed_point::wrap(T(1u) << (f- i -1u));

                result = fixed_point(result * log2_lut[i]);
            }
        }

        result = result * fixed_point(T(1u) << power);
        return result;
    }
}
