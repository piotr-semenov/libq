/// @brief provides CORDIC for cos function
/// @ref see H. Dawid, H. Meyr, "CORDIC Algorithms and Architectures"

namespace std {
    template<typename T, size_t n, size_t f, class op, class up>
    utils::number<T, n, f, op, up> cos(utils::number<T, n, f, op, up> const& x)
    {
        #define iterations 20

        static double lut[64] = {

        }
    }
}
