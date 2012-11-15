/// @provides in    ferred types for elementary functions

#include <boost/integer/static_min_max.hpp>
#include <boost/integer/static_log2.hpp>

namespace core { 
    namespace {
        using boost::mpl::eval_if;
        using boost::mpl::and_;
        using boost::static_unsigned_max;

        using boost::static_log2;
    }

/// LOG
    template<typename T, size_t n, size_t f, class op, class up>
    class fixed_point;

    template<typename T>
    class get_
    {
    public:
        typedef double log_type;
        typedef double sqrt_type;
        typedef double max_signed_type;

        typedef double sin_type;
        typedef sin_type cos_type;
        typedef double tan_type;

        typedef double arc_type;

        //typedef double product_type;
        //typedef double quotient_type;
        //typedef double sum_type;
        //typedef double diff_type;
    };

    template<typename T, size_t n, size_t f, class op, class up>
    class get_<fixed_point<T, n, f, op, up> >
    {
        typedef fixed_point<T, n, f, op, up> operand_type;

        struct log_info
        {
            static size_t const extra_bits =
                static_unsigned_max<static_log2<f>::value + 1u, n + static_log2<n>::value + 1u>::value;

            struct can_expand
            {
                enum { value = (f != 0) &&
                    (n + log_info::extra_bits + 1u < std::numeric_limits<boost::intmax_t>::digits) };
            };

            struct expanded
            {
                typedef fixed_point<
                    typename boost::int_t<n + extra_bits + 1u>::least,
                    n + extra_bits,
                    f,
                    op,
                    up
                > type;
            };

            struct same
            {
                typedef operand_type type;
            };
        };

    public:
        typedef typename eval_if<
            typename log_info::can_expand,
            typename log_info::expanded,
            typename log_info::same
        >::type log_type;

        typedef fixed_point<typename boost::uint_t<n / 2>::least, n / 2, f / 2, op, up> sqrt_type;

        typedef fixed_point<boost::uintmax_t,
            std::numeric_limits<boost::uintmax_t>::digits, f, op, up> max_unsigned_type;

        typedef fixed_point<boost::intmax_t,
            std::numeric_limits<boost::intmax_t>::digits, f, op, up> max_signed_type;

        typedef fixed_point<typename boost::int_t<1u + f>::least, 1u + f, f, op, up> sin_type;
        typedef sin_type cos_type;
        typedef typename quotient<sin_type, cos_type>::type tan_type;

        typedef max_unsigned_type exp_type;

        //typedef fixed_point<typename boost::uintmax_t<2u + f>::least, f, op, up> arcsin_type;
        //typedef arcsin_type arccos_type;
    };
}
