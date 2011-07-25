/// @provides look-up table for CORDIC for trigonometric functions

#ifndef INC_ARCTANH_LUT_HPP_
#define INC_ARCTANH_LUT_HPP_

#include <boost/array.hpp>

namespace utils { namespace cordic {
    template<size_t n>
    class arctan_lut
        :    public boost::array<double, n>
    {
        typedef boost::array<double, n> base_class;
        typedef arctan_lut<n> this_class;

        static base_class build();

    public:
        static size_t const dim = n;
        typedef base_class lut_type;

        arctan_lut()
            :    this_class(build()){};
    };
}}

#include "./../../Common/cordic/arctan_lut.inl"

#endif
