/// @brief computes scales for 

#include <boost/foreach.hpp>
#include <boost/range/irange.hpp>

#include <cmath>

namespace core { namespace cordic {
    // n is number of iterations in CORDIC algorithm
    template<size_t n, typename fp>
    double lut<n, fp>::compute_hyperbolic_scale(size_t n)
    {
        double scale(1.0);

        {
            size_t num(0);
            size_t repeated(4u);
            BOOST_FOREACH(size_t i, boost::irange<size_t>(1, n, 1))
            {
                scale *= std::sqrt(1 - std::powl(2.0, -2.0 * i));
                if (i == repeated && i != n - 1) {
                    scale *= std::sqrt(1 - std::powl(2.0, -2.0 * i++));

                    repeated = 3u * repeated  + 1u;
                }
            }
        }

        return scale;
    }
}}
