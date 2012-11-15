/// @brief provides CORDIC for ln function
/// @ref see C. Baumann, "A simple and fast look-up table method to compute the
/// exp(x) and ln(x) functions", 2004

namespace std {
    template<typename T, size_t n, size_t f, class op, class up>
    typename core::fixed_point<T, n, f, op, up>::log_type log(core::fixed_point<T, n, f, op, up> const& val)
    {
        typedef core::fixed_point<T, n, f, op, up> fp;
        typedef typename fp::log_type log_type;
        typedef core::cordic::lut<f, fp> lut;

        assert(("log2: argument must be positive", val >= fp(0)));
        if (val <= fp(0)) {
            throw std::exception("log2: argument must be positive");
        }

        // one need 1 bit to represent integer part of reals from [1.0, 2.0]
        typedef core::fixed_point<boost::uint_t<n + 1u>::least, f + 1u, f,
            op, up> work_type;

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

        // one can consider 0 < y = log(2, x) < 1 as x = 2^y
        // so CORDIC rotation is just a multiplication by 2^{1/2^i}:
        // 2^y = 2^{a1/2} * 2^{a2/4} * ... * 2^{ai/2^i}, where ai is from
        // {0, 1}
        static lut const inv_pow2_lut = lut::inv_pow2();

        work_type result(0);
        BOOST_FOREACH(size_t i, boost::irange<size_t>(0, f, 1))
        {
            if (work_type(arg * inv_pow2_lut[i]) >= work_type(1.0)) {
                arg = work_type(arg * inv_pow2_lut[i]);

                as_native(result) += T(1u) << (f - i - 1u);
            }
        }

        log_type const r0(log_type(result) + log_type(power));
        log_type const r1(r0 * work_type::CONST_1_LOG2E);

        return r1;
    }
}
