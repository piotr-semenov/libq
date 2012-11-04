/// @brief provides CORDIC for ln function
/// @ref see C. Baumann, "A simple and fast look-up table method to compute the
/// exp(x) and ln(x) functions", 2004

#include "./../../fixed_point_lib/src/CORDIC/lut/lut.hpp"

#include <boost/foreach.hpp>
#include <boost/range/irange.hpp>

namespace std {
    template<typename T, size_t n, size_t f, class op, class up>
    utils::number<T, n, f, op, up> log(utils::number<T, n, f, op, up> const& val)
    {
        typedef utils::number<T, n, f, op, up> fixed_point;
        typedef utils::cordic::lut<f, fixed_point> lut_type;

        assert(("log2: argument must be positive", val >= fixed_point(0)));
        if (val < fixed_point(0)) {
            throw std::exception("log2: argument must be positive");
        }

        // reduces argument to interval [1.0, 2.0]
        int power(0);
        fixed_point arg(val);
        while (arg >= fixed_point(2.0)) {
            as_native(arg) >>= 1u;
            power++;
        }
        while (arg < fixed_point(1.0)) {
            as_native(arg) <<= 1u;
            power--;
        }

        // one can consider 0 < y = log(2, x) < 1 as x = 2^y
        // so CORDIC rotation is just a multiplication by 2^{1/2^i}:
        // 2^y = 2^{a1/2} * 2^{a2/4} * ... * 2^{ai/2^i}, where ai is from
        // {0, 1}
        static lut_type const inv_roots_from2_lut = lut_type::build_inv_2roots_lut();

        fixed_point result(0);
        BOOST_FOREACH(size_t i, boost::irange<size_t>(0, f, 1))
        {
            fixed_point const test0(inv_roots_from2_lut[i]);
            fixed_point const test(arg * inv_roots_from2_lut[i]);
            if (fixed_point(arg * inv_roots_from2_lut[i]) >= fixed_point(1.0)) {
                arg = fixed_point(arg * inv_roots_from2_lut[i]);

                as_native(result) += T(1u) << (f - i - 1u);
            }
        }

        result += fixed_point(power);
        return result;
    }
}
