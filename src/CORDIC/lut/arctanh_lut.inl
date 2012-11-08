/// @brief implements look-up table for trigonometric functions
/// @ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"

#include <boost/foreach.hpp>
#include <boost/range/irange.hpp>

#include <boost/integer.hpp>

#include <cmath>

namespace core { namespace cordic {
    /// @ref page 5, equation 7, m = -1 (hyperbolic coordinate system)
    template<size_t n, typename fp>
    typename lut<n, fp> lut<n, fp>::build_arctanh_lut()
    {
        base_class table;

        // Shift sequence is 1, 2, 3, ..., i, ... united with (3k + 1, k = 1, 2, ...).
        // But lut uses only 1, 2, 3 ,... sequence.
        /// @ref page 10, table 24.1, m = -1
        {
            BOOST_FOREACH(size_t i, boost::irange<size_t>(0, n, 1))
            {
                double const arg = 1.0 / std::pow(2.0, double(i + 1u));
                table[i] = fp(-0.5 * std::log(1 - arg) + 0.5 * std::log(1 + arg));
            }
        }

        return this_class(table);
    }
}}
