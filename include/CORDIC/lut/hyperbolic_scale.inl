/// @brief computes scales for CORDIC rotations in case of hyperbolic coordinates

namespace core { namespace cordic {
    template<size_t n, typename fp>
    double lut<n, fp>::hyperbolic_scale_with_repeated_iterations(size_t n)
    {
        double scale(1.0);

        size_t repeated(4u);
        BOOST_FOREACH(size_t i, boost::irange<size_t>(1, n, 1))
        {
            scale *= std::sqrt(1 - std::pow(2.0, -2.0 * i));
            if (i == repeated && i != n - 1) {
                scale *= std::sqrt(1 - std::pow(2.0, -2.0 * i++));

                repeated = 3u * repeated  + 1u;
            }
        }

        return scale;
    }
}}
