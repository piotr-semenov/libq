/// @brief provides CORDIC for cos function
/// @ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"

namespace std {
    template<typename T, size_t n, size_t f, class op, class up>
    core::fixed_point<T, n, f, op, up> cos(core::fixed_point<T, n, f, op, up> const& val)
    {
        typedef core::fixed_point<T, n, f, op, up> fp;
        BOOST_STATIC_ASSERT(std::numeric_limits<fp>::is_signed);

        // convergence interval for CORDIC rotations is [-pi/2, pi/2].
        // So one has to map input angle to that interval
        fp arg(0);
        int sign(-1);
        {
            // put argument to [-pi, pi] interval (with change of sign for
            // cos)
            fp const x = fp::CONST_PI - std::fmod(val, fp::CONST_2PI);
            if (x < -fp::CONST_PI_2) {     // convert to interval [-pi/2, pi/2]
                arg = x + fp::CONST_PI;

                sign = 1;
            }
            else if (x > fp::CONST_PI_2) {
                arg = x - fp::CONST_PI;

                sign = 1;
            }
            else {
                arg = x;
            }
        }

        typedef core::cordic::lut<f, fp> lut;
        static lut const angles = lut::circular();

        // normalization factor: see page 10, table 24.1 and pages 4-5, equations
        // (5)-(6)
        // factor converges to the limit 1.64676 very fast: it tooks 8 iterations
        // only. 8 iterations correpsonds to precision of size 0.007812 for
        // angle approximation
        static fp norm_factor(1.0 / lut::circular_scale(f));

        // rotation mode: see page 6
        // shift sequence is just 0, 1, ... (circular coordinate system)
        fp x(norm_factor), y(0.0), z(arg);
        fp x1, y1, z1;
        BOOST_FOREACH(size_t i, boost::irange<size_t>(0, f, 1))
        {
            int const sign = (z > fp(0)) ? 1 : -1;
            fp const x_scaled = fp::wrap(sign * (x.value() >> i));
            fp const y_scaled = fp::wrap(sign * (y.value() >> i));

            x1 = fp(x - y_scaled);
            y1 = fp(y + x_scaled);
            z1 = fp(z - fp((sign > 0) ? angles[i] : -angles[i]));

            x = x1; y = y1; z = z1;
        }

        return (sign > 0) ? x : - x;
    }
}
