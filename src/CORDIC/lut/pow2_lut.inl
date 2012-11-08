/// @brief implements look-up table for log2 function
/// @ref see C. Baumann, "A simple and fast look-up table method to compute the
/// exp(x) and ln(x) functions", 2004

#include <boost/foreach.hpp>
#include <boost/range/irange.hpp>

#include <boost/integer.hpp>

#include <cmath>

namespace core { namespace cordic {
    template<size_t n, typename fp>
    typename lut<n, fp> lut<n, fp>::pow2()
    {
        base_class table;

        /// @ref see C. Baumann, "A simple and fast look-up table method to compute
        /// the exp(x) and ln(x) functions", 2004
        BOOST_FOREACH(int i, boost::irange<int>(1, n+1, 1))
        {
            table[i-1] = fp(std::powl(2.0, std::powl(2.0, -i)));
        }

        return this_class(table);
    }
}}
