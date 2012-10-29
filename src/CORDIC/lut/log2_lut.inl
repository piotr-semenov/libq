/// @brief implements look-up table for trigonometric functions
/// @ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"

#include <boost/foreach.hpp>
#include <boost/range/irange.hpp>

#include <boost/integer.hpp>

#include <cmath>

namespace utils { namespace cordic {
    /// @ref page 5, equation 7, m = 1 (circular coordinate system)
    template<size_t n, typename fixed_point>
    typename lut<n, fixed_point> lut<n, fixed_point>::build_log2_lut()
    {
        base_class table;

        /// @ref see C. Baumann, "A simple and fast look-up table method to compute
        /// the exp(x) and ln(x) functions", 2004
        BOOST_FOREACH(int i, boost::irange<int>(1, n+1, 1))
        {
            table[i-1] = fixed_point(std::powl(2.0, std::powl(2.0, -i)));
        }

        return this_class(table);
    }
}}
