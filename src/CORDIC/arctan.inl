/// @brief provides CORDIC for arccos function
/// @ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"

#include "./../../fixed_point_lib/src/CORDIC/lut/lut.hpp"

namespace std {
    template<typename T, size_t n, size_t f, class op, class up>
    utils::number<T, n, f, op, up> atan(utils::number<T, n, f, op, up> val)
    {
        typedef utils::number<T, n, f, op, up> fp;
        typedef utils::cordic::lut<f, fp> lut;

        static lut const angles = lut::build_arctan_lut();

        // vectoring mode: see page 10, table 24.2
        // shift sequence is just 0, 1, ... (circular coordinate system)
        fp x(1.0), y(val), z(0.0);
        BOOST_FOREACH(size_t i, boost::irange<size_t>(0, f, 1))
        {
            int const sign = ((x.value() > 0)? +1 : -1) * ((y.value() > 0)? +1 : -1);

            fp::value_type const store(x.value());
            x = x + fp::wrap(sign * (y.value() >> i));
            y = y - fp::wrap(sign * (store >> i));
            z = (sign > 0)? z + angles[i] : z - angles[i];
        }

        return z;
    }
}
