/** @file mult_of.hpp
    @copyright (c) 2016 Piotr K. Semenov (piotr.k.semenov at gmail dot com)

    Distributed under the New BSD License. (See accompanying file LICENSE)

*/

#ifndef INC_LIBQ_DETAILS_MULT_OF_HPP_
#define INC_LIBQ_DETAILS_MULT_OF_HPP_

#include "libq/details/fixed_point_common.hpp"

namespace libq {
namespace details {

#if 0
template<typename T, typename U>
class mult_of
    : public type_promotion_base<T, 0, 0, 0> 
{
    static_assert(std::is_same<T, U>::value,
                  "unexpected use of multiplication_promoted_type_of template");
};
#else
template<typename T, typename U>
class mult_of;

#endif

/**
 @note User has no need to take care about "fixed-point" promotion.
 Presently, std::common_type<L, R>::type equals to type having the max number
 of significant bits.
 @note If n1 = max possible word size for unsigned integers and T2 is signed
 than you will get the error.
*/
template<
    typename T1,
    std::size_t n1,
    std::size_t f1,
    int e1,
    typename T2,
    std::size_t n2,
    std::size_t f2,
    int e2,
    class op,
    class up>
class mult_of<libq::fixed_point<T1, n1, f1, e1, op, up>, libq::fixed_point<T2, n2, f2, e2, op, up> >
    : public std::conditional<
        (n2 + f2 < n1 + f1),
        type_promotion_base<
            libq::fixed_point<
                typename std::conditional<
                    (std::numeric_limits<T1>::is_signed || std::numeric_limits<T2>::is_signed) &&
                    n1 + f1 <= std::numeric_limits<typename std::make_signed<T1>::type>::digits,
                    typename std::make_signed<T1>::type,
                    T1
                >::type,
                n1,
                f1,
                e1,
                op,
                up
            >,
            n2,
            f2,
            e2
        >,
        type_promotion_base<
            libq::fixed_point<
                typename std::conditional<
                    (std::numeric_limits<T1>::is_signed || std::numeric_limits<T2>::is_signed) &&
                    n1 + f1 <= std::numeric_limits<typename std::make_signed<T1>::type>::digits,
                    typename std::make_signed<T2>::type,
                    T2
                >::type,
                n2,
                f2,
                e2,
                op,
                up
            >,
            n1,
            f1,
            e1
        >
     >::type
{
};

}  // namespace details
}  // namespace libq

#endif  // INC_LIBQ_DETAILS_MULT_OF_HPP_
