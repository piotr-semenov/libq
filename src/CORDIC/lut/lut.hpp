/// @provides look-up table (LUT) for CORDIC stuff.
/// @ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"

#ifndef INC_CORE_CORDIC_LUT_HPP_
#define INC_CORE_CORDIC_LUT_HPP_

#include <boost/array.hpp>

#include <boost/foreach.hpp>
#include <boost/range/irange.hpp>

#include <cmath>

namespace core { namespace cordic {
    /// @brief look-up table for CORDIC algorithms
    template<size_t n, typename fp>
    class lut
        :    public boost::array<fp, n>
    {
        typedef boost::array<fp, n> base_class;
        typedef lut<n, fp> this_class;

    public:
        static size_t const dim = n; ///< size of lut
        typedef fp fixed_point_type; ///< type of fixed-point numbers

        /// @brief builds lut for angles in case of CORDIC rotations in circular
        /// coordinates
        static this_class circular();

        /// @brief builds lut for angles in case of CORDIC rotations in hyperbolic
        /// coordinates. It is not use repeated iterations. All values are unique
        static this_class hyperbolic_wo_repeated_iterations();

        /// @brief builds lut for 2^i-powers of 2 in case of arguments from [1, 2]
        /// for computing exp
        static this_class pow2();

        /// @brief builds lut for 1/2^i-powers of 2 in case of arguments from [0, 1]
        /// for computng log2
        static this_class inv_pow2();

        /// @brief computes scale of n CORDIC-rotations in circular coordinates
        static double circular_scale(size_t n);

        /// @brief builds lut for scales of n CORDIC rotations in case of circular
        /// coordinates
        static this_class circular_scales();

        /// @brief computes scale of n CORDIC-rotations in hyperbolic coordinates.
        /// It uses repeated iterations for convergence.
        static double hyperbolic_scale_with_repeated_iterations(size_t n);

    private:
        lut(base_class const& dat)
            :    base_class(dat){}
    };
}}

#include "./../../../fixed_point_lib/src/CORDIC/lut/arctan_lut.inl"
#include "./../../../fixed_point_lib/src/CORDIC/lut/arctanh_lut.inl"

#include "./../../../fixed_point_lib/src/CORDIC/lut/pow2_lut.inl"
#include "./../../../fixed_point_lib/src/CORDIC/lut/inv_pow2_lut.inl"

#include "./../../../fixed_point_lib/src/CORDIC/lut/circular_scales.inl"
#include "./../../../fixed_point_lib/src/CORDIC/lut/hyperbolic_scale.inl"

#endif
