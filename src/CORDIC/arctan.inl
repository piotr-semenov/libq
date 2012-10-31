/// @brief provides CORDIC for arccos function
/// @ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"

#include "./../../fixed_point_lib/src/CORDIC/lut/lut.hpp"

namespace std {
    template<typename T, size_t n, size_t f, class op, class up>
    utils::number<T, n, f, op, up> atan(utils::number<T, n, f, op, up> val)
    {
        typedef utils::number<T, n, f, op, up> fixed_point;
        using utils::cordic::lut;

        bool const is_negative = (val < fixed_point(0));
        assert(("argument has to be from interval [-1.0, 1.0]",
            std::fabs(val) <= fixed_point(1.0)));

        typedef lut<f, fixed_point> lut_type;
        static lut_type const angles = lut_type::build_arctan_lut();

        // vectoring mode: see page 10, table 24.2
        // shift sequence is just 0, 1, ... (circular coordinate system)
        fixed_point x(1.0), y(val), z(0.0);
        fixed_point x1, y1;
        BOOST_FOREACH(size_t i, boost::irange<size_t>(0, f, 1))
        {
            int const sign = ((x > fixed_point(0)) ? 1 : -1) * ((y > fixed_point(0)) ? 1 : -1);

            fixed_point const x_scaled = fixed_point::wrap(sign * (x.value() >> i));
            fixed_point const y_scaled = fixed_point::wrap(sign * (y.value() >> i));

            x1 = fixed_point(x + y_scaled);
            y1 = fixed_point(y - x_scaled);

            x = x1; y = y1;
            z = z + ((sign > 0) ? angles[i] : -angles[i]);
        }

        return z;
    }
}
