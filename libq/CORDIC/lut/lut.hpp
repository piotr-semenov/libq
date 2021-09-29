/** @file lut.hpp
    @brief Provides a look-up table (LUT) for CORDIC stuff.
    @copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)

    Distributed under the New BSD License. (See accompanying file LICENSE)
    @note See H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures".
*/

#ifndef INC_LIBQ_CORDIC_LUT_LUT_HPP_
#define INC_LIBQ_CORDIC_LUT_LUT_HPP_

#include "libq/details/fixed_point_common.hpp"

#include <array>

namespace libq {
namespace cordic {

/** @brief Look-up table for CORDIC algorithms.
    @tparam n
    @tparam Q
*/
template <std::size_t n, typename Q>
class lut : public std::array<Q, n>
{
    using Base_class = std::array<Q, n>;
    using This_class = lut<n, Q>;

    explicit lut(Base_class const& _dat)
        : Base_class(_dat)
    {}

public:
    /// @brief type of fixed-point numbers
    using fixed_point_type = Q;

    /// @brief size of LUT
    static constexpr std::size_t const dim = n;

    /** @brief Creates the LUT for angles in case of CORDIC rotations are
     performed for circular coordinates.
    */
    static This_class
        circular();

    /** @brief Creates the LUT for angles in case of CORDIC rotations are
        performed in hyperbolic coordinates.

        @note This does not use the repeated iterations. All values are unique.
    */
    static This_class
        hyperbolic_wo_repeated_iterations();

    /** @brief Creates the LUT of \f$2^{2^{-i}}\f$ for n positions.
        @note This LUT is used for exp function.
    */
    static This_class
        pow2();

    /** @brief Creates the LUT of @f$ \frac1{2 ^ \frac1{2^i}} @f$ for @f$ n @f$
        positions.
        @note This LUT is used for log2 function.
    */
    static This_class
        inv_pow2();

    /// @brief Computes the scale of n CORDIC-rotations in circular coordinates.
    static double
        circular_scale(std::size_t const _n);

    /** @brief Creates the LUT for scales of n CORDIC-rotations in case of
        circular coordinates.
    */
    static This_class
        circular_scales();

    /** @brief Computes the scale of n CORDIC-rotations in hyperbolic
        coordinates.
        @note This uses repeated iterations for convergence.
    */
    static double
        hyperbolic_scale_with_repeated_iterations(std::size_t _n);
};

}  // namespace cordic
}  // namespace libq

/// @bug #include after code
#include "libq/CORDIC/lut/arctan_lut.hpp"
#include "libq/CORDIC/lut/arctanh_lut.hpp"
#include "libq/CORDIC/lut/pow2_lut.hpp"
#include "libq/CORDIC/lut/inv_pow2_lut.hpp"
#include "libq/CORDIC/lut/circular_scales.hpp"
#include "libq/CORDIC/lut/hyperbolic_scale.hpp"

#endif  // INC_LIBQ_CORDIC_LUT_LUT_HPP_
