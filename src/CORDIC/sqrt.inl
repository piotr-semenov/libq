/// @brief provides CORDIC for cos function
/// @ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"

namespace std {
    /// @brief computes square root for fixed-point by long-algorithm: to enable
    /// sqrt computation in case if no built-in integral type can represent
    /// (2*f) bits
    /// @ref http://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Binary_numeral_system_.28base_2.29
    template<typename T, size_t n, size_t f, class op, class up>
    utils::number<T, n, f, op, up> sqrt(utils::number<T, n, f, op, up> const& x)
    {
        typedef utils::number<T, n, f, op, up> type;
        typedef utils::number<T, n, (f >> 1), op, up> root_type;
        typedef type::value_type word_type;

        assert(("sqrt parameter is negative", x > type(0)));
        assert(("one has to have at least one integer bit", n - f >= 1u));;
        if (x < type(0)) {
            throw std::exception("sqrt parameter is negative");
        }

        // procedure logics: we search for max integer which square is less than x
        // so for fixed-point square root format is: [n + m/2 | m/2] where
        // n is total bits of input integer and m is its fractional

        // integer to get square root of
        word_type op(x.value());

        word_type res(0);

        // "one" starts at the highest power of four <= than the argument
        word_type one = word_type(1u) << (((n-1) & 1) ? n-2 : n-1);
        while (one > op) {
            one >>= 2;
        }

        while (one != 0) {
            if (op >= res + one) {
                op = op - (res + one);
                res = one + (res >> 1u);
            }
            else {
                res >>= 1u;
            }
            one >>= 2u;
        }

        type result = type::wrap(word_type(res) << (f >> 1));
        if (f & 1) {
            result = result * type::CONST_SQRT2;
        }

        return result;
    }
}
