/// @brief provides CORDIC for arccos function
/// @ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"

#include "./../../fixed_point_lib/src/CORDIC/lut/lut.hpp"

namespace std {
    template<typename T, size_t n, size_t f, class op, class up>
    typename core::fixed_point<T, n, f, op, up>::acos_type acos(core::fixed_point<T, n, f, op, up> val)
    {
        typedef core::fixed_point<T, n, f, op, up> fp;
        typedef typename fp::acos_type result_type;
        typedef core::cordic::lut<f, fp> lut;

        assert(("argument has to be from interval [1.0, 1.0]", val <= fp(1.0)));
        if (val > fp(1.0)) {
            throw std::exception("acos: argument must be from [-1.0, 1.0]");
        }
        if (val == fp(1.0)) {
            return result_type::wrap(0);
        }
        else if (val == fp(-1.0)) {
            return result_type::CONST_PI;
        }
        else if (val == fp::wrap(0)) {
            return result_type::CONST_PI_2;
        }

        bool const is_negative = val.value() < 0;
        val = std::fabs(val);

        static lut const angles = lut::circular();
        static lut const scales = lut::circular_scales();

        // rotation mode: see page 6
        // shift sequence is just 0, 1, ... (circular coordinate system)
        result_type x(1.0), y(0.0), z(0.0);
        BOOST_FOREACH(size_t i, boost::irange<size_t>(0, f, 1))
        {
            int sign(0);
            if (val <= x) {
                sign = (y < 0.0) ? -1 : +1;
            }
            else {
                sign = (y < 0.0) ? +1 : -1;
            }

            result_type::word_type const storage(x.value());
            x = x - result_type::wrap(sign * (y.value() >> i));
            y = y + result_type::wrap(sign * (storage >> i));
            z = (sign > 0)? z + angles[i] : z - angles[i];
            val = val * scales[i]; // multiply by square of K(n)
        }

        return (is_negative) ? result_type(result_type::CONST_PI - std::fabs(z)) :
            std::fabs(result_type(z));
    }
}
