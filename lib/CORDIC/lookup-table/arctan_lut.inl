/// @brief implements look-up table for trigonometric functions

#include <boost/foreach.hpp>
#include <boost/range/irange.hpp>

#include <cmath>

namespace utils { namespace cordic {
    template<size_t n>
    typename arctan_lut<n>::lut_type arctan_lut<n>::build()
    {
        lut_type table;

        BOOST_FOREACH(size_t i, boost::irange<size_t>(0, n+1, 1))
        {
            double const tan_value(1 << i);

            table[i] = std::atan(1.0 / tan_value);
        }

        return table;
    }
}}
