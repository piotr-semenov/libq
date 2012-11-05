/// @brief provides CORDIC for arccos function
/// @ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"

#include "./../../fixed_point_lib/src/CORDIC/lut/lut.hpp"

namespace std {
    template<typename T, size_t n, size_t f, class op, class up>
    utils::number<T, n, f, op, up> acos(utils::number<T, n, f, op, up> val)
    {
        typedef utils::number<T, n, f, op, up> fp;
        typedef utils::cordic::lut<f, fp> lut;

        BOOST_STATIC_ASSERT(n - f >= 2); // to enable pi representative
        assert(("argument has to be from interval [1.0, 1.0]", val <= fp(1.0)));
        if (val > fp(1.0)) {
            throw std::exception("acos: argument must be from [-1.0, 1.0]");
        }
        if (val == fp(1.0)) {
            return fp::wrap(0);
        }
        else if (val == fp(-1.0)) {
            return fp::CONST_PI;
        }
        else if (val == fp::wrap(0)) {
            return fp::CONST_PI_2;
        }

        bool const is_negative = val.value() < 0;
        val = std::fabs(val);

        static lut const angles = lut::build_arctan_lut();
        static lut const scales = lut::build_circular_scales_lut();

        // rotation mode: see page 6
        // shift sequence is just 0, 1, ... (circular coordinate system)
        fp x(1.0), y(0.0), z(0.0);
        BOOST_FOREACH(size_t i, boost::irange<size_t>(0, f, 1))
        {
            int sign(0);
            if (val <= x) {
                sign = (y < 0.0) ? -1 : +1;
            }
            else {
                sign = (y < 0.0) ? +1 : -1;
            }

            fp::value_type const storage(x.value());
            x = x - fp::wrap(sign * (y.value() >> i));
            y = y + fp::wrap(sign * (storage >> i));
            z = (sign > 0)? z + angles[i] : z - angles[i];
            val = val * scales[i]; // multiply by square of K(n)
        }

        return (is_negative) ? fp(fp::CONST_PI - std::fabs(z)) : std::fabs(z);
    }
}
