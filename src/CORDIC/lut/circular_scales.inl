/// @brief computes scales for 

#include <boost/foreach.hpp>
#include <boost/range/irange.hpp>

#include <cmath>

namespace utils { namespace cordic {
    template<size_t n, typename fixed_point>
    double lut<n, fixed_point>::compute_circular_scale(size_t n)
    {
        double scale(1.0);

        BOOST_FOREACH(size_t i, boost::irange<size_t>(0, n, 1))
        {
            scale *= std::sqrt(1 + std::powl(2.0, -2.0 * i));
        }

        return scale;
    }
}}
