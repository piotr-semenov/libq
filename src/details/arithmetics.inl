/// @brief provides redefinition for floating-point in case of fixed-point types

#include <boost/integer/integer_mask.hpp>

namespace std {
    /// @brief computes absolute value
    template<typename T, size_t n, size_t f, class op, class up>
    utils::number<T, n, f, op, up> fabs(utils::number<T, n, f, op, up> const& x)
    {
        typedef utils::number<T, n, f, op, up> type;

        return (x < type(0)) ? -x : x;
    }

    /// @brief ceil
    template<typename T, size_t n, size_t f, class op, class up>
    utils::number<T, n, f, op, up> ceil(utils::number<T, n, f, op, up> const& x)
    {
        typedef utils::number<T, n, f, op, up> type;
        static T const mask(boost::low_bits_mask_t<f>::least);

        type::value_type val = x.value();
        if ((val & mask) != 0) {
            val += T(1u) << f;
        }

        return type::wrap(val);
    }

    /// @brief floor
    template<typename T, size_t n, size_t f, class op, class up>
    utils::number<T, n, f, op, up> floor(utils::number<T, n, f, op, up> const& x)
    {
        typedef utils::number<T, n, f, op, up> type;
        static T const mask(boost::high_bits_mask_t<std::numeric_limits<T>::digits -
            f>::least);

        return type::wrap(x.value() & mask);
    }

    /// @brief rounds
    template<typename T, size_t n, size_t f, class op, class up>
    utils::number<T, n, f, op, up> round(utils::number<T, n, f, op, up> const& x)
    {
        typedef utils::number<T, n, f, op, up> type;
        static T const mask(boost::low_bits_mask_t<f>::least);

        type::value_type val = x.value() + (x > type(0)) ? type(0.5).value() :
            type(-0.5).value();

        return type::wrap(val & mask);
    }

    /// @brief computes fixed-point remainder of x/y
    template<typename T, size_t n, size_t f, class op, class up>
    utils::number<T, n, f, op, up> fmod(utils::number<T, n, f, op, up> const& a,
        utils::number<T, n, f, op, up> const& b)
    {
        typedef utils::number<T, n, f, op, up> fixed_point;
        static T const mask(boost::low_bits_mask_t<f>::sig_bits);

        fixed_point const fractional = fixed_point::wrap(
            fixed_point(a / b).value() & mask
        );
        return fractional;
    }

    /// @brief computes square root for fixed-point by long-algorithm
    template<typename T, size_t n, size_t f, class op, class up>
    utils::number<typename boost::uint_t<n - (f >> 1u)>::least, n - (f >> 1u),
        (f >> 1u), op, up> sqrt(utils::number<T, n, f, op, up> const& x)
    {
        typedef utils::number<T, n, f, op, up> type;
        typedef type::value_type word_type;

        typedef utils::number<T, n - (f >> 1u), (f >> 1u)> root;

        assert(("sqrt parameter is negative", x < type(0.0)));

        // square root fixed-point format is (n - m/2) for total and m/2 is for
        // fractionals
        word_type op = x.value();

        word_type res(0);
        word_type one = word_type(1) << (std::numeric_limits<word_type>::digits - 1);

        while (one > op) {
            one >>= 2u;
        }

        while (one != word_type(0)) {
            if (op >= res + one) {
                op -= res + one;
                res += one << 1u;
            }

            res >>= 1u;
            one >>= 2u;
        }

        return root::wrap(res << root::fractionals);
    }
}
