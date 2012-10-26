/// @provides look-up table (LUT) for CORDIC in case of circular rotation
/// @ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"

#ifndef INC_ARCTANH_LUT_HPP_
#define INC_ARCTANH_LUT_HPP_

#include <boost/array.hpp>

namespace utils { namespace cordic {
    template<size_t n, typename fixed_point>
    class arctan_lut
        :    public boost::array<fixed_point, n>
    {
        typedef boost::array<fixed_point, n> base_class;
        typedef arctan_lut<n, fixed_point> this_class;

        static base_class build();

    public:
        static size_t const dim = n;
        typedef fixed_point fixed_point_type;

        arctan_lut()
            :    base_class(build()){};
    };
}}

#include "./../../../fixed_point_lib/src/CORDIC/lut/arctan_lut.inl"

#endif
