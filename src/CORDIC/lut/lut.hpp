/// @provides look-up table (LUT) for CORDIC stuff.
/// @ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"

#ifndef INC_CORDIC_LUT_HPP_
#define INC_CORDIC_LUT_HPP_

#include <boost/array.hpp>

namespace utils { namespace cordic {
    template<size_t n, typename fixed_point>
    class lut
        :    public boost::array<fixed_point, n>
    {
        typedef boost::array<fixed_point, n> base_class;
        typedef lut<n, fixed_point> this_class;

    public:
        static size_t const dim = n;
        typedef fixed_point fixed_point_type;

        static this_class build_arctan_lut();
        static this_class build_log2_lut();
        static this_class build_inv_2roots_lut();

        static double compute_circular_scale(size_t n);

    private:
        lut(base_class const& dat)
            :    base_class(dat){}
    };
}}

#include "./../../../fixed_point_lib/src/CORDIC/lut/arctan_lut.inl"
#include "./../../../fixed_point_lib/src/CORDIC/lut/log2_lut.inl"
#include "./../../../fixed_point_lib/src/CORDIC/lut/inv2roots_lut.inl"

#include "./../../../fixed_point_lib/src/CORDIC/lut/circular_scales.inl"

#endif
