/// @brief provides CORDIC for cos function
/// @ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"

#include "./../../fixed_point_lib/src/CORDIC/lut/arctan_lut.hpp"

namespace std {
    template<typename T, size_t n, size_t f, class op, class up>
    utils::number<T, n, f, op, up> cos(utils::number<T, n, f, op, up> const& val)
    {
        typedef utils::number<T, n, f, op, up> fixed_point;
        BOOST_STATIC_ASSERT(std::numeric_limits<fixed_point>::is_signed);
        BOOST_STATIC_ASSERT(64u > f);

        static utils::cordic::arctan_lut<64u, fixed_point> const lut;

        // normalization factor: see page 10, table 24.1 and pages 4-5, equations
        // (5)-(6)
        // factor converges to the limit 1.64676 very fast: it tooks 8 iterations
        // only. 8 iterations correpsonds to precision of size 0.007812 for
        // angle approximation
        static fixed_point norm_factor(1.0 / 1.64676);

        assert(("illegal argument", std::fabs(val) <= fixed_point(1.0)));

        // rotation mode: see page 6
        // shift sequence is just 0, 1, ... (circular coordinate system)
        fixed_point x(norm_factor), y(0.0), z(val);
        fixed_point x1, y1, z1;
        BOOST_FOREACH(size_t i, boost::irange<size_t>(0, std::min(64u, f), 1))
        {
            int const sign = (z > fixed_point(0)) ? 1 : -1;
            fixed_point const x_scaled = fixed_point::wrap(sign * (x.value() >> i));
            fixed_point const y_scaled = fixed_point::wrap(sign * (y.value() >> i));

            x1 = fixed_point(x - y_scaled);
            y1 = fixed_point(y + x_scaled);
            z1 = fixed_point(z - fixed_point((sign > 0) ? lut[i] : -lut[i]));

            x = x1; y = y1; z = z1;
        }

        return x;
    }
}
