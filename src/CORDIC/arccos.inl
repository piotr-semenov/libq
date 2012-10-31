/// @brief provides CORDIC for arccos function
/// @ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"

#include "./../../fixed_point_lib/src/CORDIC/lut/lut.hpp"

namespace std {
    template<typename T, size_t n, size_t f, class op, class up>
    utils::number<T, n, f, op, up> acos(utils::number<T, n, f, op, up> val)
    {
        typedef utils::number<T, n, f, op, up> fixed_point;
        using utils::cordic::lut;

        // one need integer part bits to represent pi number
        #define pi fixed_point(3.141592653589793)
        assert(("fixed-point format is illegal", n - f >= 2));

        bool const is_negative = (val < fixed_point(0));
        val = std::fabs(val);   // make the value positive
        assert(("argument has to be from interval [0.0, 1.0]", val <= fixed_point(1.0)));

        typedef lut<f, fixed_point> lut_type;
        static lut_type const angles = lut_type::build_arctan_lut();
        static lut_type const scales = lut_type::build_circular_scales_lut();

        // rotation mode: see page 6
        // shift sequence is just 0, 1, ... (circular coordinate system)
        fixed_point x(1.0), y(0.0), z(0.0);
        fixed_point x1, y1;
        BOOST_FOREACH(size_t i, boost::irange<size_t>(0, f, 1))
        {
            int s, sign(0);
            if (y < 0.0) {
                s = -1;
            }
            else {
                s = +1;
            }

            if (val <= x) {
                sign = +s;
            }
            else {
                sign = -s;
            }

            fixed_point const x_scaled = fixed_point::wrap(sign * (x.value() >> i));
            fixed_point const y_scaled = fixed_point::wrap(sign * (y.value() >> i));

            x1 = fixed_point(x - y_scaled);
            y1 = fixed_point(y + x_scaled);

            x = x1; y = y1;
            z = z + ((sign > 0) ? angles[i] : -angles[i]);

            // multiply by square of scale parameter K(n)
            val = fixed_point(val * scales[i]);
        }

        return (is_negative) ? fixed_point(pi - std::fabs(z)) : std::fabs(z);

        #undef pi
    }
}
