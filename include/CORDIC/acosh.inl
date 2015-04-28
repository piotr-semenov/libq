// acosh.inl
//
// Copyright (c) 2014-2015 Piotr K. Semenov (piotr.k.semenov at gmail dot com)
// Distributed under the New BSD License. (See accompanying file LICENSE)

/*!
 \file acosh.inl

 Provides CORDIC for tanh function as a ratio of sinh and tanh
*/

namespace libq {
namespace details {
/*!
*/
template<typename T>
class acosh_of
{};

template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
class acosh_of<libq::fixed_point<T, n, f, e, op, up> >
{
public:
    typedef typename
        log_of<T, n, f, e, op, up>::promoted_type::to_unsigned_type promoted_type;
};
} // details
} // libq

namespace std {
/*!
 \brief computes acosh as logarithm
*/
template<typename T, std::size_t n, std::size_t f, int e, class op, class up>
typename libq::details::acosh_of<libq::fixed_point<T, n, f, e, op, up> >::promoted_type
    acosh(libq::fixed_point<T, n, f, e, op, up> _val)
{
    typedef libq::fixed_point<T, n, f, e, op, up> Q;
    typedef typename libq::details::acosh_of<Q>::promoted_type result_type;

    assert(("[std::acosh] argument is not from [1.0, +inf)", _val > Q(1.0f)));
    if (_val < Q(1.0f)) {
        throw std::logic_error("[std::acosh] argument is not from [1.0, +inf)");
    }

    return result_type(
        std::log(
            _val + std::sqrt(_val * _val - 1)
        )
    );
}
} // std
