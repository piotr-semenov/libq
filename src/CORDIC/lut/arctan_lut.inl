/// @brief implements look-up table for trigonometric functions
/// @ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"

#include <boost/foreach.hpp>
#include <boost/range/irange.hpp>

#include <boost/integer.hpp>

#include <cmath>

namespace utils { namespace cordic {
    /// @ref page 5, equation 7, m = 1 (circular coordinate system)
    template<size_t n, typename fixed_point>
    typename arctan_lut<n, fixed_point>::base_class arctan_lut<n, fixed_point>::build()
    {
        base_class table;

        // shift sequence is just 0, 1, 2, 3, ..., i, ...
        /// @ref page 10, table 24.1, m = 1
        BOOST_FOREACH(size_t i, boost::irange<size_t>(0, n, 1))
        {
            table[i] = fixed_point(std::atan(1.0 / std::pow(2.0, double(i))));
        }

        return table;
    }
}}
