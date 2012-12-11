/// @brief computes scales for CORDIC-rotations in case of circular coordiantes

namespace core { namespace cordic {
    template<size_t n, typename fp>
    double lut<n, fp>::circular_scale(size_t n)
    {
        double scale(1.0);

        BOOST_FOREACH(size_t i, boost::irange<size_t>(0, n, 1))
        {
            scale *= std::sqrt(1.0 + std::pow(2.0, -2.0 * i));
        }

        return scale;
    }

    template<size_t n, typename fp>
    lut<n, fp> lut<n, fp>::circular_scales()
    {
        base_class scales;

        BOOST_FOREACH(size_t i, boost::irange<size_t>(0, n, 1))
        {
            scales[i] = std::sqrt(1.0 + std::pow(2.0, -2.0 * i));
        }

        return this_class(scales);
    }
}}
