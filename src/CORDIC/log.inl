/// @brief provides CORDIC for ln function
/// @ref see C. Baumann, "A simple and fast look-up table method to compute the
/// exp(x) and ln(x) functions", 2004

#include "./../../fixed_point_lib/src/CORDIC/lut/lut.hpp"

#include <boost/foreach.hpp>
#include <boost/range/irange.hpp>

namespace std {
    template<typename T, size_t n, size_t f, class op, class up>
    core::fixed_point<T, n, f, op, up> log(core::fixed_point<T, n, f, op, up> const& val)
    {
        typedef core::fixed_point<T, n, f, op, up> fp;
        typedef core::cordic::lut<f, fp> lut;

        assert(("log2: argument must be positive", val >= fp(0)));
        if (val < fp(0)) {
            throw std::exception("log2: argument must be positive");
        }

        // reduces argument to interval [1.0, 2.0]
        int power(0);
        fp arg(val);
        while (arg >= fp(2.0)) {
            as_native(arg) >>= 1u;
            power++;
        }
        while (arg < fp(1.0)) {
            as_native(arg) <<= 1u;
            power--;
        }

        // approximation in case argument approaches 1.0
        if (arg - fp(1.0) <= fp(0.01)) {
            return fp((fp(power) / fp::CONST_LOG2E) + (arg - fp(1.0))); 
        }

        // one can consider 0 < y = log(2, x) < 1 as x = 2^y
        // so CORDIC rotation is just a multiplication by 2^{1/2^i}:
        // 2^y = 2^{a1/2} * 2^{a2/4} * ... * 2^{ai/2^i}, where ai is from
        // {0, 1}
        static lut const inv_pow2_lut = lut::inv_pow2();

        fp result(0);
        BOOST_FOREACH(size_t i, boost::irange<size_t>(0, f, 1))
        {
            if (fp(arg * inv_pow2_lut[i]) >= fp(1.0)) {
                arg = fp(arg * inv_pow2_lut[i]);

                as_native(result) += T(1u) << (f - i - 1u);
            }
        }

        result += fp(power);
        return fp(result / fp::CONST_LOG2E);
    }
}
