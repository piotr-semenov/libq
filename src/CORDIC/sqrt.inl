/// @brief provides CORDIC for cos function
/// @ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"

namespace std {
    /// @brief computes square root for fixed-point by long-algorithm
    template<typename T, size_t n, size_t f, class op, class up>
    utils::number<T, n, f, op, up> sqrt(utils::number<T, n, f, op, up> const& val)
    {
        typedef utils::number<T, n, f, op, up> type;

        assert(("sqrt parameter is negative", val > type(0.0)));
        if (val == type(0)) {
            return type(0);
        }
        else if (val == type(1)) {
            return type(1);
        }
        type root(1.0);
        if (val < type(1.0)) {
            while (val <= type(root * root)) {
                as_native(root) >>= 1u;
            }
        }
        else {
            while (type(root * root) <= val) {
                as_native(root) <<= 1u;
            }
            as_native(root) >>= 1u;
        }

        // square root fixed-point format is (n - m/2) for total and m/2 is for
        // fractionals
        type residue(root);
        for (size_t i(1u); i != f; ++i) {
            as_native(residue) >>= 1u;
            if (type((root + residue) * (root + residue)) <= val) {
                root = type(root + residue);
            }
        }

        return root;
    }
}
